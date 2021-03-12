#include "faiss_grpc_serv.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <cmath>
#include <map>
#include <utility>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <spdlog/spdlog.h>
#include <spdlog/common.h>
#include "faiss_rpc.grpc.pb.h"

#include <filesystem>
// #include <pthread.h>
#include <utility>
#include <map>
#include <exception>
// #include <libfswatch/libfswatch.h>
#include <faiss/index.h>
#include <faiss/index_io.h>
#include <nlohmann/json.hpp>
#include <nlohmann/adl_serializer.hpp>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReaderWriter;
using grpc::Status;
using hszofficial::faiss_rpc::Query;
using hszofficial::faiss_rpc::BatchQuery;
using hszofficial::faiss_rpc::Response;
using hszofficial::faiss_rpc::BatchResponse;
using hszofficial::faiss_rpc::TopK;
using hszofficial::faiss_rpc::ResponseStatus;
using hszofficial::faiss_rpc::ResponseStatus_Stat;
using hszofficial::faiss_rpc::ResponseStatus_Stat_SUCCEED;
using hszofficial::faiss_rpc::ResponseStatus_Stat_FAILED;
using hszofficial::faiss_rpc::FaissRpc;


using faiss::Index;
using faiss::read_index;
namespace fs = std::filesystem;
// 64-bit int
using idx_t = faiss::Index::idx_t;
using json = nlohmann::json;

namespace faiss_index_manager{
    //endsWith c++20才可以替换掉
    bool endsWith(std::string s,std::string sub){
        return s.rfind(sub)==(s.length()-sub.length())? true:false;
    }
    //DNotEqualException 维度不一致异常
    class DNotEqualException : public std::exception{
        public:
            DNotEqualException() : message("d not equal"){}
            DNotEqualException(std::string str) : message("d not equal," + str) {}
            ~DNotEqualException() throw () {
            }

            virtual const char* what() const throw () {
                return message.c_str();
            }

        private:
            std::string message;
    };

    class IndexManager{
        public:
            ~IndexManager(){
                spdlog::info((json{{"event", "IndexManager destructor called"}}).dump());
            }
            IndexManager(const IndexManager&)=delete;
            IndexManager& operator=(const IndexManager&)=delete;
            static IndexManager& get_instance(){
                static IndexManager instance;
                return instance;

            }
            //load_index 加载指定路径下的index文件
            //@params std::vector< std::string > paths 查找index文件的路径列表
            void load_index(std::vector< std::string > paths){
                spdlog::info((json{{"event", "load_index start"}}).dump());
                for (auto ps : paths){
                    for(auto& p: fs::directory_iterator(ps)){
                        auto ppath = p.path();
                        auto s = p.symlink_status();
                        auto name = ppath.filename().string();
                        if (fs::is_regular_file(s) && faiss_index_manager::endsWith(name,".index")) {
                            auto indexname = ppath.filename().stem().string();
                            json evt1;
                            evt1["event"]="find faiss index";
                            evt1["indexname"]=indexname;
                            evt1["path"]=ppath;
                            spdlog::info(evt1.dump());
                            (this->m)[indexname]= std::unique_ptr<Index>(read_index(ppath.string().c_str()));
                            json evt2;
                            evt2["event"]="load faiss index";
                            evt2["path"]=ppath;
                            spdlog::info(evt2.dump());
                        }
                    }         
                }
                spdlog::info((json{{"event", "load_index end"}}).dump());
            }
            //search 查找指定index中与请求最相近的前k个向量id
            //@params std::string indexname index名
            //@params std::vector<std::vector<float>> query 查找的向量列表
            //@params int k 查找最相似的k个向量id
            //@return std::vector<std::vector<long long>> 请求向量列表对应的排名id列表,第一层与query对应,第二层顺序为排名
            //@exception std::out_of_range 未找到 indexname则会抛出超界错误
            //@exception faiss_index_manager::DNotEqual query的d和index的d不一致则会抛出DNotEqual
            std::vector<std::vector<long long>> search(std::string indexname, std::vector<std::vector<float>> query ,int k){
                auto nq = query.size();
                auto d = query.front().size();
                idx_t* I = new idx_t[k * nq];
                float* D = new float[k * nq];
                float* xq = new float[d * nq];
                for (int i = 0; i < nq; i++) {
                    for (int j = 0; j < d; j++){
                        xq[d * i + j] = query[i][j];
                    }     
                }
                spdlog::info((json{{"event", "search prepare ok"}}).dump());
                (this->m).at(indexname);
                if ((this->m)[indexname]->d != d){
                    throw faiss_index_manager::DNotEqualException();
                }
                (this->m)[indexname]->search(nq, xq, k, D, I);
                spdlog::info((json{{"event", "search ok"}}).dump());
                std::vector<std::vector<long long>> result;
                printf("I=\n");
                for (int i = 0; i < nq; i++) {
                    std::vector<long long> v;
                    for (int j = 0; j < k; j++){
                        printf("%5zd ", I[i * k + j]);
                        v.push_back(I[i * k + j]);
                    }
                    printf("\n ");
                    result.push_back(v);
                }
                delete [] I;
                delete [] D;
                delete [] xq;
                return result;
            }
        private:
            std::map<std::string,std::unique_ptr<Index>> m;
            IndexManager(){
                spdlog::info((json{{"event", "IndexManager constructor called!"}}).dump());
            }
    };
}

