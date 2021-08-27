#pragma once

#include <string>
#include <ctime>
#include <memory>
#include <faiss/Index.h>
#include "../faiss_grpc_pb/faiss_grpc.grpc.pb.h"

using faiss_grpc::BatchVec;
using faiss_grpc::TopK;

using faiss::Index;

namespace faiss_grpc_index_manager{
    // 定义类
    class IndexInfo {
        public:
        std::string index_name="";
        std::string index_path="";
        std::string file_hash="";
        time_t last_update;
        std::unique_ptr<Index> index; 
        bool loaded=false;
        IndexInfo(){};
        IndexInfo(std::string index_name,int index_version, std::string index_path): index_name(index_name), index_path(index_path),loaded(false) {};
        
        void load_index();
        std::vector<TopK*> search_top_k(BatchVec& query, int k);
    };
}