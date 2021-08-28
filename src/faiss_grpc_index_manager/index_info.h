#pragma once

#include <string>
#include <ctime>
#include <memory>
#include <faiss/Index.h>
#include "../faiss_grpc_pb/faiss_grpc.grpc.pb.h"

using faiss_grpc::BatchVec;

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
        IndexInfo(std::string index_name, std::string index_path): index_name(index_name), index_path(index_path),loaded(false) {};
        
        void load_index();
        std::vector<std::vector<long long>> search_top_k(BatchVec& query, int k);
    };
}