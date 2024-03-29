// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: faiss_grpc.proto

#include "faiss_grpc.pb.h"
#include "faiss_grpc.grpc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace faiss_grpc {

static const char* FAISS_GRPC_method_names[] = {
  "/faiss_grpc.FAISS_GRPC/search",
  "/faiss_grpc.FAISS_GRPC/batch_search",
  "/faiss_grpc.FAISS_GRPC/get_index_list",
  "/faiss_grpc.FAISS_GRPC/get_index_metadata",
  "/faiss_grpc.FAISS_GRPC/reload_index",
};

std::unique_ptr< FAISS_GRPC::Stub> FAISS_GRPC::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< FAISS_GRPC::Stub> stub(new FAISS_GRPC::Stub(channel, options));
  return stub;
}

FAISS_GRPC::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_search_(FAISS_GRPC_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_batch_search_(FAISS_GRPC_method_names[1], options.suffix_for_stats(),::grpc::internal::RpcMethod::BIDI_STREAMING, channel)
  , rpcmethod_get_index_list_(FAISS_GRPC_method_names[2], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_get_index_metadata_(FAISS_GRPC_method_names[3], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_reload_index_(FAISS_GRPC_method_names[4], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status FAISS_GRPC::Stub::search(::grpc::ClientContext* context, const ::faiss_grpc::Query& request, ::faiss_grpc::Response* response) {
  return ::grpc::internal::BlockingUnaryCall< ::faiss_grpc::Query, ::faiss_grpc::Response, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_search_, context, request, response);
}

void FAISS_GRPC::Stub::async::search(::grpc::ClientContext* context, const ::faiss_grpc::Query* request, ::faiss_grpc::Response* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::faiss_grpc::Query, ::faiss_grpc::Response, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_search_, context, request, response, std::move(f));
}

void FAISS_GRPC::Stub::async::search(::grpc::ClientContext* context, const ::faiss_grpc::Query* request, ::faiss_grpc::Response* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_search_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::faiss_grpc::Response>* FAISS_GRPC::Stub::PrepareAsyncsearchRaw(::grpc::ClientContext* context, const ::faiss_grpc::Query& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::faiss_grpc::Response, ::faiss_grpc::Query, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_search_, context, request);
}

::grpc::ClientAsyncResponseReader< ::faiss_grpc::Response>* FAISS_GRPC::Stub::AsyncsearchRaw(::grpc::ClientContext* context, const ::faiss_grpc::Query& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncsearchRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::ClientReaderWriter< ::faiss_grpc::Query, ::faiss_grpc::Response>* FAISS_GRPC::Stub::batch_searchRaw(::grpc::ClientContext* context) {
  return ::grpc::internal::ClientReaderWriterFactory< ::faiss_grpc::Query, ::faiss_grpc::Response>::Create(channel_.get(), rpcmethod_batch_search_, context);
}

void FAISS_GRPC::Stub::async::batch_search(::grpc::ClientContext* context, ::grpc::ClientBidiReactor< ::faiss_grpc::Query,::faiss_grpc::Response>* reactor) {
  ::grpc::internal::ClientCallbackReaderWriterFactory< ::faiss_grpc::Query,::faiss_grpc::Response>::Create(stub_->channel_.get(), stub_->rpcmethod_batch_search_, context, reactor);
}

::grpc::ClientAsyncReaderWriter< ::faiss_grpc::Query, ::faiss_grpc::Response>* FAISS_GRPC::Stub::Asyncbatch_searchRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc::internal::ClientAsyncReaderWriterFactory< ::faiss_grpc::Query, ::faiss_grpc::Response>::Create(channel_.get(), cq, rpcmethod_batch_search_, context, true, tag);
}

::grpc::ClientAsyncReaderWriter< ::faiss_grpc::Query, ::faiss_grpc::Response>* FAISS_GRPC::Stub::PrepareAsyncbatch_searchRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncReaderWriterFactory< ::faiss_grpc::Query, ::faiss_grpc::Response>::Create(channel_.get(), cq, rpcmethod_batch_search_, context, false, nullptr);
}

::grpc::Status FAISS_GRPC::Stub::get_index_list(::grpc::ClientContext* context, const ::faiss_grpc::ListQuery& request, ::faiss_grpc::ListResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::faiss_grpc::ListQuery, ::faiss_grpc::ListResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_get_index_list_, context, request, response);
}

void FAISS_GRPC::Stub::async::get_index_list(::grpc::ClientContext* context, const ::faiss_grpc::ListQuery* request, ::faiss_grpc::ListResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::faiss_grpc::ListQuery, ::faiss_grpc::ListResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_get_index_list_, context, request, response, std::move(f));
}

void FAISS_GRPC::Stub::async::get_index_list(::grpc::ClientContext* context, const ::faiss_grpc::ListQuery* request, ::faiss_grpc::ListResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_get_index_list_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::faiss_grpc::ListResponse>* FAISS_GRPC::Stub::PrepareAsyncget_index_listRaw(::grpc::ClientContext* context, const ::faiss_grpc::ListQuery& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::faiss_grpc::ListResponse, ::faiss_grpc::ListQuery, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_get_index_list_, context, request);
}

::grpc::ClientAsyncResponseReader< ::faiss_grpc::ListResponse>* FAISS_GRPC::Stub::Asyncget_index_listRaw(::grpc::ClientContext* context, const ::faiss_grpc::ListQuery& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncget_index_listRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status FAISS_GRPC::Stub::get_index_metadata(::grpc::ClientContext* context, const ::faiss_grpc::MetadataQuery& request, ::faiss_grpc::MetadataResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::faiss_grpc::MetadataQuery, ::faiss_grpc::MetadataResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_get_index_metadata_, context, request, response);
}

