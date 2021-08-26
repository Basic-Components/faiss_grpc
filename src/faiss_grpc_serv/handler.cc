#include <cmath>
#include "spdloghelper.h"
#include <grpcpp/grpcpp.h>
#include "../faiss_grpc_pb/faiss_grpc.grpc.pb.h"
#include "faiss_grpc_serv.h"

using grpc::ServerContext;
using grpc::Status;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using faiss_grpc::FAISS_GRPC_RPC;
using faiss_grpc::Message;

using Logger = spdloghelper::LogHelper;

// 实现接口
namespace faiss_grpc_serv {
    // req-res
    Status FAISS_GRPC_RPCServiceImpl::Square(ServerContext* context, const Message* request, Message* response) {
        Logger* logger = Logger::getInstance();
        logger->info("Square get message",{ { "msg", request->message() } });
        response->set_message(std::pow(request->message(), 2));
        return Status::OK;
    }
    // stream-stream
    Status FAISS_GRPC_RPCServiceImpl::StreamrangeSquare(ServerContext* context,ServerReaderWriter<Message,Message>* stream) {
        Logger* logger = Logger::getInstance();
        Message temp;
        while (stream->Read(&temp)) {
            logger->info("StreamrangeSquare get message",{ { "msg", temp.message() } });
            auto max = temp.message();
            for (int i; i < max; i++ ) {
                Message m;
                auto ii = double(i);
                m.set_message(std::pow(ii,2));
                stream->Write(m);
            }
        }
        return Status::OK;
    }
}