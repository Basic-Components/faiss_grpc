#include <string>

#include <cryptopp/cryptlib.h>
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/md5.h>
#include <cryptopp/hex.h>

#include "utils.h"

namespace faiss_grpc_index_manager{
    bool endsWith(std::string s, std::string sub){
        return s.rfind(sub) == (s.length() - sub.length()) ? true : false;
    }

    std::string get_content_hash(std::string content_str){
        CryptoPP::Weak1::MD5 md;
        const size_t size = CryptoPP::Weak1::MD5::DIGESTSIZE * 2;
        byte buf[size] = {0};
        CryptoPP::FileSource(
            content_str.c_str(), 
            true,
            new CryptoPP::HashFilter(md, new CryptoPP::HexEncoder(new CryptoPP::ArraySink(buf, size)))
        );
        auto result_hash = std::string(reinterpret_cast<const char*>(buf), size);
        return result_hash
    }
}