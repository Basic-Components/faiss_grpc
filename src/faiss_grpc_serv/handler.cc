#include <cmath>
#include "spdloghelper.h"
#include <grpcpp/grpcpp.h>
#include "../faiss_grpc_pb/faiss_grpc.grpc.pb.h"
#include "../faiss_grpc_index_manager/index_manager.h"
#include "faiss_grpc_serv.h"

using grpc::ServerContext;
using grpc::Status;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;

using faiss_grpc::FAISS_GRPC;
using faiss_grpc::Response;
using faiss_grpc::ListResponse;
using faiss_grpc::MetadataResponse;
using faiss_grpc::ReloadResponse;

using faiss_grpc::ResponseStatus;
using faiss_grpc::ResponseStatus_Stat;
using faiss_grpc::ResponseStatus_Stat_FAILED;
using faiss_grpc::ResponseStatus_Stat_SUCCEED;


using Logger = spdloghelper::LogHelper;
using IndexManager = faiss_grpc_index_manager::IndexManager;

// 实现接口
namespace faiss_grpc_serv {
    Status FAISS_GRPC_RPCServiceImpl::get_index_list(ServerContext* context, const ListQuery* request, ListResponse* response){
        // 校验参数
        IndexManager* index_manager = IndexManager::getInstance();
        auto res = index_manager->get_index_list();
        auto sta = response->mutable_status();
        sta->set_status(ResponseStatus_Stat_SUCCEED);
        sta->set_message("ok");
        // auto result = response->mutable_index_names();
        for (auto r : res){
            response->add_index_names(r);
        }
        return Status::OK;

    }
    Status FAISS_GRPC_RPCServiceImpl::get_index_metadata(ServerContext* context, const MetadataQuery* request, MetadataResponse* response){
        // 校验参数
        auto index_name = request->index_name();
        if (index_name.empty()){
            auto sta = response->mutable_status();
            sta->set_status(ResponseStatus_Stat_FAILED);
            sta->set_message("index_name must not empty");
            return Status::OK;
        }
        IndexManager* index_manager = IndexManager::getInstance();
        auto res = index_manager->get_index_metadata(index_name);
        auto sta = response->mutable_status();
        sta->set_status(ResponseStatus_Stat_SUCCEED);
        sta->set_message("ok");
        response->set_allocated_detail(res);
        return Status::OK;

    }
    Status FAISS_GRPC_RPCServiceImpl::reload_index(ServerContext* context, const ReloadQuery* request, ReloadResponse* response){
        IndexManager* index_manager = IndexManager::getInstance();
        if (!request->index_names_size() < 1){
            index_manager->load_index_dir();
            auto sta = response->mutable_status();
            sta->set_status(ResponseStatus_Stat_SUCCEED);
            sta->set_message("ok");
            return Status::OK;
        }else{
            for (auto index_name : request->index_names()){
                index_manager->reload_index(index_name);
            }
            auto sta = response->mutable_status();
            sta->set_status(ResponseStatus_Stat_SUCCEED);
            sta->set_message("ok");
            return Status::OK;
        }
    }

    Status FAISS_GRPC_RPCServiceImpl::search(ServerContext* context, const Query* request, Response* response){
        // 校验参数
        if (!request->has_query_vecs()){
            auto sta = response->mutable_status();
            sta->set_status(ResponseStatus_Stat_FAILED);
            sta->set_message("must have query_vec");
            return Status::OK;
        }
        auto index_name = request->index_name();
        auto k = request->k();
        auto query_vecs = request->query_vecs();
        if (index_name.empty()){
            auto sta = response->mutable_status();
            sta->set_status(ResponseStatus_Stat_FAILED);
            sta->set_message("index_name must not empty");
            return Status::OK;
        }
        if (k <= 0){
            auto sta = response->mutable_status();
            sta->set_status(ResponseStatus_Stat_FAILED);
            sta->set_message("k must larger than 0");
            return Status::OK;
        }
        //业务部分
        IndexManager* index_manager = IndexManager::getInstance();
        auto res = index_manager->search(index_name,query_vecs,k);
        auto sta = response->mutable_status();
        sta->set_status(ResponseStatus_Stat_SUCCEED);
        sta->set_message("ok");
        // auto result = response->mutable_result();
        for (auto r : res){
            auto t = response->add_result();
            for (auto i : r){
                t->add_rank(i);
            }
        }
        return Status::OK;
    }

    Status FAISS_GRPC_RPCServiceImpl::batch_search(ServerContext* context, ServerReaderWriter< Response, Query>* stream){
        IndexManager* index_manager = IndexManager::getInstance();
        Query request;
        while (stream->Read(&request)) {
            Response response;
            auto id = request.id();
            if (id.empty()){
                auto sta = response.mutable_status();
                sta->set_status(ResponseStatus_Stat_FAILED);
                sta->set_message("id must not empty");
                return Status::OK;
            }
            response.set_id(id);
            if (!request.has_query_vecs()){
                auto sta = response.mutable_status();
                sta->set_status(ResponseStatus_Stat_FAILED);
                sta->set_message("must have query_vecs");
                return Status::OK;
            }
            
            auto index_name = request.index_name();
            auto k = request.k();
            auto query_vecs = request.query_vecs();
            
            if (index_name.empty()){
                auto sta = response.mutable_status();
                sta->set_status(ResponseStatus_Stat_FAILED);
                sta->set_message("index_name must not empty");
                return Status::OK;
            }
            if (k <= 0){
                auto sta = response.mutable_status();
                sta->set_status(ResponseStatus_Stat_FAILED);
                sta->set_message("k must larger than 0");
                return Status::OK;
            }
            
            //业务部分
            auto res = index_manager->search(index_name,query_vecs,k);
            auto sta = response.mutable_status();
            sta->set_status(ResponseStatus_Stat_SUCCEED);
            sta->set_message("ok");
            // auto result = response.mutable_result();
            for (auto r : res){
                auto t = response.add_result();
                for (auto i : r){
                    t->add_rank(i);
                }
            }
            stream->Write(response);
        }
        return Status::OK;
    }
}