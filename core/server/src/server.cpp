#include "server_impl.hpp"

#include <grpcpp/server_builder.h>

void Init() {
    gpr_set_log_verbosity(gpr_log_severity::GPR_LOG_SEVERITY_DEBUG);
}

void RunServer() {
    std::string serverAddress("[::]:50051");
    TFDESolverServerImpl service;
    grpc::ServerBuilder builder;
    grpc::ResourceQuota resourceQuota;
    resourceQuota.SetMaxThreads(8);

    builder.SetResourceQuota(resourceQuota);
    builder.AddListeningPort(serverAddress, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

    if (server) {
        gpr_log(GPR_DEBUG, "Server listening on address: %s", serverAddress.c_str());
        server->Wait();
    } else {
        gpr_log(GPR_DEBUG, "Can not run server on address: %s", serverAddress.c_str());
    }
}

int main(int, char**) {
    Init();
    RunServer();
    return 0;
}
