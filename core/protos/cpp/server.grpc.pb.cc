// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: server.proto

#include "server.pb.h"
#include "server.grpc.pb.h"

#include <functional>
#include <grpcpp/support/async_stream.h>
#include <grpcpp/support/async_unary_call.h>
#include <grpcpp/impl/channel_interface.h>
#include <grpcpp/impl/client_unary_call.h>
#include <grpcpp/support/client_callback.h>
#include <grpcpp/support/message_allocator.h>
#include <grpcpp/support/method_handler.h>
#include <grpcpp/impl/rpc_service_method.h>
#include <grpcpp/support/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/server_context.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/sync_stream.h>
namespace PFDESolverServer {

static const char* TFDESolverServer_method_names[] = {
  "/PFDESolverServer.TFDESolverServer/RunTask",
};

std::unique_ptr< TFDESolverServer::Stub> TFDESolverServer::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< TFDESolverServer::Stub> stub(new TFDESolverServer::Stub(channel, options));
  return stub;
}

TFDESolverServer::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_RunTask_(TFDESolverServer_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status TFDESolverServer::Stub::RunTask(::grpc::ClientContext* context, const ::PFDESolver::TClientConfig& request, ::PFDESolver::TResult* response) {
  return ::grpc::internal::BlockingUnaryCall< ::PFDESolver::TClientConfig, ::PFDESolver::TResult, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_RunTask_, context, request, response);
}

void TFDESolverServer::Stub::async::RunTask(::grpc::ClientContext* context, const ::PFDESolver::TClientConfig* request, ::PFDESolver::TResult* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::PFDESolver::TClientConfig, ::PFDESolver::TResult, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_RunTask_, context, request, response, std::move(f));
}

void TFDESolverServer::Stub::async::RunTask(::grpc::ClientContext* context, const ::PFDESolver::TClientConfig* request, ::PFDESolver::TResult* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_RunTask_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::PFDESolver::TResult>* TFDESolverServer::Stub::PrepareAsyncRunTaskRaw(::grpc::ClientContext* context, const ::PFDESolver::TClientConfig& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::PFDESolver::TResult, ::PFDESolver::TClientConfig, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_RunTask_, context, request);
}

::grpc::ClientAsyncResponseReader< ::PFDESolver::TResult>* TFDESolverServer::Stub::AsyncRunTaskRaw(::grpc::ClientContext* context, const ::PFDESolver::TClientConfig& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncRunTaskRaw(context, request, cq);
  result->StartCall();
  return result;
}

TFDESolverServer::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      TFDESolverServer_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< TFDESolverServer::Service, ::PFDESolver::TClientConfig, ::PFDESolver::TResult, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](TFDESolverServer::Service* service,
             ::grpc::ServerContext* ctx,
             const ::PFDESolver::TClientConfig* req,
             ::PFDESolver::TResult* resp) {
               return service->RunTask(ctx, req, resp);
             }, this)));
}

TFDESolverServer::Service::~Service() {
}

::grpc::Status TFDESolverServer::Service::RunTask(::grpc::ServerContext* context, const ::PFDESolver::TClientConfig* request, ::PFDESolver::TResult* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace PFDESolverServer

