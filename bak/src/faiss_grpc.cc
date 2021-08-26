#include <cstdlib>
#include <iostream>
#include <cstddef>
#include <string>
#include <fstream>
#include <exception>
#include "nlohmann/json.hpp"
#include <argparse/argparse.hpp>
#include "faiss_rpc_serv.h"

using faiss_rpc_serv::FAISS_RPCConf;
using json = nlohmann::json;

std::vector<std::string>split(const std::string& str, const std::string& delims = ","){
    std::vector<std::string> output;
    auto first = std::cbegin(str);

    while (first != std::cend(str))
    {
        const auto second = std::find_first_of(first, std::cend(str), 
                  std::cbegin(delims), std::cend(delims));

        if (first != second)
            output.emplace_back(first, second);

        if (second == std::cend(str))
            break;

        first = std::next(second);
    }
    return output;
}
void get_conf_from_env(FAISS_RPCConf* conf){
    if (const char* env_app_name = std::getenv("FAISS_GRPC_APP_NAME")) {
        conf->app_name = env_app_name;
    }
    if (const char* env_app_version = std::getenv("FAISS_GRPC_APP_VERSION")) {
        conf->app_version = env_app_version;
    }
    if (const char* env_address = std::getenv("FAISS_GRPC_ADDRESS")) {
        conf->address = env_address;
    }
    if (const char* env_server_cert_path = std::getenv("FAISS_GRPC_SERVER_CERT_PATH")) {
        conf->server_cert_path = env_server_cert_path;
    }
    if (const char* env_server_key_path = std::getenv("FAISS_GRPC_SERVER_KEY_PATH")) {
        conf->server_key_path = env_server_key_path;
    }
    if (const char* env_root_cert_path = std::getenv("FAISS_GRPC_ROOT_CERT_PATH")) {
        conf->root_cert_path = env_root_cert_path;
    }
    if (const char* env_client_certificate_request = std::getenv("FAISS_GRPC_CLIENT_CERT_REQUEST")) {
      std::vector<std::string> choices = {"not_request","not_verify","request_and_verify","enforce_request","enforce_request_and_verify"};
      if (std::find(choices.begin(), choices.end(), env_client_certificate_request) != choices.end()) {
          conf->client_certificate_request = env_client_certificate_request;
      }
    }

    if (const char* env_max_rec_msg_size = std::getenv("FAISS_GRPC_MAX_REC_MSG_SIZE")) {
        try {
            auto max_rec_msg_size = std::stoi(env_max_rec_msg_size);
            conf->max_rec_msg_size = max_rec_msg_size;
        } catch (std::exception &err){
            std::cout<< "get config max_rec_msg_size from env error: " << err.what() << std::endl;
        } 
    }
    if (const char* env_max_send_msg_size = std::getenv("FAISS_GRPC_MAX_SEND_MSG_SIZE")) {
        try {
            auto max_send_msg_size = std::stoi(env_max_send_msg_size);
            conf->max_send_msg_size = max_send_msg_size;
        } catch (std::exception &err){
            std::cout<< "get config max_send_msg_size from env error: " << err.what() << std::endl;
        } 
    }
    if (const char* env_max_concurrent_streams = std::getenv("FAISS_GRPC_MAX_CONCURRENT_STREAMS")) {
        try {
            auto max_concurrent_streams = std::stoi(env_max_concurrent_streams);
            conf->max_concurrent_streams = max_concurrent_streams;
        } catch (std::exception &err){
            std::cout<< "get config max_concurrent_streams from env error: " << err.what() << std::endl;
        } 
    }
    if (const char* env_max_connection_idle = std::getenv("FAISS_GRPC_MAX_CONNECTION_IDLE")) {
        try {
            auto max_connection_idle = std::stoi(env_max_connection_idle);
            conf->max_connection_idle = max_connection_idle;
        } catch (std::exception &err){
            std::cout<< "get config max_connection_idle from env error: " << err.what() << std::endl;
        } 
    }
    if (const char* env_max_connection_age = std::getenv("FAISS_GRPC_MAX_CONNECTION_AGE")) {
        try {
            auto max_connection_age = std::stoi(env_max_connection_age);
            conf->max_connection_age = max_connection_age;
        } catch (std::exception &err){
            std::cout<< "get config max_connection_age from env error: " << err.what() << std::endl;
        } 
    }
    if (const char* env_max_connection_age_grace = std::getenv("FAISS_GRPC_MAX_CONNECTION_AGE_GRACE")) {
        try {
            auto max_connection_age_grace = std::stoi(env_max_connection_age_grace);
            conf->max_connection_age_grace = max_connection_age_grace;
        } catch (std::exception &err){
            std::cout<< "get config max_connection_age_grace from env error: " << err.what() << std::endl;
        } 
    }
    if (const char* env_keepalive_time = std::getenv("FAISS_GRPC_KEEPALIVE_TIME")) {
        try {
            auto keepalive_time = std::stoi(env_keepalive_time);
            conf->keepalive_time = keepalive_time;
        } catch (std::exception &err){
            std::cout<< "get config keepalive_time from env error: " << err.what() << std::endl;
        } 
    }
    if (const char* env_keepalive_timeout = std::getenv("FAISS_GRPC_KEEPALIVE_TIMEOUT")) {
        try {
            auto keepalive_timeout = std::stoi(env_keepalive_timeout);
            conf->keepalive_timeout = keepalive_timeout;
        } catch (std::exception &err){
            std::cout<< "get config keepalive_timeout from env error: " << err.what() << std::endl;
        } 
    }
    if (const char* env_keepalive_enforement_permit_without_stream = std::getenv("FAISS_GRPC_KEEPALIVE_ENFORCEMENT_PERMIT_WITHOUT_STREAM")) {
        std::vector<std::string> choices = {"TRUE", "true", "True", "1"};
        if (std::find(choices.begin(), choices.end(), env_keepalive_enforement_permit_without_stream) != choices.end()) {
            conf->keepalive_enforement_permit_without_stream = true;
        }
    }

    if (const char* env_log_level = std::getenv("FAISS_GRPC_LOG_LEVEL")) {
        std::vector<std::string> choices = {"trace", "debug", "info", "warn", "err", "critical"};
        if (std::find(choices.begin(), choices.end(), env_log_level) != choices.end()) {
            conf->log_level = env_log_level;
        }
    }

    if (const char* env_compression = std::getenv("FAISS_GRPC_COMPRESSION")) {
        std::vector<std::string> choices = {"deflate", "gzip"};
        if (std::find(choices.begin(), choices.end(), env_compression) != choices.end()) {
            conf->compression = env_compression;
        }
    }
    if (const char* env_index_dirs = std::getenv("FAISS_GRPC_INDEX_DIRS")) {
       conf->index_dirs = split(env_index_dirs,",");
    }
    
}



