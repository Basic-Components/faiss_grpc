#pragma once

#include <grpcpp/grpcpp.h>
#include "schema_entry.h"
#include "../faiss_grpc_pb/faiss_grpc.grpc.pb.h"

using grpc::ServerContext;
using grpc::Status;
using faiss_grpc::FAISS_GRPC_RPC;
using faiss_grpc::Message;

using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;

// 实现接口
namespace faiss_grpc_serv {
    class FAISS_GRPC_RPCServiceImpl final : public FAISS_GRPC_RPC::Service {
        Status Square(ServerContext* context, const Message* request, Message* response) override;
        Status RangeSquare(ServerContext* context, const Message* request, ServerWriter<Message>* writer) override;
        Status SumSquare(ServerContext* context, ServerReader<Message>* reader, Message* response) override;
        Status StreamrangeSquare(ServerContext* context, ServerReaderWriter<Message, Message>* stream) override;
    };
    const json SCHEMA = R"({
    "$schema": "http://json-schema.org/draft-07/schema#",
    "type": "object",
    "required": ["app_name", "address", "log_level"],
    "properties": {
        "app_version": {
            "type": "string",
            "title": "v",
            "description": "应用版本"
            "default": "0.0.2"
        },
        "app_name": {
            "type": "string",
            "title": "n",
            "description": "应用名",
            "default": "faiss_grpc"
        },
        "address": {
            "type": "string",
            "title": "a",
            "description": "服务启动地址",
            "default": "0.0.0.0:5000"
        },
        "log_level": {
            "type": "string",
            "title": "l",
            "description": "log等级",
            "enum": ["trace", "debug", "info", "warn", "err", "critical"],
            "default": "debug"
        },
        "server_cert_path": {
            "type": "string",
            "description": "使用TLS时服务端的证书位置"
        },
        "server_key_path": {
            "type": "string",
            "description": "使用TLS时服务端证书的私钥位置"
        },
        "root_cert_path": {
            "type": "string",
            "description": "使用TLS并启用客户端校验时根证书的位置"
        },
        "client_certificate_request": {
            "type": "string",
            "description": "使用TLS的校验策略",
            "enum": ["not_request","not_verify","request_and_verify","enforce_request","enforce_request_and_verify"]
        },
        "compression": {
            "type": "string",
            "title": "z",
            "description": "使用哪种方式压缩发送的消息",
            "enum": ["deflate", "gzip"]
        },
        "max_rec_msg_size": {
            "type": "integer",
            "description": "允许接收的最大消息长度",
            "default": 1048576
        },
        "max_send_msg_size": {
            "type": "integer",
            "description": "允许发送的最大消息长度",
            "default": 1048576
        },
        "max_concurrent_streams": {
            "type": "integer",
            "description": "单连接最大并发量",
            "default": 100
        },
        "max_connection_idle": {
            "type": "integer",
            "description": "客户端连接的最大空闲时长s"
        },
        "max_connection_age": {
            "type": "integer",
            "description": "如果连接存活超过n则发送goaway"
        },
        "max_connection_age_grace": {
            "type": "integer",
            "description": "强制关闭连接之前允许等待的rpc在n秒内完成"
        },
        "keepalive_time": {
            "type": "integer",
            "description": "空闲连接每隔n秒ping一次客户端已确保连接存活"
        },
        "keepalive_timeout": {
            "type": "integer",
            "description": "ping时长超过n则认为连接已死"
        },
        "keepalive_enforement_permit_without_stream": {
            "type": "boolean",
            "description": "即使没有活动流也允许ping"
        }
    }
})"_json;
    class RootNode:public schema_entry::EntryPoint{
        public:
        ~RootNode(){};
        void main() override;
    };
}