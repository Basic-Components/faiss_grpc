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
include_directories(inc)

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
# # json
# message(NOTICE "下载nlohmann/json")
# FetchContent_Declare(json
#   # GIT_REPOSITORY https://github.com/nlohmann/json.git
#   # GIT_TAG        v3.7.3
#   # GIT_SHALLOW    TRUE
#   TLS_VERIFY     FALSE
#   URL  https://github.com/nlohmann/json/archive/v3.9.1.tar.gz
# )
# # FetchContent_GetProperties(json)
# # if(NOT json_POPULATED)
# #   FetchContent_Populate(json)
# #   add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
# # endif()
# FetchContent_MakeAvailable(json)
# ## log
# message(NOTICE "下载spdlog")
# FetchContent_Declare(spdlog
#   # GIT_REPOSITORY https://github.com/gabime/spdlog.git
#   # GIT_TAG        v1.8.2
#   # GIT_SHALLOW    TRUE
#   TLS_VERIFY     FALSE
#   URL  https://github.com/gabime/spdlog/archive/v1.8.2.tar.gz
# )
# # FetchContent_GetProperties(spdlog)
# # if(NOT spdlog_POPULATED)
# #   FetchContent_Populate(spdlog)
# #   add_subdirectory(${spdlog_SOURCE_DIR} ${spdlog_BINARY_DIR} EXCLUDE_FROM_ALL)
# # endif()
# FetchContent_MakeAvailable(spdlog)
# ## args
# message(NOTICE "下载argparse")
# FetchContent_Declare(argparse
#   # GIT_REPOSITORY https://github.com/p-ranav/argparse.git
#   # GIT_TAG        9903a22904fed8176c4a1f69c4b691304b23c78e
#   # GIT_SHALLOW    TRUE
#   TLS_VERIFY     FALSE
#   URL  https://github.com/p-ranav/argparse/archive/refs/tags/v2.0.tar.gz
# )
# # FetchContent_GetProperties(argparse)
# # if(NOT argparse_POPULATED)
# #   FetchContent_Populate(argparse)
# #   add_subdirectory(${argparse_SOURCE_DIR} ${argparse_BINARY_DIR} EXCLUDE_FROM_ALL)
# # endif()
# FetchContent_MakeAvailable(argparse)
## faiss
message(NOTICE "下载faiss")
FetchContent_Declare(faiss
  # GIT_REPOSITORY https://github.com/facebookresearch/faiss.git
  # GIT_TAG        v1.7.0
  # GIT_SHALLOW    TRUE
  TLS_VERIFY     FALSE
  URL  https://github.com/facebookresearch/faiss/archive/refs/tags/v1.7.0.tar.gz
)
# FetchContent_GetProperties(faiss)
# if(NOT faiss_POPULATED)
#   FetchContent_Populate(faiss)
#   add_subdirectory(${faiss_SOURCE_DIR} ${faiss_BINARY_DIR} EXCLUDE_FROM_ALL)
# endif()
FetchContent_MakeAvailable(faiss)
# include_directories(/code/_deps/faiss-src/faiss)

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

# # # https://github.com/facebookresearch/faiss/archive/refs/tags/v1.7.0.tar.gz
# # add_subdirectory(${THIRD_PART}/faiss  ${CMAKE_CURRENT_LIST_DIR}/lib/faiss)

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
# # 如果本地自己编译faiss
# # 引入faiss
# # Find gRPC include directory
# find_path(FAISS_INCLUDE_DIR NAMES faiss
#     /usr/local/include/faiss
# )
# if (FAISS_INCLUDE_DIR-NOTFOUND)
#     message(FATAL_ERROR "找不到faiss的头文件位置")
# endif()
# message(NOTICE "faiss的头文件位置${FAISS_INCLUDE_DIR}")
# mark_as_advanced(FAISS_INCLUDE_DIR)

# find_library(FAISS_LIBRARY NAMES libfaiss.a 
#     PATHS /usr/local/lib
#     REQUIRED
# )
# if (FAISS_LIBRARY-NOTFOUND)
#     message(FATAL_ERROR "找不到faiss的库文件位置")
# endif()
# message(NOTICE "faiss的库文件位置${FAISS_LIBRARY}")
# mark_as_advanced(FAISS_LIBRARY)
# add_library(faiss::libfaiss UNKNOWN IMPORTED)
# set_target_properties(faiss::libfaiss PROPERTIES
#     INTERFACE_INCLUDE_DIRECTORIES ${FAISS_INCLUDE_DIR}
#     INTERFACE_LINK_LIBRARIES "-lgomp;-lopenblas;-lpthread"
#     IMPORTED_LOCATION ${FAISS_LIBRARY}
# )

target_link_libraries(faiss_grpc
PRIVATE nlohmann_json::nlohmann_json
PRIVATE spdlog::spdlog
PRIVATE argparse::argparse
PRIVATE gRPC::grpc++_reflection
PRIVATE protobuf::libprotobuf
# PRIVATE faiss:libfaiss
PUBLIC faiss
PRIVATE fswatch
PRIVATE stdc++fs
#PRIVATE OpenMP::OpenMP_CXX
)