using faiss_index_manager::IndexManager;
using faiss_index_manager::DNotEqualException;

namespace faiss_grpc_serv{
  // Logic and data behind the server's behavior.
    class FaissRpcServiceImpl final : public FaissRpc::Service {
    //search 单次请求结果
        Status search(ServerContext* context, const Query* request, Response* reply) override {
            IndexManager& instance = IndexManager::get_instance();
            // 校验参数
            if (! request->has_query_vec()){
                auto sta = reply->mutable_status();
                sta->set_status(ResponseStatus_Stat_FAILED);
                sta->set_message("must have query_vec");
                return Status::OK;
            }
            auto indexname = request->target_index();
            auto k = request->k();
            auto query_vec = request->query_vec();
            if (indexname.empty()){
                auto sta = reply->mutable_status();
                sta->set_status(ResponseStatus_Stat_FAILED);
                sta->set_message("target_index must not empty");
                return Status::OK;
            }
            if (k <=0){
                auto sta = reply->mutable_status();
                sta->set_status(ResponseStatus_Stat_FAILED);
                sta->set_message("k must larger than 0");
                return Status::OK;
            }

            //构造符合格式的请求
            std::vector<std::vector<float>> query;
            auto elements = query_vec.elements();
            std::vector<float> vec;
            for (auto e : elements){
                vec.push_back(e);
            }
            query.push_back(vec);
            // 搜索
            try{
                auto res = instance.search(indexname,query,k);
                auto sta = reply->mutable_status();
                sta->set_status(ResponseStatus_Stat_SUCCEED);
                sta->set_message("ok");
                auto result = reply->mutable_result();
                for (auto r :res.front()){
                    result->add_rank(r);
                }
                return Status::OK;
            } catch(std::out_of_range & e){
                auto sta = reply->mutable_status();
                sta->set_status(ResponseStatus_Stat_FAILED);
                sta->set_message("can not find target_index");
                return Status::OK;
            } catch(DNotEqualException & e){
                auto sta = reply->mutable_status();
                sta->set_status(ResponseStatus_Stat_FAILED);
                sta->set_message("target_index's d is not equal to query's d");
                return Status::OK;
            } catch(std::exception& e){
                auto sta = reply->mutable_status();
                sta->set_status(ResponseStatus_Stat_FAILED);
                sta->set_message(e.what());
                return Status::OK;
            }
        }
        //batch_search 请求结果批处理
        Status batch_search(ServerContext* context, ServerReaderWriter< BatchResponse,BatchQuery >* stream) override {
            IndexManager& instance = IndexManager::get_instance();
            BatchQuery request;
            while (stream->Read(&request)) {
                BatchResponse reply;
                // 校验参数
                auto batch_id = request.batch_id();
                auto indexname = request.target_index();
                auto k = request.k();
                auto query_vecs = request.query_vecs();
                if (batch_id.empty()){
                    auto sta = reply.mutable_status();
                    sta->set_status(ResponseStatus_Stat_FAILED);
                    sta->set_message("batch_id must not empty");
                    stream->Write(reply);
                    continue;
                }
                reply.set_batch_id(batch_id);
                if (indexname.empty()){
                    auto sta = reply.mutable_status();
                    sta->set_status(ResponseStatus_Stat_FAILED);
                    sta->set_message("target_index must not empty");
                    stream->Write(reply);
                    continue;
                }
                if (k <=0){
                    auto sta = reply.mutable_status();
                    sta->set_status(ResponseStatus_Stat_FAILED);
                    sta->set_message("k must larger than 0");
                    stream->Write(reply);
                    continue;
                }
                if (query_vecs.empty()){
                    auto sta = reply.mutable_status();
                    sta->set_status(ResponseStatus_Stat_FAILED);
                    sta->set_message("query_vecs must not empty");
                    stream->Write(reply);
                    continue;
                }
                json evt;
                evt["event"]="get query";
                evt["method"]="batch_search";
                evt["batch_id"]=batch_id;
                spdlog::info(evt.dump());
                // 构造请求
                std::vector<std::vector<float>> query;
                for (auto query_vec : query_vecs){
                    auto elements = query_vec.elements();
                    std::vector<float> vec;
                    for (auto e : elements){
                        vec.push_back(e);
                    }
                    query.push_back(vec);
                }
                // 搜索
                try{
                    auto res = instance.search(indexname,query,k);
                    auto sta = reply.mutable_status();
                    sta->set_status(ResponseStatus_Stat_SUCCEED);
                    sta->set_message("ok");

                    for (auto r :res){
                        auto result = reply.add_result();
                        for (auto e: r){
                            result->add_rank(e);
                        }
                    }
                    stream->Write(reply);
                    evt["event"]="send response";
                    spdlog::info(evt.dump());
                    continue;
                } catch(std::out_of_range & e){
                    auto sta = reply.mutable_status();
                    sta->set_status(ResponseStatus_Stat_FAILED);
                    sta->set_message("can not find target_index");
                    stream->Write(reply);
                    continue;
                } catch(DNotEqualException & e){
                    auto sta = reply.mutable_status();
                    sta->set_status(ResponseStatus_Stat_FAILED);
                    sta->set_message("target_index's d is not equal to query's d");
                    stream->Write(reply);
                    continue;
                } catch(std::exception& e){
                    auto sta = reply.mutable_status();
                    sta->set_status(ResponseStatus_Stat_FAILED);
                    sta->set_message(e.what());
                    stream->Write(reply);
                    continue;
                }
            }
            return Status::OK;
        }
    };
 
