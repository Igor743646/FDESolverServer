#include <math_expression_calculator.h>
#include <config.pb.h>
#include <server.grpc.pb.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include <mfdes.hpp>

using namespace ::grpc;
using namespace ::PFDESolverServer;
using namespace ::PFDESolver;

class TFDESolverServerImpl final : public PFDESolverServer::TFDESolverServer::Service {
public:

    Status RunTask(ServerContext* context, const TClientConfig* request, TResults* response) override {
        
        try {
            NEquationSolver::TSolverConfig solverConfig = ParseClientConfig(*request);
            std::cout << solverConfig.DiffusionCoefficient(0.5) << std::endl;
        } catch (...) {
            std::cout << "Error" << std::endl;
        }
        
        return Status::OK;
    }

private:

    #define GetFunction1(var_name, function_name) [&config](f64 var_name){ \
                std::cout << "Create calculator" << std::endl; \
                antlrcpptest::MathExpressionCalculator calculator(config.function_name(), {#var_name}); \
                std::cout << "Calculator created" << std::endl; \
                calculator.SetVar(#var_name, var_name); \
                std::cout << "Calculate" << std::endl; \
            return calculator.Calc();}
    #define GetFunction2(var_name1, var_name2, function_name) [&config](f64 var_name1, f64 var_name2){ \
                antlrcpptest::MathExpressionCalculator calculator(config.function_name(), {#var_name1, #var_name2}); \
                calculator.SetVar(#var_name1, var_name1); \
                calculator.SetVar(#var_name2, var_name2); \
            return calculator.Calc();}

    NEquationSolver::TSolverConfig ParseClientConfig(const TClientConfig& config) {
        NEquationSolver::TSolverConfig solverConfig;

        solverConfig.SpaceCount = config.has_spacecount() ? config.spacecount() : 0; 
        solverConfig.TimeCount = config.has_timecount() ? config.timecount() : 0; 
        solverConfig.LeftBound = config.leftbound(); 
        solverConfig.RightBound = config.rightbound();
        solverConfig.MaxTime = config.maxtime();
        solverConfig.Alpha = config.alpha(); 
        solverConfig.Gamma = config.gamma();
        solverConfig.SpaceStep = config.spacestep(); 
        solverConfig.TimeStep = config.timestep();
        solverConfig.Beta = config.beta();
        solverConfig.AlphaLeft = config.alphaleft(); 
        solverConfig.BetaLeft = config.betaleft();
        solverConfig.AlphaRight = config.alpharight(); 
        solverConfig.BetaRight = config.betaright();
        std::cout << "Parse DiffusionCoefficient" << std::endl;
        solverConfig.DiffusionCoefficient = GetFunction1(x, diffusioncoefficient);
        solverConfig.DemolitionCoefficient = GetFunction1(x, demolitioncoefficient);
        solverConfig.ZeroTimeState = GetFunction1(x, zerotimestate);
        solverConfig.SourceFunction = GetFunction2(x, t, sourcefunction);
        solverConfig.LeftBoundState = GetFunction1(t, leftboundstate);
        solverConfig.RightBoundState = GetFunction1(t, rightboundstate);
        solverConfig.BordersAvailable = config.bordersavailable();
        solverConfig.StochasticIterationCount = config.has_stochasticiterationcount() ? config.stochasticiterationcount() : 1000; 
        solverConfig.RealSolutionName = config.has_realsolutionname() ? std::optional(config.realsolutionname()) : std::nullopt;
        solverConfig.RealSolution = config.has_realsolution() ? std::optional(GetFunction2(x, t, realsolutionname)) : std::nullopt;

        std::cout << "Parsed all" << std::endl;

        return solverConfig;
    }
    #undef GetFunction1
    #undef GetFunction2
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
