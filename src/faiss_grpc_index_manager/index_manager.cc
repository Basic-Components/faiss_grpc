#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <exception>
#include <regex>
#include <nlohmann/json-schema.hpp>
#include <spdloghelper.h>


#include "exception.h"
#include "utils.h"
#include "index_manager.h"

namespace fs = std::filesystem;
// log依赖
using Logger = spdloghelper::LogHelper;
// json依赖
using nlohmann::json;
using nlohmann::json_schema::json_validator;


const std::regex index_regex("^v(0|[1-9][0-9]*).index$");
void check_config_schema(json& config ){
    Logger* logger = Logger::getInstance();
    json schema = R"({
    "$schema": "http://json-schema.org/draft-07/schema#",
    "type": "array",
    "items":{
        "type": "object",
        "properties": {
            "index_name":{
                "type": "string",
                "description": "index的名字"
            }
            "index_path":{
                "type": "string",
                "description": "index所在的文件夹"
            }
        },
        "required": ["index_name", "index_path"],
        "additionalProperties":false
    },
    "minItems": 1
    })"_json;
    json_validator validator; // create validator
    try {
        validator.set_root_schema(schema); // insert root-schema
    } catch (const std::exception &err) {
        logger->error("Validation of schema failed", {{"error", err.what()}});
        throw err;
    }
    try {
        validator.validate(config); // validate the document - uses the default throwing error-handler
    } catch (const std::exception &err) {
        logger->error("Config Validation failed", {{"error", err.what()},{"config", config.dump()},{"schema",schema.dump()}});
        throw err;
    }
}


namespace faiss_grpc_index_manager{

    void IndexManager::set_config_path(std::string config_path){
        this->config_path=config_path;
    }

    map<int, IndexInfo> IndexManager::load_index(std::string index_name, std::string path){
        map<int, IndexInfo> result;
        fs::path p = path;
        
        // 路径为文件,则作为version=v0的索引
        if(fs::is_regular_file(p) && endsWith(p.filename().string(), ".index") ){
            result[0] = IndexInfo(index_name,0, path);
        }else if(fs::is_directory(p)){
            for (auto &ele : fs::directory_iterator(p)){
                auto subp = ele.path();
                if (fs::is_regular_file(subp) && std::regex_match(subp.filename().string(), index_regex) ){
                    subp.filename().string().
                }
            }  
        }else{

        }
    }
    void IndexManager::watch_index(std::string path){

    }
    void IndexManager::load_config(){
        Logger* logger = Logger::getInstance();
        std::ifstream file(this->config_path);
        if (file.is_open()){
            std::stringstream buffer;
            buffer << file.rdbuf();
            std::string content_str(buffer.str());
            file.close();
            // 计算读入配置的hash
            auto content_hash = get_content_hash(content_str);
            if (this->config_hash != "" && content_hash == this->config_hash){
                logger->info("config file not change")
                return
            }
            json result = json::parse(content_str);
            if (result.empty()){
                throw ConfigFileEmptyException();
            }
 
            try {
                check_config_schema(result);
            }catch (std::exception &err){
                throw CheckConfigSchemaException();
            }
            this->config_hash = content_hash
            // //解析配置
            // for (auto& i : result){
                
            // }
            // this->im[]
            
        }else{
            throw ConfigFileNotExistException()
        }
    }
    void IndexManager::watch_config();
}