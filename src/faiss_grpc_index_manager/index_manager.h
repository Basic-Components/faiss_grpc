#pragma once

#include <string>
#include <map>
#include <mutex>
#include <thread>

#include <libfswatch/c++/monitor_factory.hpp>
#include <libfswatch/c++/monitor.hpp>
#include <libfswatch/c++/event.hpp>
#include <libfswatch/c/cevent.h>

#include "index_info.h"

using fsw::monitor;

namespace faiss_grpc_index_manager{

    class IndexManager{
        private:
        IndexManager(){};
        static IndexManager *p;
        std::string config_path="./index_manager_config.json";
        std::string config_hash="";
        std::mutex lock;
        map<std::string, map<int, IndexInfo>> im;//索引 auto info =  im[index_name][version]
        monitor *index_monitor; // 监听索引
        std::thread index_monitor_t; //监听索引监听器所在的线程

        public:
        static IndexManager * getInstance() {
            return p;
        };

        void set_config_path(std::string config_path);
        void load_config();
        map<int, IndexInfo> load_index(std::string index_name, std::string path);
        void watch_config();
        void watch_index(std::string index_name, std::string path);
        std::vector<TopK> search(std::string i,BatchVec& query, int k);
        std::vector<std::string> get_index_list(std::string ndex_name);
        IndexDetail get_index_metadata(std::string ndex_name);
        void load_index(std::vector<std::string>);
    };
}