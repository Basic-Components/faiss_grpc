#include <string>
#include <cstddef>
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
        std::string digest;
        std::string result_hash;
        CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(result_hash));
        CryptoPP::Weak1::MD5 hash;
        hash.Update((const CryptoPP::byte*)&content_str[0], content_str.size());
        digest.resize(hash.DigestSize());
        hash.Final((CryptoPP::byte*)&digest[0]);

        CryptoPP::StringSource(
            digest, 
            true,
            new CryptoPP::Redirector(encoder)
        );
        return result_hash;
    }
}