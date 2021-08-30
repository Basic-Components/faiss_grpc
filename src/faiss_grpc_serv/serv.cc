#include <memory>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>


#include <grpcpp/grpcpp.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/health_check_service_interface.h>
#include "schema_entry.h"
#include "spdloghelper.h"
#include <nlohmann/json.hpp>
#include <libfswatch/c++/monitor_factory.hpp>
#include <libfswatch/c++/monitor.hpp>


#include "../faiss_grpc_index_manager/index_manager.h"
#include "faiss_grpc_serv.h"

using nlohmann::json;
using Logger = spdloghelper::LogHelper;

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerReaderWriter;
using grpc::SslServerCredentialsOptions;

using fsw::monitor;
using fsw::monitor_factory;

using IndexManager = faiss_grpc_index_manager::IndexManager;

// 读取文件
void read (const std::string& filename, std::string& data){
    std::ifstream file ( filename.c_str (), std::ios::in );
    if ( file.is_open() ){
        std::stringstream ss;
        ss << file.rdbuf();
        file.close ();
        data = ss.str();
    }
    return;
}


namespace faiss_grpc_serv{
    void RootNode::main() {
        auto address = this->config["address"].get<std::string>();
        auto app_name = this->config["app_name"].get<std::string>();
        auto log_level = this->config["log_level"].get<std::string>();
        auto index_dir = this->config["index_dir"].get<std::string>();
        auto logger = Logger::getInstance();
        if (this->config.contains("app_version")){
            auto app_version = this->config["app_version"].get<std::string>();
            logger->init_logger(log_level,app_name,app_version);
        }else{
            logger->init_logger(log_level,app_name);
        }
        logger->info("get config",{{"config",this->config.dump() }});
        IndexManager* index_manager = IndexManager::getInstance();
        index_manager->set_index_dir(index_dir);
        index_manager->load_index_dir();
        
        //监听index变化
        monitor *active_monitor = monitor_factory::create_monitor(
            fsw_monitor_type::poll_monitor_type,
            {index_dir},
            faiss_grpc_index_manager::fschangecallback
        );
        // Configure the monitor
        active_monitor->add_event_type_filter({Updated});
        active_monitor->set_watch_access(false);

        // Start the monitor 
        std::thread t([&]() {
            logger->info("start watching fs",{{"path", index_dir}});
            active_monitor->start(); 
            });
        t.detach();


        //grpc
        FAISS_GRPC_RPCServiceImpl service;
        // 注册健康检查和反射接口
        grpc::EnableDefaultHealthCheckService(true);
        grpc::reflection::InitProtoReflectionServerBuilderPlugin();
        ServerBuilder builder;
        // 压缩
        if (this->config.contains("compression")){
            auto compression = this->config["compression"].get<std::string>();
            if (compression == std::string("gzip")){
                builder.SetDefaultCompressionAlgorithm(GRPC_COMPRESS_GZIP);
                logger->info("Compression setted",{ { "algo", compression } });
            }else if (compression == std::string("deflate")){
                builder.SetDefaultCompressionAlgorithm(GRPC_COMPRESS_DEFLATE);
                logger->info("Compression setted",{ { "algo", compression } });
            }else{
                logger->info("Compression not support",{ { "algo", compression } });
            }
        }
        // 消息大小设置
        if (this->config.contains("max_rec_msg_size")){
            auto max_rec_msg_size = this->config["max_rec_msg_size"].get<int>();
            builder.SetMaxReceiveMessageSize(max_rec_msg_size);
        }
        if (this->config.contains("max_send_msg_size")){
            auto max_send_msg_size = this->config["max_send_msg_size"].get<int>();
            builder.SetMaxReceiveMessageSize(max_send_msg_size);
        }
        //设置Channel参数
        if (this->config.contains("max_concurrent_streams")){
            auto max_concurrent_streams = this->config["max_concurrent_streams"].get<int>();
            if (max_concurrent_streams > 0){
                builder.AddChannelArgument(GRPC_ARG_MAX_CONCURRENT_STREAMS,max_concurrent_streams);
            }
        }
        if (this->config.contains("max_connection_idle")){
            auto max_connection_idle = this->config["max_connection_idle"].get<int>();
            if (max_connection_idle > 0){
                builder.AddChannelArgument(GRPC_ARG_MAX_CONNECTION_IDLE_MS,max_connection_idle);
            }
        }
        if (this->config.contains("max_connection_age")){
            auto max_connection_age = this->config["max_connection_age"].get<int>();
            if (max_connection_age > 0){
                builder.AddChannelArgument(GRPC_ARG_MAX_CONNECTION_AGE_MS,max_connection_age);
            }
        }
        if (this->config.contains("max_connection_age_grace")){
            auto max_connection_age_grace = this->config["max_connection_age_grace"].get<int>();
            if (max_connection_age_grace > 0){
                builder.AddChannelArgument(GRPC_ARG_MAX_CONNECTION_AGE_GRACE_MS,max_connection_age_grace);
            }
        }
        if (this->config.contains("keepalive_time")){
            auto keepalive_time = this->config["keepalive_time"].get<int>();
            if (keepalive_time > 0){
                builder.AddChannelArgument(GRPC_ARG_KEEPALIVE_TIME_MS,keepalive_time);
            }
        }
        if (this->config.contains("keepalive_timeout")){
            auto keepalive_timeout = this->config["keepalive_timeout"].get<int>();
            if (keepalive_timeout > 0){
                builder.AddChannelArgument(GRPC_ARG_KEEPALIVE_TIMEOUT_MS,keepalive_timeout);
            }
        }
        if (this->config.contains("keepalive_timeout")){
            auto keepalive_timeout = this->config["keepalive_timeout"].get<int>();
            if (keepalive_timeout > 0){
                builder.AddChannelArgument(GRPC_ARG_KEEPALIVE_TIMEOUT_MS,keepalive_timeout);
            }
        }
        if (this->config.contains("keepalive_enforement_permit_without_stream")){
            auto keepalive_enforement_permit_without_stream = this->config["keepalive_enforement_permit_without_stream"].get<bool>();
            if (keepalive_enforement_permit_without_stream){
                builder.AddChannelArgument(GRPC_ARG_KEEPALIVE_PERMIT_WITHOUT_CALLS,1);
            }else{
                builder.AddChannelArgument(GRPC_ARG_KEEPALIVE_PERMIT_WITHOUT_CALLS,0);
            }
        }
        
        // 注册监听端口(以及tls)
        //启用tls
        if (this->config.contains("server_cert_path") && this->config.contains("server_key_path")){
            auto server_cert_path = this->config["server_cert_path"].get<std::string>();
            auto server_key_path = this->config["server_key_path"].get<std::string>();
            SslServerCredentialsOptions sslOps;
            std::string key;
	        std::string cert;
            read( server_cert_path, cert );
	        read( server_key_path, key );
            SslServerCredentialsOptions::PemKeyCertPair keycert ={key,cert};
            sslOps.pem_key_cert_pairs.push_back ( keycert );
            // 启用客户端校验
            if (this->config.contains("root_cert_path")){
                auto root_cert_path = this->config["root_cert_path"].get<std::string>();
                std::string root;
                read( root_cert_path, root );
                sslOps.pem_root_certs = root;
                if (this->config.contains("client_certificate_request")){
                    auto client_certificate_request = this->config["client_certificate_request"].get<std::string>();
                    if (client_certificate_request == std::string("not_request")){
                        sslOps.force_client_auth=true;
                        sslOps.client_certificate_request=GRPC_SSL_DONT_REQUEST_CLIENT_CERTIFICATE;
                    } else if (client_certificate_request == std::string("not_verify")){
                        sslOps.force_client_auth=true;
                        sslOps.client_certificate_request=GRPC_SSL_REQUEST_CLIENT_CERTIFICATE_BUT_DONT_VERIFY;
                    } else if (client_certificate_request == std::string("request_and_verify")){
                        sslOps.force_client_auth=true;
                        sslOps.client_certificate_request=GRPC_SSL_REQUEST_CLIENT_CERTIFICATE_AND_VERIFY;
                    } else if (client_certificate_request == std::string("enforce_request")){
                        sslOps.force_client_auth=true;
                        sslOps.client_certificate_request=GRPC_SSL_REQUEST_AND_REQUIRE_CLIENT_CERTIFICATE_BUT_DONT_VERIFY;
                    } else if (client_certificate_request == std::string("enforce_request_and_verify")){
                        sslOps.force_client_auth=true;
                        sslOps.client_certificate_request=GRPC_SSL_REQUEST_AND_REQUIRE_CLIENT_CERTIFICATE_AND_VERIFY ;
                    } else {
                        sslOps.force_client_auth=false;
                        sslOps.client_certificate_request=GRPC_SSL_DONT_REQUEST_CLIENT_CERTIFICATE;
                    }
                }
            }
            builder.AddListeningPort(address, grpc::SslServerCredentials( sslOps ));
        }else{
            builder.AddListeningPort(address, grpc::InsecureServerCredentials());
        }
        
        // 注册服务实现
        builder.RegisterService(&service);
        // Finally assemble the server.
        std::unique_ptr<Server> server(builder.BuildAndStart());
        logger->info("Server start",{ { "address", address } });
        server->Wait();
        active_monitor->stop();
    }
}