  //class FaissGrpcConf impl

  //FaissGrpcConf 构造函数,可以使用 nlohmann::json 来构造实例
  FaissGrpcConf::FaissGrpcConf(const json& j){
    try{
        j.at("app_version").get_to(this->app_version);
    }catch(std::exception &e){
        this->app_version = "0.0.0";
    }
    try{
        j.at("app_name").get_to(this->app_name);
    }catch(std::exception &e){
        this->app_name = "faiss_grpc";
    }
    try{
        j.at("address").get_to(this->address);
    }catch(std::exception &e){
        this->address = "address";
    }
    try{
        j.at("log_level").get_to(this->log_level);
    }catch(std::exception &e){
        this->log_level = "debug";
    }
    try{
        j.at("server_cert_path").get_to(this->server_cert_path);
    }catch(std::exception &e){
        this->server_cert_path = "";
    }
    try{
        j.at("server_key_path").get_to(this->server_key_path);
    }catch(std::exception &e){
        this->server_key_path = "";
    }
    try{
        j.at("index_dirs").get_to(this->index_dirs);
    }catch(std::exception &e){
        this->index_dirs = {"./indexes"};
    }
    try{
        j.at("max_rec_msg_size").get_to(this->max_rec_msg_size);
    }catch(std::exception &e){
        this->max_rec_msg_size = 1024*1024;
    }
    try{
        j.at("max_send_msg_size").get_to(this->max_send_msg_size);
    }catch(std::exception &e){
        this->max_send_msg_size = 1024*1024;
    }
    try{
        j.at("max_concurrent_streams").get_to(this->max_concurrent_streams);
    }catch(std::exception &e){
        this->max_concurrent_streams = 100;
    }
    try{
        j.at("max_connection_idle").get_to(this->max_connection_idle);
    }catch(std::exception &e){
        this->max_connection_idle = 0;
    }
    try{
        j.at("max_connection_age").get_to(this->max_connection_age);
    }catch(std::exception &e){
        this->max_connection_age = 0;
    }
    try{
        j.at("max_connection_age_grace").get_to(this->max_connection_age_grace);
    }catch(std::exception &e){
        this->max_connection_age_grace = 0;
    }
    try{
        j.at("keepalive_time").get_to(this->keepalive_time);
    }catch(std::exception &e){
        this->keepalive_time = 0;
    }
    try{
        j.at("keepalive_timeout").get_to(this->keepalive_timeout);
    }catch(std::exception &e){
        this->keepalive_timeout = 0;
    }
    try{
        j.at("keepalive_enforement_permit_without_stream").get_to(this->keepalive_enforement_permit_without_stream);
    }catch(std::exception &e){
        this->keepalive_enforement_permit_without_stream = true;
    }
    try{
        j.at("compression").get_to(this->compression);
    }catch(std::exception &e){
        this->compression = "";
    }
  }
  //to_json 序列化对象为json
  json FaissGrpcConf::to_json() const {
    auto j = json{
      {"app_version", this->app_version}, 
      {"app_name", this->app_name}, 
      {"address", this->address},
      {"log_level", this->log_level},
      {"server_cert_path", this->server_cert_path},
      {"server_key_path", this->server_key_path},
      {"index_dirs", this->index_dirs},
      {"max_rec_msg_size", this->max_rec_msg_size},
      {"max_send_msg_size", this->max_send_msg_size},
      {"max_concurrent_streams", this->max_concurrent_streams},
      {"max_connection_idle", this->max_connection_idle},
      {"max_connection_age", this->max_connection_age},
      {"max_connection_age_grace", this->max_connection_age_grace},
      {"keepalive_time", this->keepalive_time},
      {"keepalive_timeout", this->keepalive_timeout},
      {"keepalive_enforement_permit_without_stream", this->keepalive_enforement_permit_without_stream},
      {"compression", this->compression}
    };
    return j;
  }
  //RunServer 根据配置启动服务
  void FaissGrpcConf::RunServer() {

    std::string jsonlastlogpattern = "{\"time\": \"%Y-%m-%dT%H:%M:%S.%f%z\", \"app_name\": \"";
    jsonlastlogpattern += this->app_name;
    jsonlastlogpattern += "\",\"app_version\": \"";
    jsonlastlogpattern += this->app_version;
    jsonlastlogpattern +="\", \"level\": \"%^%l%$\", \"process\": %P, \"thread\": %t, \"message\": %v}";
    spdlog::set_pattern(jsonlastlogpattern);
    std::vector<std::string> level_range = {"trace","debug","info","warn","err","critical"};
    if ( std::find(level_range.begin(), level_range.end(), this->log_level) != level_range.end()){
      auto level = spdlog::level::from_str(this->log_level);
      spdlog::set_level(level);
    } else {
      json evt;
      evt["event"]="unknown log level";
      evt["log_level"]=this->log_level;
      evt["after"]="use debug as log level";
      spdlog::info(evt.dump());
      spdlog::set_level(spdlog::level::debug);
    }
    
    spdlog::info("{\"config\":"+ this->to_json().dump() + "}");

    std::string server_address(this->address);
    FaissRpcServiceImpl service;
    IndexManager& instance = IndexManager::get_instance();
    instance.load_index(this->index_dirs);

    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
    // Finally assemble the server.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    spdlog::info((json{
                    {"event", "Server start"}, 
                    {"address", this->address}
                }).dump());
    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
  }

  void to_json(json& j, const faiss_grpc_serv::FaissGrpcConf& p) {
    j = p.to_json();
  }

  void from_json(const json& j, faiss_grpc_serv::FaissGrpcConf& p) {
    p = faiss_grpc_serv::FaissGrpcConf(j);
  }
}


