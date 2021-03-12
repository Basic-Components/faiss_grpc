#ifndef FAISS_RPC_SERV__INCLUDED
#define FAISS_RPC_SERV__INCLUDED
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <cmath>
#include <map>
#include <utility>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <nlohmann/adl_serializer.hpp>
#include "faiss_rpc.grpc.pb.h"

#include <filesystem>
// #include <pthread.h>
#include <utility>
#include <map>
#include <exception>
// #include <libfswatch/libfswatch.h>
#include <faiss/index.h>
#include <faiss/index_io.h>

namespace faiss_grpc_serv{
    class FaissGrpcConf {
        public:
            
            std::string app_version = "0.0.0";
            std::string app_name = "faiss_grpc";
            std::string address = "0.0.0.0:5000";
            std::string log_level = "debug";
            std::string server_cert_path = "";
            std::string server_key_path = "";
            std::vector< std::string > index_dirs = {"./indexes"};
            int max_rec_msg_size = 1024*1024;
            int max_send_msg_size = 1024 * 1024;
            int max_concurrent_streams = 100;
            int max_connection_idle = 0;
            int max_connection_age = 0;
            int max_connection_age_grace = 0;
            int keepalive_time = 0;
            int keepalive_timeout = 0;
            bool keepalive_enforement_permit_without_stream = true;
            std::string compression = "";

            FaissGrpcConf () = default;
            FaissGrpcConf(const nlohmann::json& j);
            nlohmann::json to_json() const;
            void RunServer();
    };
    void to_json(nlohmann::json& j, const FaissGrpcConf& p);

    void from_json(const nlohmann::json& j, FaissGrpcConf& p);
    // void RunServer(faiss_grpc_serv::FaissGrpcConf = faiss_grpc_serv::FaissGrpcConf());
}

#endif
