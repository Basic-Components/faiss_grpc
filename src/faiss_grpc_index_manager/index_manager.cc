#include <filesystem>
#include <set>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <mutex>
#include <exception>
#include <regex>

#include <nlohmann/json-schema.hpp>
#include <spdloghelper.h>

#include <libfswatch/c++/monitor_factory.hpp>
#include <libfswatch/c++/monitor.hpp>
#include <libfswatch/c++/event.hpp>
#include <libfswatch/c/cevent.h>

#include <faiss/MetricType.h>

#include "../faiss_grpc_pb/faiss_grpc.grpc.pb.h"

#include "exception.h"
#include "utils.h"
#include "index_manager.h"

namespace fs = std::filesystem;
// log依赖
using Logger = spdloghelper::LogHelper;

// fsw
using fsw::event;
using fsw::monitor;
using fsw::monitor_factory;

//pb的依赖
using faiss_grpc::BatchVec;
using faiss_grpc::TopK;
using faiss_grpc::IndexDetail;



namespace faiss_grpc_index_manager{

    void fschangecallback(const std::vector<event> &evts, void *ctx){
        auto index_manager = IndexManager::get_instance();
        auto logger = Logger::getInstance();
        for (auto evt : evts){
            auto flags = evt.get_flags();
            if (flags.empty()){
                logger->info("get event empty")
                continue;
            }
            for (auto& flag : flags){
                auto ele_path_str = evt.get_path();
                fs::path ele_path = ele_path_str;
                switch (flag) {
                    case Created :
                    case Updated :
                    {
                        logger->info("get effective fsevent",{ {"fsevent",event::get_event_flag_name(flag)},{"path",ele_path_str} });
                        index_manager->load_index_dir()
                    }
                    break;
                    default:
                        logger->info("get uneffective fsevent",{ {"fsevent",event::get_event_flag_name(flag)},{"path",ele_path_str} });
                }
            }
        }
    }

    void IndexManager::update_index(std::string index_name,std::string index_path){
        auto logger = Logger::getInstance();
        std::lock_guard<std::mutex> guard(this->lock);
        try{
            this->name_map[index_name].index_path = index_path;
            this->name_map[index_name].load_index();
            logger->info("update index done",{ {"index_name",index_name},{"path",index_path} });
        }catch (std::exception &e) {
            logger->error("update index get error",{{"index_name",index_name},{"error",e.what()}});
        }
    }
    void IndexManager::add_index(std::string index_name,std::string index_path){
        auto logger = Logger::getInstance();
        std::lock_guard<std::mutex> guard(this->lock);
        try{
            this->name_map[index_name]= IndexInfo(index_name,index_path);
            this->name_map[index_name].load_index();
            logger->info("add index done",{ {"index_name",index_name},{"path",index_path} });
        }catch (std::exception &e) {
            logger->error("add index get error",{{"index_name",index_name},{"error",e.what()}});
        }
    }
    void IndexManager::delete_index(std::string index_name){
        auto logger = Logger::getInstance();
        std::lock_guard<std::mutex> guard(this->lock);
        this->name_map.erase(index_name);
        logger->info("delete index done",{ {"index_name",index_name} });
    }

    void IndexManager::load_index_dir(){
        fs::path index_dir_path =this->index_dir;
        if (!fs::is_directory(index_dir_path)){
            throw IndexDirNotExistException();
        }
        for(auto& ele: fs::directory_iterator(index_dir_path)){
            auto ele_path = ele.path();
            auto ele_path_str = ele_path.string();
            std::set<std::string> all_index_names;
            if (fs::is_regular_file(ele_path) && endsWith(ele_path.filename().string(),".index")){
                auto index_name = std::regex_replace(ele_path.filename().string() ".index", "");
                all_index_names.insert(index_name)// 插入用于不删除的set
                if (this->name_map.contains(index_name)){
                    this->update_index(index_name,ele_path_str);
                }else{
                    ithis->add_index(index_name,ele_path_str);
                }
            }
            std::set<std::string> need_to_del;
            for (auto& e : this->name_map.items()){
                auto index_name = e.key();
                if (!all_index_names.contains(index_name)){
                    need_to_del.insert(index_name);
                }
            }
            for (auto& ne : need_to_del){
                this->delete_index(ne);
            }
        }
    }

    void IndexManager::reload_index(std::string index_name){
        std::lock_guard<std::mutex> guard(this->lock);
        if (!this->name_map.contains(index_name)){
            throw IndexNameNotExistException(index_name);
        }
        this->name_map[index_name].load_index();
    }

    std::vector<TopK*> IndexManager::search(std::string index_name,BatchVec& query, int k){
        std::lock_guard<std::mutex> guard(this->lock);
        if (!this->name_map.contains(index_name)){
            throw IndexNameNotExistException(index_name);
        }
        return this->name_map[index_name].search_top_k(query,k);
    }
    std::vector<std::string> IndexManager::get_index_list(){
        std::lock_guard<std::mutex> guard(this->lock);
        std::vector<std::string> result;
        for (auto& e : this->name_map.items()){
            result.push_back(e.key());
        }
        return result;
    }
    IndexDetail* IndexManager::get_index_metadata(std::string index_name){
        std::lock_guard<std::mutex> guard(this->lock);
        if (!this->name_map.contains(index_name)){
            throw IndexNameNotExistException(index_name);
        }
        IndexDetail* result;
        result->set_index_name(this->name_map[index_name].index_name);
        result->set_index_path(this->name_map[index_name].index_path);
        longlong last_load_timestamp = this->name_map[index_name].last_update;
        result->set_last_load_timestamp(last_load_timestamp);
        result->set_d(this->name_map[index_name].index->d);
        result->set_ntotal(this->name_map[index_name].index->ntotal);
        result->set_is_trained(this->name_map[index_name].index->is_trained);
        result->set_metric_arg(this->name_map[index_name].index->metric_arg);
        result->set_metric_type(this->name_map[index_name].index->metric_type);
        return result;
    }
}