void get_conf_from_cmd(FAISS_RPCConf* conf,int argc, char** argv){
    argparse::ArgumentParser program("faiss_rpc","0.0.0");
    program.add_argument("--app_name")
      .help("应用名,会在log中体现")
      .action([&](const std::string& value) { conf->app_name = value;return value; });
    program.add_argument("--app_version")
      .help("应用版本,会在log中体现")
      .action([&](const std::string& value) { conf->app_version = value;return value; });
    program.add_argument("-a", "--address")
      .help("服务启动地址")
      .action([&](const std::string& value) { conf->address = value;return value; });
    program.add_argument("--server_cert_path")
      .help("使用TLS时服务端的证书位置,必须和server_key_path同时启用")
      .action([&](const std::string& value) { conf->server_cert_path = value;return value; });
    program.add_argument("--server_key_path")
      .help("使用TLS时服务端证书的私钥位置,必须和server_cert_path同时启用")
      .action([&](const std::string& value) { conf->server_key_path = value;return value; }); 
    program.add_argument("--root_cert_path")
      .help("TLS根证书位置")
      .action([&](const std::string& value) { conf->root_cert_path = value;return value; });
    program.add_argument("--client_certificate_request")
      .help("设置向客户端请求证书的行为,未设置则不强制请求,只支持not_request,not_verify,request_and_verify,enforce_request,enforce_request_and_verify")
      .action([&](const std::string& value) { 
        std::vector<std::string> choices = {"not_request","not_verify","request_and_verify","enforce_request","enforce_request_and_verify"};
        if (std::find(choices.begin(), choices.end(), value) != choices.end()) {
          conf->client_certificate_request = value;
        }
        return value;
        });

    program.add_argument("--max_rec_msg_size")
      .help("允许接收的最大消息长度,单位byte")
      .action([&](const std::string& value) { 
        try {
          auto max_rec_msg_size = std::stoi(value);
          conf->max_rec_msg_size = max_rec_msg_size;
        } catch (std::exception &err){
          std::cout<< "get config max_rec_msg_size from cmd error: " << err.what() << std::endl;
        }
        return value; 
      });
    program.add_argument("--max_send_msg_size")
      .help("允许发送的最大消息长度,单位byte")
      .action([&](const std::string& value) { 
        try {
          auto max_send_msg_size = std::stoi(value);
          conf->max_send_msg_size = max_send_msg_size;
        } catch (std::exception &err){
          std::cout<< "get config max_send_msg_size from cmd error: " << err.what() << std::endl;
        }
        return value; 
      });
    program.add_argument("--max_concurrent_streams")
      .help("单连接最大并发量")
      .action([&](const std::string& value) { 
        try {
          auto max_concurrent_streams = std::stoi(value);
          conf->max_concurrent_streams = max_concurrent_streams;
        } catch (std::exception &err){
          std::cout<< "get config max_concurrent_streams from cmd error: " << err.what() << std::endl;
        }
        return value; 
      }); 
    program.add_argument("--max_connection_idle")
      .help("客户端连接的最大空闲时长,单位ms")
      .action([&](const std::string& value) { 
        try {
          auto max_connection_idle = std::stoi(value);
          conf->max_connection_idle = max_connection_idle;
        } catch (std::exception &err){
          std::cout<< "get config max_connection_idle from cmd error: " << err.what() << std::endl;
        }
        return value; 
      }); 
    program.add_argument("--max_connection_age")
      .help("如果连接存活超过设定值则发送goaway信号,单位ms")
      .action([&](const std::string& value) { 
        try {
          auto max_connection_age = std::stoi(value);
          conf->max_connection_age = max_connection_age;
        } catch (std::exception &err){
          std::cout<< "get config max_connection_age from cmd error: " << err.what() << std::endl;
        }
        return value; 
      }); 
    program.add_argument("--max_connection_age_grace")
      .help("强制关闭连接之前允许等待的rpc在设置数值内完成,单位ms")
      .action([&](const std::string& value) { 
        try {
          auto max_connection_age_grace = std::stoi(value);
          conf->max_connection_age_grace = max_connection_age_grace;
        } catch (std::exception &err){
          std::cout<< "get config max_connection_age_grace from cmd error: " << err.what() << std::endl;
        }
        return value; 
      }); 

    program.add_argument("--keepalive_time")
      .help("空闲连接每隔设置数值ping一次客户端已确保连接存活,单位ms")
      .action([&](const std::string& value) { 
        try {
          auto keepalive_time = std::stoi(value);
          conf->keepalive_time = keepalive_time;
        } catch (std::exception &err){
          std::cout<< "get config keepalive_time from cmd error: " << err.what() << std::endl;
        }
        return value; 
      }); 

    program.add_argument("--keepalive_timeout")
      .help("ping时长超过设定值则认为连接已死,单位ms")
      .action([&](const std::string& value) { 
        try {
          auto keepalive_timeout = std::stoi(value);
          conf->keepalive_timeout = keepalive_timeout;
        } catch (std::exception &err){
          std::cout<< "get config keepalive_timeout from cmd error: " << err.what() << std::endl;
        }
        return value; 
      }); 

    program.add_argument("--not_keepalive_enforement_permit_without_stream")
      .help("没有活动流就不允许ping操作")
      .action([&](const std::string& value) { 
        conf->keepalive_enforement_permit_without_stream = false;
        return value; 
      }); 
      
    program.add_argument("--log_level")
      .help("设置log等级,默认debug,只支持trace,debug,info,warn,err,critical")
      .action([&](const std::string& value) { 
        std::vector<std::string> choices = {"trace", "debug", "info", "warn", "err", "critical"};
        if (std::find(choices.begin(), choices.end(), value) != choices.end()) {
          conf->log_level = value;
        }
        return value;
        });
    program.add_argument("--compression")
      .help("使用哪种方式压缩发送的消息,如果不设置则不做压缩,只支持deflate,gzip")
      .action([&](const std::string& value) { 
        std::vector<std::string> choices = {"deflate", "gzip"};
        if (std::find(choices.begin(), choices.end(), value) != choices.end()) {
          conf->compression = value;
        }
        return value;
        });
    program.add_argument("--index_dirs")
      .help("指定监听的目录,以`,`分割")
      .action([&](const std::string& value) { 
        conf->index_dirs = split(value,",");
        return value;
        });

    try {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error& err) {
        std::cout << err.what() << std::endl;
        std::cout << program;
        exit(0);
    }

}

int main(int argc, char** argv) {
    FAISS_RPCConf serv;
    std::ifstream file("config.json");
    if (file.is_open()) {
      json j = json::parse(file);
      file.close();
      serv = FAISS_RPCConf(j);
      }
    get_conf_from_env(&serv);
    get_conf_from_cmd(&serv,argc,argv);
    serv.RunServer();
    return 0;
}