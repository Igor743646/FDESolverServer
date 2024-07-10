#include <math_expression_calculator.hpp>
#include <server.grpc.pb.h>
#include <mfdes.hpp>
#include <sfdes.hpp>

class TFDESolverServerImpl final : public ::PFDESolverServer::TFDESolverServer::Service {
private:

    using TStatus = ::grpc::Status;
    using TServerContext = ::grpc::ServerContext;
    using TClientConfig = ::PFDESolver::TClientConfig;
    using TResults = ::PFDESolver::TResults;
    using TSolvers = std::array<std::unique_ptr<NEquationSolver::IEquationSolver>, NEquationSolver::MethodsCount>;

public:

    TStatus RunTask(TServerContext*, const TClientConfig*, TResults*) override final;

private:

    virtual void DoRunTask(TServerContext*, const TClientConfig*, TResults*);

    void SolveTaskAndSave(NEquationSolver::IEquationSolver&, 
                          PFDESolver::TResults&,
                          bool = true);

    auto GetFunction1(std::string, std::string);
    auto GetFunction2(std::string, std::string, std::string);
    NEquationSolver::TSolverConfig ParseClientConfig(const TClientConfig&);
    TSolvers ParseSolveMethods(const TClientConfig&, const NEquationSolver::TSolverConfig&);
};
