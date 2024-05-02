#include <math_expression_calculator.h>
#include <config.pb.h>
#include <result.pb.h>
#include <matrix.pb.h>
#include <server.grpc.pb.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpc/support/log.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include <mfdes.hpp>
#include <sfdes.hpp>

using namespace ::grpc;
using namespace ::PFDESolverServer;
using namespace ::PFDESolver;

auto CalculateTime(auto callback) {
    auto start = std::chrono::system_clock::now();
    auto result = callback();
    auto stop = std::chrono::system_clock::now();

    f64 time_ms = std::chrono::duration<f64, std::milli>(stop - start).count();

    INFO_LOG << "Calculated time: " << time_ms << Endl;

    return result;
}

void SolveTaskAndSave(NEquationSolver::IEquationSolver& solver, 
                      PFDESolver::TResults& results, 
                      bool calculateTime = true, 
                      bool saveMeta = true) 
{
    NEquationSolver::IEquationSolver::TResult result = calculateTime ? CalculateTime([&solver, saveMeta](){
        return solver.Solve(saveMeta);
    }) : solver.Solve(saveMeta);

    auto pbResult = results.add_results();
    pbResult->Swap(new PFDESolver::TResult(result.ToProto()));

    const auto& config = solver.GetConfig();
    results.mutable_task()->Swap(new PFDESolver::TSolverConfig(config.ToProto()));

    if (saveMeta && config.RealSolution.has_value()) {
        INFO_LOG << "Adding real solution" << Endl;
        const usize n = config.SpaceCount;
        const usize k = config.TimeCount;

        results.set_realsolutionname(*config.RealSolutionName);
        auto RealSolution = NLinalg::TMatrix(k + 1, n + 1);
        for (usize j = 0; j <= k; j++) {
            for (usize i = 0; i <= n; i++) {
                if (i == n) {
                    RealSolution[j][i] = config.RightBoundState(static_cast<f64>(j) * config.TimeStep);
                } else {
                    RealSolution[j][i] = (*config.RealSolution)(config.LeftBound + static_cast<f64>(i) * config.SpaceStep, static_cast<f64>(j) * config.TimeStep);
                }
            }
        }

        PFDESolver::TMatrix realSolution(std::move(RealSolution.ToProto()));
        results.mutable_realsolution()->Swap(&realSolution);
    }
}

class TFDESolverServerImpl final : public PFDESolverServer::TFDESolverServer::Service {
public:

    Status RunTask(ServerContext* context, const TClientConfig* request, TResults* response) override {
        auto start = std::chrono::system_clock::now();

        try {
            NEquationSolver::TSolverConfig solverConfig = ParseClientConfig(*request);
            NEquationSolver::TMatrixFDES solver(solverConfig);
            SolveTaskAndSave(solver, *response, false, true);
        } catch (...) {
            gpr_log(GPR_ERROR, "Error while running task");
        }

        auto stop = std::chrono::system_clock::now();
        f64 workTime = std::chrono::duration<f64, std::milli>(stop - start).count();

        context->AddTrailingMetadata("work-time", std::to_string(workTime));
        context->AddTrailingMetadata("version", "0.1");
        
        return Status::OK;
    }

private:

    #define GetFunction1(var_name, function_name) [&config](f64 var_name){ \
                static antlrcpptest::MathExpressionCalculator calculator(config.function_name(), {#var_name}); \
                calculator.SetVar(#var_name, var_name); \
            return calculator.Calc();}
    #define GetFunction2(var_name1, var_name2, function_name) [&config](f64 var_name1, f64 var_name2){ \
                static antlrcpptest::MathExpressionCalculator calculator(config.function_name(), {#var_name1, #var_name2}); \
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
        solverConfig.DiffusionCoefficient = GetFunction1(x, diffusioncoefficient);
        solverConfig.DemolitionCoefficient = GetFunction1(x, demolitioncoefficient);
        solverConfig.ZeroTimeState = GetFunction1(x, zerotimestate);
        solverConfig.SourceFunction = GetFunction2(x, t, sourcefunction);
        solverConfig.LeftBoundState = GetFunction1(t, leftboundstate);
        solverConfig.RightBoundState = GetFunction1(t, rightboundstate);
        solverConfig.BordersAvailable = config.bordersavailable();
        solverConfig.StochasticIterationCount = config.has_stochasticiterationcount() ? config.stochasticiterationcount() : 1000; 
        solverConfig.RealSolutionName = config.has_realsolutionname() ? std::optional(config.realsolutionname()) : std::nullopt;
        solverConfig.RealSolution = config.has_realsolution() ? std::optional(GetFunction2(x, t, realsolution)) : std::nullopt;

        return solverConfig;
    }
    #undef GetFunction1
    #undef GetFunction2
};

void Init() {
    gpr_set_log_verbosity(gpr_log_severity::GPR_LOG_SEVERITY_DEBUG);
}

void RunServer() {
    std::string server_address("localhost:50051");
    TFDESolverServerImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());

    if (server) {
        gpr_log(GPR_DEBUG, "Server listening on address: %s", server_address.c_str());
        server->Wait();
    } else {
        gpr_log(GPR_DEBUG, "Can not run server on address: %s", server_address);
    }
}

int main(int, char**) {
    Init();
    RunServer();
    return 0;
}
