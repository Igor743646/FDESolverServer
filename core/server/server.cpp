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

    auto GetFunction1(std::string expression, std::string var_name) {
        auto calculator = make_shared<ANTLRMathExpParser::MathExpressionCalculator>(expression, std::vector{var_name});
        return [calc=calculator, var_name](f64 var_value) -> f64 {
            calc->SetVar(var_name, var_value);
            return calc->Calc();
        };
    }

    auto GetFunction2(std::string expression, std::string var_name1, std::string var_name2) {
        auto calculator = make_shared<ANTLRMathExpParser::MathExpressionCalculator>(expression, std::vector{var_name1, var_name2});
        return [calc=calculator, var_name1, var_name2](f64 var_value1, f64 var_value2) -> f64 {
            calc->SetVar(var_name1, var_value1);
            calc->SetVar(var_name2, var_value2);
            return calc->Calc();
        };
    }

    NEquationSolver::TSolverConfig ParseClientConfig(const TClientConfig& config) {
        return {
            .SpaceCount = config.has_spacecount() ? config.spacecount() : 0, 
            .TimeCount = config.has_timecount() ? config.timecount() : 0, 
            .LeftBound = config.leftbound(), 
            .RightBound = config.rightbound(),
            .MaxTime = config.maxtime(),
            .Alpha = config.alpha(), 
            .Gamma = config.gamma(),
            .SpaceStep = config.spacestep(), 
            .TimeStep = config.timestep(),
            .Beta = config.beta(),
            .AlphaLeft = config.alphaleft(), 
            .BetaLeft = config.betaleft(),
            .AlphaRight = config.alpharight(), 
            .BetaRight = config.betaright(),
            .DiffusionCoefficient = GetFunction1(config.diffusioncoefficient(), "x"),
            .DemolitionCoefficient = GetFunction1(config.demolitioncoefficient(), "x"),
            .ZeroTimeState = GetFunction1(config.zerotimestate(), "x"),
            .SourceFunction = GetFunction2(config.sourcefunction(), "x", "t"),
            .LeftBoundState = GetFunction1(config.leftboundstate(), "t"),
            .RightBoundState = GetFunction1(config.rightboundstate(), "t"),
            .BordersAvailable = config.bordersavailable(),
            .StochasticIterationCount = config.has_stochasticiterationcount() ? config.stochasticiterationcount() : 1000, 
            .RealSolutionName = config.has_realsolutionname() ? std::optional(config.realsolutionname()) : std::nullopt,
            .RealSolution = config.has_realsolution() ? std::optional(GetFunction2(config.realsolution(), "x", "t")) : std::nullopt,
        };
    }
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
