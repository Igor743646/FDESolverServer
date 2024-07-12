#include "service_impl.hpp"
#include <server.grpc.pb.h>

class TFDESolverServerImpl final : public ::PFDESolverServer::TFDESolverServer::Service, 
                                   public ::NFDESolverService::TFDESolverService {
private:

    using TStatus = ::grpc::Status;
    using TServerContext = ::grpc::ServerContext;
    using TClientConfig = ::PFDESolver::TClientConfig;
    using TResults = ::PFDESolver::TResults;

public:
    TStatus RunTask(TServerContext*, const TClientConfig*, TResults*) override final;
};
