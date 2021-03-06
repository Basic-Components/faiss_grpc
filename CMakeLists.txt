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

#添加外部依赖(如果可以最好还是用上面的下载方式)
set(THIRD_PART
    ${CMAKE_CURRENT_LIST_DIR}/thirdpart
)

# https://github.com/nlohmann/json/archive/v3.9.1.tar.gz
add_subdirectory(${THIRD_PART}/nlohmann_json)
message(NOTICE "下载nlohmann/json")
# https://github.com/gabime/spdlog/archive/v1.8.2.tar.gz
add_subdirectory(${THIRD_PART}/spdlog)
message(NOTICE "安装spdlog")
# https://github.com/p-ranav/argparse/archive/refs/tags/v2.0.tar.gz
add_subdirectory(${THIRD_PART}/argparse)
message(NOTICE "安装argparse")

find_package(Faiss REQUIRED)
if (Faiss-NOTFOUND)
    message(FATAL_ERROR "找不到Faiss")
endif()
include_directories(${FAISS_FAISS_INCLUDE_DIR})

# 编译
message(NOTICE "开始编译")
add_compile_options(-std=c++17)
set(SRC_DIR 
    ${CMAKE_CURRENT_LIST_DIR}/src
)
include_directories(${SRC_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)
add_executable(faiss_grpc
    ${SRC_DIR}/faiss_grpc.cc
    ${SRC_DIR}/faiss_rpc_serv.cc
    ${PROTO_SRCS}
    ${GRPC_SRCS}
)

target_link_libraries(faiss_grpc
PRIVATE nlohmann_json::nlohmann_json
PRIVATE argparse::argparse
PRIVATE nlohmann_json::nlohmann_json
PRIVATE spdlog::spdlog
PRIVATE gRPC::grpc++_reflection
PRIVATE protobuf::libprotobuf
PRIVATE faiss::libfaiss
PRIVATE fswatch
PRIVATE stdc++fs
)


