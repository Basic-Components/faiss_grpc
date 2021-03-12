#include <cstdlib>
#include <cstddef>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
#include "clipp.h"
#include "faiss_grpc_serv.h"

using faiss_grpc_serv::FaissGrpcConf;

using json = nlohmann::json;
int main(int argc, char** argv) {
  FaissGrpcConf serv;
  std::ifstream file("config.json");
  if (file.is_open()) {
    json j = json::parse(file);
    file.close();
    serv = FaissGrpcConf(j);
  }
  if(const char* env_address = std::getenv("FAISS_GRPC_ADDRESS")){
    serv.address = env_address;
  }
  serv.RunServer();
  return 0;
}