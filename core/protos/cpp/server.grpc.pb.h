// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: server.proto
#ifndef GRPC_server_2eproto__INCLUDED
#define GRPC_server_2eproto__INCLUDED

#include "server.pb.h"

#include <functional>
#include <grpcpp/generic/async_generic_service.h>
#include <grpcpp/support/async_stream.h>
#include <grpcpp/support/async_unary_call.h>
#include <grpcpp/support/client_callback.h>
#include <grpcpp/client_context.h>
#include <grpcpp/completion_queue.h>
#include <grpcpp/support/message_allocator.h>
#include <grpcpp/support/method_handler.h>
#include <grpcpp/impl/proto_utils.h>
#include <grpcpp/impl/rpc_method.h>
#include <grpcpp/support/server_callback.h>
#include <grpcpp/impl/server_callback_handlers.h>
#include <grpcpp/server_context.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/status.h>
#include <grpcpp/support/stub_options.h>
#include <grpcpp/support/sync_stream.h>

namespace PFDESolverServer {

class TFDESolverServer final {
 public:
  static constexpr char const* service_full_name() {
    return "PFDESolverServer.TFDESolverServer";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    virtual ::grpc::Status RunTask(::grpc::ClientContext* context, const ::PFDESolver::TClientConfig& request, ::PFDESolver::TResults* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::PFDESolver::TResults>> AsyncRunTask(::grpc::ClientContext* context, const ::PFDESolver::TClientConfig& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::PFDESolver::TResults>>(AsyncRunTaskRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::PFDESolver::TResults>> PrepareAsyncRunTask(::grpc::ClientContext* context, const ::PFDESolver::TClientConfig& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::PFDESolver::TResults>>(PrepareAsyncRunTaskRaw(context, request, cq));
    }
    class async_interface {
     public:
      virtual ~async_interface() {}
      virtual void RunTask(::grpc::ClientContext* context, const ::PFDESolver::TClientConfig* request, ::PFDESolver::TResults* response, std::function<void(::grpc::Status)>) = 0;
      virtual void RunTask(::grpc::ClientContext* context, const ::PFDESolver::TClientConfig* request, ::PFDESolver::TResults* response, ::grpc::ClientUnaryReactor* reactor) = 0;
    };
    typedef class async_interface experimental_async_interface;
    virtual class async_interface* async() { return nullptr; }
    class async_interface* experimental_async() { return async(); }
   private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::PFDESolver::TResults>* AsyncRunTaskRaw(::grpc::ClientContext* context, const ::PFDESolver::TClientConfig& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::PFDESolver::TResults>* PrepareAsyncRunTaskRaw(::grpc::ClientContext* context, const ::PFDESolver::TClientConfig& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());
    ::grpc::Status RunTask(::grpc::ClientContext* context, const ::PFDESolver::TClientConfig& request, ::PFDESolver::TResults* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::PFDESolver::TResults>> AsyncRunTask(::grpc::ClientContext* context, const ::PFDESolver::TClientConfig& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::PFDESolver::TResults>>(AsyncRunTaskRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::PFDESolver::TResults>> PrepareAsyncRunTask(::grpc::ClientContext* context, const ::PFDESolver::TClientConfig& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::PFDESolver::TResults>>(PrepareAsyncRunTaskRaw(context, request, cq));
    }
    class async final :
      public StubInterface::async_interface {
     public:
      void RunTask(::grpc::ClientContext* context, const ::PFDESolver::TClientConfig* request, ::PFDESolver::TResults* response, std::function<void(::grpc::Status)>) override;
      void RunTask(::grpc::ClientContext* context, const ::PFDESolver::TClientConfig* request, ::PFDESolver::TResults* response, ::grpc::ClientUnaryReactor* reactor) override;
     private:
      friend class Stub;
      explicit async(Stub* stub): stub_(stub) { }
      Stub* stub() { return stub_; }
      Stub* stub_;
    };
    class async* async() override { return &async_stub_; }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    class async async_stub_{this};
    ::grpc::ClientAsyncResponseReader< ::PFDESolver::TResults>* AsyncRunTaskRaw(::grpc::ClientContext* context, const ::PFDESolver::TClientConfig& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::PFDESolver::TResults>* PrepareAsyncRunTaskRaw(::grpc::ClientContext* context, const ::PFDESolver::TClientConfig& request, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_RunTask_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status RunTask(::grpc::ServerContext* context, const ::PFDESolver::TClientConfig* request, ::PFDESolver::TResults* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_RunTask : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_RunTask() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_RunTask() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status RunTask(::grpc::ServerContext* /*context*/, const ::PFDESolver::TClientConfig* /*request*/, ::PFDESolver::TResults* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestRunTask(::grpc::ServerContext* context, ::PFDESolver::TClientConfig* request, ::grpc::ServerAsyncResponseWriter< ::PFDESolver::TResults>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_RunTask<Service > AsyncService;
  template <class BaseClass>
  class WithCallbackMethod_RunTask : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithCallbackMethod_RunTask() {
      ::grpc::Service::MarkMethodCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::PFDESolver::TClientConfig, ::PFDESolver::TResults>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::PFDESolver::TClientConfig* request, ::PFDESolver::TResults* response) { return this->RunTask(context, request, response); }));}
    void SetMessageAllocatorFor_RunTask(
        ::grpc::MessageAllocator< ::PFDESolver::TClientConfig, ::PFDESolver::TResults>* allocator) {
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(0);
      static_cast<::grpc::internal::CallbackUnaryHandler< ::PFDESolver::TClientConfig, ::PFDESolver::TResults>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~WithCallbackMethod_RunTask() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status RunTask(::grpc::ServerContext* /*context*/, const ::PFDESolver::TClientConfig* /*request*/, ::PFDESolver::TResults* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* RunTask(
      ::grpc::CallbackServerContext* /*context*/, const ::PFDESolver::TClientConfig* /*request*/, ::PFDESolver::TResults* /*response*/)  { return nullptr; }
  };
  typedef WithCallbackMethod_RunTask<Service > CallbackService;
  typedef CallbackService ExperimentalCallbackService;
  template <class BaseClass>
  class WithGenericMethod_RunTask : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_RunTask() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_RunTask() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status RunTask(::grpc::ServerContext* /*context*/, const ::PFDESolver::TClientConfig* /*request*/, ::PFDESolver::TResults* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithRawMethod_RunTask : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_RunTask() {
      ::grpc::Service::MarkMethodRaw(0);
    }
    ~WithRawMethod_RunTask() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status RunTask(::grpc::ServerContext* /*context*/, const ::PFDESolver::TClientConfig* /*request*/, ::PFDESolver::TResults* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestRunTask(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawCallbackMethod_RunTask : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawCallbackMethod_RunTask() {
      ::grpc::Service::MarkMethodRawCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->RunTask(context, request, response); }));
    }
    ~WithRawCallbackMethod_RunTask() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status RunTask(::grpc::ServerContext* /*context*/, const ::PFDESolver::TClientConfig* /*request*/, ::PFDESolver::TResults* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* RunTask(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_RunTask : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_RunTask() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::internal::StreamedUnaryHandler<
          ::PFDESolver::TClientConfig, ::PFDESolver::TResults>(
            [this](::grpc::ServerContext* context,
                   ::grpc::ServerUnaryStreamer<
                     ::PFDESolver::TClientConfig, ::PFDESolver::TResults>* streamer) {
                       return this->StreamedRunTask(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_RunTask() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status RunTask(::grpc::ServerContext* /*context*/, const ::PFDESolver::TClientConfig* /*request*/, ::PFDESolver::TResults* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedRunTask(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::PFDESolver::TClientConfig,::PFDESolver::TResults>* server_unary_streamer) = 0;
  };
  typedef WithStreamedUnaryMethod_RunTask<Service > StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef WithStreamedUnaryMethod_RunTask<Service > StreamedService;
};

}  // namespace PFDESolverServer


#endif  // GRPC_server_2eproto__INCLUDED
