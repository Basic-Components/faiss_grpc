#include <fstream>
#include <cstdlib>
#include <iostream>
#include <exception>
#include <nlohmann/json.hpp>
#include <nlohmann/adl_serializer.hpp>
#include "spdlog/spdlog.h"
using json = nlohmann::json;
namespace faiss_grpc_serv{
   
    class FaissGrpcConf {
        public:
            std::string app_version = "0.0.0";
            std::string app_name = "faiss_grpc";
            std::string address = "0.0.0.0:5000";
            std::string log_level = "DEBUG";
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
            FaissGrpcConf(const json& j){
                try{
                    j.at("app_version").get_to(this->app_version);
                }catch(std::exception &e){
                    this->app_version = "0.0.0";
                }
                try{
                    j.at("app_name").get_to(this->app_name);
                }catch(std::exception &e){
                    this->app_name = "faiss_grpc";
                }
                try{
                    j.at("address").get_to(this->address);
                }catch(std::exception &e){
                    this->address = "address";
                }
                try{
                    j.at("log_level").get_to(this->log_level);
                }catch(std::exception &e){
                    this->log_level = "DEBUG";
                }
                try{
                    j.at("server_cert_path").get_to(this->server_cert_path);
                }catch(std::exception &e){
                    this->server_cert_path = "";
                }
                try{
                    j.at("server_key_path").get_to(this->server_key_path);
                }catch(std::exception &e){
                    this->server_key_path = "";
                }
                try{
                    j.at("index_dirs").get_to(this->index_dirs);
                }catch(std::exception &e){
                    this->index_dirs = {"./indexes"};
                }
                try{
                    j.at("max_rec_msg_size").get_to(this->max_rec_msg_size);
                }catch(std::exception &e){
                    this->max_rec_msg_size = 1024*1024;
                }
                try{
                    j.at("max_send_msg_size").get_to(this->max_send_msg_size);
                }catch(std::exception &e){
                    this->max_send_msg_size = 1024*1024;
                }
                try{
                    j.at("max_concurrent_streams").get_to(this->max_concurrent_streams);
                }catch(std::exception &e){
                    this->max_concurrent_streams = 100;
                }
                try{
                    j.at("max_connection_idle").get_to(this->max_connection_idle);
                }catch(std::exception &e){
                    this->max_connection_idle = 0;
                }
                try{
                    j.at("max_connection_age").get_to(this->max_connection_age);
                }catch(std::exception &e){
                    this->max_connection_age = 0;
                }
                try{
                    j.at("max_connection_age_grace").get_to(this->max_connection_age_grace);
                }catch(std::exception &e){
                    this->max_connection_age_grace = 0;
                }
                try{
                    j.at("keepalive_time").get_to(this->keepalive_time);
                }catch(std::exception &e){
                    this->keepalive_time = 0;
                }
                try{
                    j.at("keepalive_timeout").get_to(this->keepalive_timeout);
                }catch(std::exception &e){
                    this->keepalive_timeout = 0;
                }
                try{
                    j.at("keepalive_enforement_permit_without_stream").get_to(this->keepalive_enforement_permit_without_stream);
                }catch(std::exception &e){
                    this->keepalive_enforement_permit_without_stream = true;
                }
                try{
                    j.at("compression").get_to(this->compression);
                }catch(std::exception &e){
                    this->compression = "";
                }
            }
            
            json to_json() const {
                auto j = json{
                    {"app_version", this->app_version}, 
                    {"app_name", this->app_name}, 
                    {"address", this->address},
                    {"log_level", this->log_level},
                    {"server_cert_path", this->server_cert_path},
                    {"server_key_path", this->server_key_path},
                    {"index_dirs", this->index_dirs},
                    {"max_rec_msg_size", this->max_rec_msg_size},
                    {"max_send_msg_size", this->max_send_msg_size},
                    {"max_concurrent_streams", this->max_concurrent_streams},
                    {"max_connection_idle", this->max_connection_idle},
                    {"max_connection_age", this->max_connection_age},
                    {"max_connection_age_grace", this->max_connection_age_grace},
                    {"keepalive_time", this->keepalive_time},
                    {"keepalive_timeout", this->keepalive_timeout},
                    {"keepalive_enforement_permit_without_stream", this->keepalive_enforement_permit_without_stream},
                    {"compression", this->compression}
                };
                return j;
            };
            
    };
    void to_json(json& j, const FaissGrpcConf& p) {
        j = p.to_json();
    }

    void from_json(const json& j, FaissGrpcConf& p) {
      p = FaissGrpcConf(j);
    }
}
int main(int argc, char* argv[]){
    // auto mylogger = spdlog::get("json_logger");
    // std::string jsonlastlogpattern = { "{\"time\": \"%Y-%m-%dT%H:%M:%S.%f%z\", \"name\": \"%n\", \"level\": \"%^%l%$\", \"process\": %P, \"thread\": %t, \"message\": \"%v\"}" };
    // spdlog::set_pattern(jsonlastlogpattern);
    std::string jsonpattern = {"{\"time\": \"%Y-%m-%dT%H:%M:%S.%f%z\", \"name\": \"%n\", \"level\": \"%^%l%$\", \"process\": %P, \"thread\": %t,\"message\": %v}"};
    spdlog::set_pattern(jsonpattern);
    spdlog::info("{ \"message\": \"We have started.\"}");
    std::ifstream file("test.json");
    if (!file.is_open()) {
        std::cout << "failed to open test.json" << '\n';
        return 1;
    } 
    json j = json::parse(file);
    file.close();

    auto fgc = faiss_grpc_serv::FaissGrpcConf(j);
    // faiss_grpc_serv::FaissGrpcConf fgc;
    // json j2 = fgc;
    // spdlog::info(j2.dump());
    spdlog::info(fgc.to_json().dump());
    return 0;
}