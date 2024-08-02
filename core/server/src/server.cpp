#include "server_impl.hpp"

#include <grpcpp/server_builder.h>
#include <utils.hpp>

void Init() {
    std::set_terminate(NUtils::TerminateWithStack);
#ifndef NDEBUG
    gpr_set_log_verbosity(gpr_log_severity::GPR_LOG_SEVERITY_DEBUG);
    NLogger::TLogHelper::SetLogLevel(NLogger::TLogHelper::TLevel::lvDEBUG);
#else 
    gpr_set_log_verbosity(gpr_log_severity::GPR_LOG_SEVERITY_INFO);
#endif
}

void RunServer() {
    std::string serverAddress("[::]:50051");
    TFDESolverServerImpl service;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(serverAddress, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

    if (server) {
        INFO_LOG << std::format("Server listening on address: {}\n", serverAddress);
        server->Wait();
    } else {
        INFO_LOG << std::format("Can not run server on address: {}\n", serverAddress);
    }
}

int main(int, char**) {
    Init();
    RunServer();
    return 0;
}
