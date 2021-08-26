#项目编译环境
cmake_minimum_required (VERSION 3.13)
project (faiss_grpc
    VERSION 0.0.0
    DESCRIPTION "a faiss grpc"
    LANGUAGES CXX
)

# grpc
list(APPEND CMAKE_PREFIX_PATH "/opt/grpc" "/opt/protobuf")
# Cmake find modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# proto
find_package(Protobuf REQUIRED)
find_package(GRPC REQUIRED)
set(PROTOS
    ${CMAKE_CURRENT_LIST_DIR}/pbschema/faiss_rpc.proto
)
set(PROTO_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/proto-src)
file(MAKE_DIRECTORY ${PROTO_SRC_DIR})
include_directories(${PROTO_SRC_DIR})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS  ${PROTO_SRC_DIR} ${PROTOS})
grpc_generate_cpp(GRPC_SRCS GRPC_HDRS ${PROTO_SRC_DIR} ${PROTOS})


# 下载外部依赖源码
message(NOTICE "下载外部依赖")
include(FetchContent)
# json
message(NOTICE "下载nlohmann/json")
FetchContent_Declare(json
  TLS_VERIFY     FALSE
  URL  https://github.com/nlohmann/json/archive/v3.9.1.tar.gz
)
FetchContent_MakeAvailable(json)
## log
message(NOTICE "下载spdlog")
FetchContent_Declare(spdlog
  TLS_VERIFY     FALSE
  URL  https://github.com/gabime/spdlog/archive/v1.8.2.tar.gz
)
FetchContent_MakeAvailable(spdlog)
## args
message(NOTICE "下载argparse")
FetchContent_Declare(argparse
  # GIT_REPOSITORY https://github.com/p-ranav/argparse.git
  # GIT_TAG        9903a22904fed8176c4a1f69c4b691304b23c78e
  # GIT_SHALLOW    TRUE
  URL  https://github.com/p-ranav/argparse/archive/refs/heads/master.zip
  TLS_VERIFY     FALSE
)
FetchContent_MakeAvailable(argparse)

message(NOTICE "下载faiss")
FetchContent_Declare(Faiss
  URL  https://github.com/facebookresearch/faiss/archive/refs/tags/v1.7.0.tar.gz
  TLS_VERIFY     FALSE
)
FetchContent_MakeAvailable(Faiss)

add_library(faiss::faiss ALIAS faiss)

# 编译
message(NOTICE "开始编译")
set(SRC_DIR  ${CMAKE_CURRENT_LIST_DIR}/src)
add_executable(${PROJECT_NAME}
    ${SRC_DIR}/faiss_grpc.cc
    ${SRC_DIR}/faiss_rpc_serv.cc
    ${PROTO_SRCS}
    ${GRPC_SRCS}
)
target_compile_options(${PROJECT_NAME} 
    PRIVATE "-std=c++17"
)
set_target_properties(${PROJECT_NAME} PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin
)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${SRC_DIR}
)
target_link_libraries(${PROJECT_NAME}
  PRIVATE nlohmann_json::nlohmann_json
  PRIVATE argparse::argparse
  PRIVATE nlohmann_json::nlohmann_json
  PRIVATE spdlog::spdlog
  PRIVATE gRPC::grpc++_reflection
  PRIVATE protobuf::libprotobuf
  PRIVATE faiss::faiss
  PRIVATE fswatch
  PRIVATE stdc++fs
)


