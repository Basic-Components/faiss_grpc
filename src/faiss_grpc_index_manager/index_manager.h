#pragma once

#include <map>
#include <vector>
#include <string>
#include <mutex>
#include <thread>
#include <memory>

#include <libfswatch/c++/event.hpp>
#include <libfswatch/c/cevent.h>

#include "../faiss_grpc_pb/faiss_grpc.grpc.pb.h"
#include "index_info.h"

//pb的依赖
using faiss_grpc::BatchVec;
using faiss_grpc::TopK;
using faiss_grpc::IndexDetail;

using fsw::event;

namespace faiss_grpc_index_manager{
    void fschangecallback(const std::vector<event> &evts, void *ctx);
    class IndexManager{
        private:
        IndexManager(){};
        static IndexManager *p;
        std::string index_dir="./index";
        std::mutex lock;
        std::map<std::string, IndexInfo> name_map;//索引
        // 单体
        void update_index(std::string index_name,std::string index_path);
        void add_index(std::string index_name,std::string index_path);
        void delete_index(std::string index_name);

        public:
        static IndexManager * getInstance() {
            return p;
        };

        // 总体
        void load_index_dir();
        void reload_index(std::string);
        // void watch_index_dir();
        // void stop_watch_index_dir();

        std::vector<TopK*> search(std::string, BatchVec& , int );
        std::vector<std::string> get_index_list();
        IndexDetail* get_index_metadata(std::string);
        
    };
}