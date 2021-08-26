#include <memory>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <exception>

#include <spdloghelper.h>
#include <faiss/Index.h>
#include <faiss/index_io.h>
#include "../faiss_grpc_pb/faiss_grpc.grpc.pb.h"


#include "exception.h"
#include "utils.h"
#include "index_info.h"


// log依赖
using Logger = spdloghelper::LogHelper;

// faiss的依赖
using faiss::Index;
using faiss::read_index;
using idx_t = faiss::Index::idx_t;// 64-bit int

//pb的依赖
using faiss_grpc::BatchVec;
using faiss_grpc::TopK;


namespace faiss_grpc_index_manager{

    void IndexInfo::load_index(){
        Logger* logger = Logger::getInstance();
        logger->info("loading index",{{"index_name", this->index_name },{ "index_path", this->index_path}});
        std::ifstream file(this->config_path);
        if (file.is_open()){
            std::stringstream buffer;
            buffer << file.rdbuf();
            std::string content_str(buffer.str());
            file.close();
            // 计算读入配置的hash
            auto content_hash = get_content_hash(content_str);
            if (this->file_hash != "" && content_hash == this->file_hash){
                logger->info("config file not change");
                return;
            }
            try {
                this->index = std::unique_ptr<Index>(read_index(this->index_path.c_str()));
            }catch (std::exception &e){
                logger->error("load index get error",{{"index_name", this->index_name },{ "index_path", this->index_path},{"error",e.what()}});
            }
            if (this->index == nullptr){
                logger->error("load index get empty",{{"index_name", this->index_name },{ "index_path", this->index_path}});
            }else{
                this->loaded=true;
                this->last_update=std::time(nullptr);
                this->file_hash = content_hash;
                logger->info("load index ok",{{"index_name", this->index_name },{ "index_path", this->index_path}});
            }
        }else{
            throw IndexFileNotExistException();
        }
        

        
    }
    std::vector<TopK> IndexInfo::search_top_k(BatchVec& query, int k){
        if (!this->loaded){
            throw IndexNotLoadException()
        }
        auto nq = query.batch_size();// 长度
        if (nq < 1){
            throw QueryBatchLessThan1Exception()
        }
        auto d = query.batch(0).elements_size();//维度
        Logger* logger = Logger::getInstance();
        idx_t *I = new idx_t[k * nq];
        float *D = new float[k * nq];
        float *xq = new float[d * nq];

        for (int i = 0; i < nq; i++){
            for (int j = 0; j < d; j++){
                xq[d * i + j] = query.batch(i).elements(j);
            }
        }
        logger->debug("search prepare ok");

        this->index->search(nq, xq, k, D, I);
        logger->debug("search ok");

        std::vector<TopK> result;
        for (int i = 0; i < nq; i++){
            TopK v;
            for (int j = 0; j < k; j++){
                v.add_rank(I[i * k + j]);
            }
            result.push_back(v);
        }
        delete[] I;
        delete[] D;
        delete[] xq;
        return result;
    }
}