void FAISS_GRPC::Stub::async::get_index_metadata(::grpc::ClientContext* context, const ::faiss_grpc::MetadataQuery* request, ::faiss_grpc::MetadataResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::faiss_grpc::MetadataQuery, ::faiss_grpc::MetadataResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_get_index_metadata_, context, request, response, std::move(f));
}

void FAISS_GRPC::Stub::async::get_index_metadata(::grpc::ClientContext* context, const ::faiss_grpc::MetadataQuery* request, ::faiss_grpc::MetadataResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_get_index_metadata_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::faiss_grpc::MetadataResponse>* FAISS_GRPC::Stub::PrepareAsyncget_index_metadataRaw(::grpc::ClientContext* context, const ::faiss_grpc::MetadataQuery& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::faiss_grpc::MetadataResponse, ::faiss_grpc::MetadataQuery, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_get_index_metadata_, context, request);
}

::grpc::ClientAsyncResponseReader< ::faiss_grpc::MetadataResponse>* FAISS_GRPC::Stub::Asyncget_index_metadataRaw(::grpc::ClientContext* context, const ::faiss_grpc::MetadataQuery& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncget_index_metadataRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status FAISS_GRPC::Stub::reload_index(::grpc::ClientContext* context, const ::faiss_grpc::ReloadQuery& request, ::faiss_grpc::ReloadResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::faiss_grpc::ReloadQuery, ::faiss_grpc::ReloadResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_reload_index_, context, request, response);
}

void FAISS_GRPC::Stub::async::reload_index(::grpc::ClientContext* context, const ::faiss_grpc::ReloadQuery* request, ::faiss_grpc::ReloadResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::faiss_grpc::ReloadQuery, ::faiss_grpc::ReloadResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_reload_index_, context, request, response, std::move(f));
}

void FAISS_GRPC::Stub::async::reload_index(::grpc::ClientContext* context, const ::faiss_grpc::ReloadQuery* request, ::faiss_grpc::ReloadResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_reload_index_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::faiss_grpc::ReloadResponse>* FAISS_GRPC::Stub::PrepareAsyncreload_indexRaw(::grpc::ClientContext* context, const ::faiss_grpc::ReloadQuery& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::faiss_grpc::ReloadResponse, ::faiss_grpc::ReloadQuery, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_reload_index_, context, request);
}

::grpc::ClientAsyncResponseReader< ::faiss_grpc::ReloadResponse>* FAISS_GRPC::Stub::Asyncreload_indexRaw(::grpc::ClientContext* context, const ::faiss_grpc::ReloadQuery& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncreload_indexRaw(context, request, cq);
  result->StartCall();
  return result;
}

FAISS_GRPC::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      FAISS_GRPC_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< FAISS_GRPC::Service, ::faiss_grpc::Query, ::faiss_grpc::Response, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](FAISS_GRPC::Service* service,
             ::grpc::ServerContext* ctx,
             const ::faiss_grpc::Query* req,
             ::faiss_grpc::Response* resp) {
               return service->search(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      FAISS_GRPC_method_names[1],
      ::grpc::internal::RpcMethod::BIDI_STREAMING,
      new ::grpc::internal::BidiStreamingHandler< FAISS_GRPC::Service, ::faiss_grpc::Query, ::faiss_grpc::Response>(
          [](FAISS_GRPC::Service* service,
             ::grpc::ServerContext* ctx,
             ::grpc::ServerReaderWriter<::faiss_grpc::Response,
             ::faiss_grpc::Query>* stream) {
               return service->batch_search(ctx, stream);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      FAISS_GRPC_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< FAISS_GRPC::Service, ::faiss_grpc::ListQuery, ::faiss_grpc::ListResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](FAISS_GRPC::Service* service,
             ::grpc::ServerContext* ctx,
             const ::faiss_grpc::ListQuery* req,
             ::faiss_grpc::ListResponse* resp) {
               return service->get_index_list(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      FAISS_GRPC_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< FAISS_GRPC::Service, ::faiss_grpc::MetadataQuery, ::faiss_grpc::MetadataResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](FAISS_GRPC::Service* service,
             ::grpc::ServerContext* ctx,
             const ::faiss_grpc::MetadataQuery* req,
             ::faiss_grpc::MetadataResponse* resp) {
               return service->get_index_metadata(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      FAISS_GRPC_method_names[4],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< FAISS_GRPC::Service, ::faiss_grpc::ReloadQuery, ::faiss_grpc::ReloadResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](FAISS_GRPC::Service* service,
             ::grpc::ServerContext* ctx,
             const ::faiss_grpc::ReloadQuery* req,
             ::faiss_grpc::ReloadResponse* resp) {
               return service->reload_index(ctx, req, resp);
             }, this)));
}

FAISS_GRPC::Service::~Service() {
}

::grpc::Status FAISS_GRPC::Service::search(::grpc::ServerContext* context, const ::faiss_grpc::Query* request, ::faiss_grpc::Response* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status FAISS_GRPC::Service::batch_search(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::faiss_grpc::Response, ::faiss_grpc::Query>* stream) {
  (void) context;
  (void) stream;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status FAISS_GRPC::Service::get_index_list(::grpc::ServerContext* context, const ::faiss_grpc::ListQuery* request, ::faiss_grpc::ListResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status FAISS_GRPC::Service::get_index_metadata(::grpc::ServerContext* context, const ::faiss_grpc::MetadataQuery* request, ::faiss_grpc::MetadataResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status FAISS_GRPC::Service::reload_index(::grpc::ServerContext* context, const ::faiss_grpc::ReloadQuery* request, ::faiss_grpc::ReloadResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace faiss_grpc

