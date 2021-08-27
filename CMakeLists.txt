cmake_minimum_required(VERSION 3.0)

if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)

project(faiss_grpc
    VERSION 0.0.2
    DESCRIPTION "faiss_grpc 的grpc服务"
    LANGUAGES CXX
)


include(${CMAKE_CURRENT_LIST_DIR}/build/conanbuildinfo.cmake)
conan_basic_setup()

add_definitions("-std=c++17")
add_definitions("-fopenmp")
add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} 
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/faiss_grpc_pb/faiss_grpc.grpc.pb.cc
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/faiss_grpc_pb/faiss_grpc.grpc.pb.h
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/faiss_grpc_pb/faiss_grpc.pb.cc
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/faiss_grpc_pb/faiss_grpc.pb.h
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/faiss_grpc_serv/handler.cc
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/faiss_grpc_serv/serv.cc
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/faiss_grpc_serv/faiss_grpc_serv.h
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/faiss_grpc_index_manager/exception.h
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/faiss_grpc_index_manager/utils.h
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/faiss_grpc_index_manager/utils.cc
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/faiss_grpc_index_manager/index_info.h
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/faiss_grpc_index_manager/index_info.cc
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/faiss_grpc_index_manager/index_manager.h
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/faiss_grpc_index_manager/index_manager.cc
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/main.cc
)

target_link_libraries(${PROJECT_NAME}
    -static
    ${CONAN_LIBS}
    gomp
    openblas
)