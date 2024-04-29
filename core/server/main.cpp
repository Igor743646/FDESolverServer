#include <config.pb.h>
#include <math_expression_calculator.h>
#include <server.grpc.pb.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

using namespace ::grpc;
using namespace ::PFDESolverServer;
using namespace ::PFDESolver;

class TFDESolverServerImpl final : public PFDESolverServer::TFDESolverServer::Service {
public:

    Status RunTask(ServerContext* context, const TClientConfig* request, TResult* response) override {
        
        return Status::OK;
    }
};

void RunServer() {
  std::string server_address("localhost:50051");
  TFDESolverServerImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

int main(int, char**) {
  RunServer();
  return 0;
}
