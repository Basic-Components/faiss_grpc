#pragma once

#include <string>

namespace faiss_grpc_index_manager{
    //endsWith c++20才可以替换掉
    bool endsWith(std::string, std::string);
    //get_content_hash 获取内容文本的hash值
    std::string get_content_hash(std::string);
}