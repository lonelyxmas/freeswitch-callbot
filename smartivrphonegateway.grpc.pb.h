// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: smartivrphonegateway.proto
#ifndef GRPC_smartivrphonegateway_2eproto__INCLUDED
#define GRPC_smartivrphonegateway_2eproto__INCLUDED

#include "smartivrphonegateway.pb.h"

#include <functional>
#include <grpcpp/generic/async_generic_service.h>
#include <grpcpp/support/async_stream.h>
#include <grpcpp/support/async_unary_call.h>
#include <grpcpp/support/client_callback.h>
#include <grpcpp/client_context.h>
#include <grpcpp/completion_queue.h>
#include <grpcpp/support/message_allocator.h>
#include <grpcpp/support/method_handler.h>
#include <grpcpp/impl/codegen/proto_utils.h>
#include <grpcpp/impl/rpc_method.h>
#include <grpcpp/support/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/server_context.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/impl/codegen/status.h>
#include <grpcpp/support/stub_options.h>
#include <grpcpp/support/sync_stream.h>

namespace smartivrphonegateway {

class SmartIVRPhonegateway final {
 public:
  static constexpr char const* service_full_name() {
    return "smartivrphonegateway.SmartIVRPhonegateway";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    std::unique_ptr< ::grpc::ClientReaderWriterInterface< ::smartivrphonegateway::SmartIVRRequest, ::smartivrphonegateway::SmartIVRResponse>> CallToBot(::grpc::ClientContext* context) {
      return std::unique_ptr< ::grpc::ClientReaderWriterInterface< ::smartivrphonegateway::SmartIVRRequest, ::smartivrphonegateway::SmartIVRResponse>>(CallToBotRaw(context));
    }
    std::unique_ptr< ::grpc::ClientAsyncReaderWriterInterface< ::smartivrphonegateway::SmartIVRRequest, ::smartivrphonegateway::SmartIVRResponse>> AsyncCallToBot(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderWriterInterface< ::smartivrphonegateway::SmartIVRRequest, ::smartivrphonegateway::SmartIVRResponse>>(AsyncCallToBotRaw(context, cq, tag));
    }
    std::unique_ptr< ::grpc::ClientAsyncReaderWriterInterface< ::smartivrphonegateway::SmartIVRRequest, ::smartivrphonegateway::SmartIVRResponse>> PrepareAsyncCallToBot(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderWriterInterface< ::smartivrphonegateway::SmartIVRRequest, ::smartivrphonegateway::SmartIVRResponse>>(PrepareAsyncCallToBotRaw(context, cq));
    }
    class async_interface {
     public:
      virtual ~async_interface() {}
      virtual void CallToBot(::grpc::ClientContext* context, ::grpc::ClientBidiReactor< ::smartivrphonegateway::SmartIVRRequest,::smartivrphonegateway::SmartIVRResponse>* reactor) = 0;
    };
    typedef class async_interface experimental_async_interface;
    virtual class async_interface* async() { return nullptr; }
    class async_interface* experimental_async() { return async(); }
   private:
    virtual ::grpc::ClientReaderWriterInterface< ::smartivrphonegateway::SmartIVRRequest, ::smartivrphonegateway::SmartIVRResponse>* CallToBotRaw(::grpc::ClientContext* context) = 0;
    virtual ::grpc::ClientAsyncReaderWriterInterface< ::smartivrphonegateway::SmartIVRRequest, ::smartivrphonegateway::SmartIVRResponse>* AsyncCallToBotRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) = 0;
    virtual ::grpc::ClientAsyncReaderWriterInterface< ::smartivrphonegateway::SmartIVRRequest, ::smartivrphonegateway::SmartIVRResponse>* PrepareAsyncCallToBotRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());
    std::unique_ptr< ::grpc::ClientReaderWriter< ::smartivrphonegateway::SmartIVRRequest, ::smartivrphonegateway::SmartIVRResponse>> CallToBot(::grpc::ClientContext* context) {
      return std::unique_ptr< ::grpc::ClientReaderWriter< ::smartivrphonegateway::SmartIVRRequest, ::smartivrphonegateway::SmartIVRResponse>>(CallToBotRaw(context));
    }
    std::unique_ptr<  ::grpc::ClientAsyncReaderWriter< ::smartivrphonegateway::SmartIVRRequest, ::smartivrphonegateway::SmartIVRResponse>> AsyncCallToBot(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderWriter< ::smartivrphonegateway::SmartIVRRequest, ::smartivrphonegateway::SmartIVRResponse>>(AsyncCallToBotRaw(context, cq, tag));
    }
    std::unique_ptr<  ::grpc::ClientAsyncReaderWriter< ::smartivrphonegateway::SmartIVRRequest, ::smartivrphonegateway::SmartIVRResponse>> PrepareAsyncCallToBot(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderWriter< ::smartivrphonegateway::SmartIVRRequest, ::smartivrphonegateway::SmartIVRResponse>>(PrepareAsyncCallToBotRaw(context, cq));
    }
    class async final :
      public StubInterface::async_interface {
     public:
      void CallToBot(::grpc::ClientContext* context, ::grpc::ClientBidiReactor< ::smartivrphonegateway::SmartIVRRequest,::smartivrphonegateway::SmartIVRResponse>* reactor) override;
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
    ::grpc::ClientReaderWriter< ::smartivrphonegateway::SmartIVRRequest, ::smartivrphonegateway::SmartIVRResponse>* CallToBotRaw(::grpc::ClientContext* context) override;
    ::grpc::ClientAsyncReaderWriter< ::smartivrphonegateway::SmartIVRRequest, ::smartivrphonegateway::SmartIVRResponse>* AsyncCallToBotRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) override;
    ::grpc::ClientAsyncReaderWriter< ::smartivrphonegateway::SmartIVRRequest, ::smartivrphonegateway::SmartIVRResponse>* PrepareAsyncCallToBotRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_CallToBot_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status CallToBot(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::smartivrphonegateway::SmartIVRResponse, ::smartivrphonegateway::SmartIVRRequest>* stream);
  };
  template <class BaseClass>
  class WithAsyncMethod_CallToBot : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_CallToBot() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_CallToBot() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status CallToBot(::grpc::ServerContext* /*context*/, ::grpc::ServerReaderWriter< ::smartivrphonegateway::SmartIVRResponse, ::smartivrphonegateway::SmartIVRRequest>* /*stream*/)  override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestCallToBot(::grpc::ServerContext* context, ::grpc::ServerAsyncReaderWriter< ::smartivrphonegateway::SmartIVRResponse, ::smartivrphonegateway::SmartIVRRequest>* stream, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncBidiStreaming(0, context, stream, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_CallToBot<Service > AsyncService;
  template <class BaseClass>
  class WithCallbackMethod_CallToBot : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithCallbackMethod_CallToBot() {
      ::grpc::Service::MarkMethodCallback(0,
          new ::grpc::internal::CallbackBidiHandler< ::smartivrphonegateway::SmartIVRRequest, ::smartivrphonegateway::SmartIVRResponse>(
            [this](
                   ::grpc::CallbackServerContext* context) { return this->CallToBot(context); }));
    }
    ~WithCallbackMethod_CallToBot() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status CallToBot(::grpc::ServerContext* /*context*/, ::grpc::ServerReaderWriter< ::smartivrphonegateway::SmartIVRResponse, ::smartivrphonegateway::SmartIVRRequest>* /*stream*/)  override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerBidiReactor< ::smartivrphonegateway::SmartIVRRequest, ::smartivrphonegateway::SmartIVRResponse>* CallToBot(
      ::grpc::CallbackServerContext* /*context*/)
      { return nullptr; }
  };
  typedef WithCallbackMethod_CallToBot<Service > CallbackService;
  typedef CallbackService ExperimentalCallbackService;
  template <class BaseClass>
  class WithGenericMethod_CallToBot : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_CallToBot() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_CallToBot() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status CallToBot(::grpc::ServerContext* /*context*/, ::grpc::ServerReaderWriter< ::smartivrphonegateway::SmartIVRResponse, ::smartivrphonegateway::SmartIVRRequest>* /*stream*/)  override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithRawMethod_CallToBot : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_CallToBot() {
      ::grpc::Service::MarkMethodRaw(0);
    }
    ~WithRawMethod_CallToBot() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status CallToBot(::grpc::ServerContext* /*context*/, ::grpc::ServerReaderWriter< ::smartivrphonegateway::SmartIVRResponse, ::smartivrphonegateway::SmartIVRRequest>* /*stream*/)  override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestCallToBot(::grpc::ServerContext* context, ::grpc::ServerAsyncReaderWriter< ::grpc::ByteBuffer, ::grpc::ByteBuffer>* stream, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncBidiStreaming(0, context, stream, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawCallbackMethod_CallToBot : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawCallbackMethod_CallToBot() {
      ::grpc::Service::MarkMethodRawCallback(0,
          new ::grpc::internal::CallbackBidiHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
                   ::grpc::CallbackServerContext* context) { return this->CallToBot(context); }));
    }
    ~WithRawCallbackMethod_CallToBot() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status CallToBot(::grpc::ServerContext* /*context*/, ::grpc::ServerReaderWriter< ::smartivrphonegateway::SmartIVRResponse, ::smartivrphonegateway::SmartIVRRequest>* /*stream*/)  override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerBidiReactor< ::grpc::ByteBuffer, ::grpc::ByteBuffer>* CallToBot(
      ::grpc::CallbackServerContext* /*context*/)
      { return nullptr; }
  };
  typedef Service StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef Service StreamedService;
};

}  // namespace smartivrphonegateway


#endif  // GRPC_smartivrphonegateway_2eproto__INCLUDED