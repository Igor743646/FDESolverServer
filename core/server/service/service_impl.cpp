#include "service_impl.hpp"

#include <timer.hpp>
#include <logger.hpp>
#include <future>
#include <boost/thread/latch.hpp>

namespace NFDESolverService {

    void TFDESolverService::DoRunTask(const TClientConfig* request, TResults* response) {
        STACK_ASSERT(request != nullptr, "Request is nullptr");
        STACK_ASSERT(response != nullptr, "Response is nullptr");

        TParsedSolverConfig parsedSolverConfig = ParseClientConfig(*request);
        TSolverConfig solverConfig(parsedSolverConfig);
        TSolvers solvers = ParseSolveMethods(*request, solverConfig);

        boost::latch latch(solvers.size());

        // №1 Set Config to results
        AddConfig(solverConfig, *response);

        // №2 Set Results to results
        for (auto& solver : solvers) {
            PFDESolver::TResult* pbResultDest = response->add_results();
            if (solver && (pbResultDest != nullptr)) {
                boost::asio::post(ThreadPool, [this, pbResultDest, solver, &latch]() {
                    NTimer::TTimer timer;
                    SolveTask(*solver, *pbResultDest, true);
                    INFO_LOG << "Method " << solver->Name() << " elapsed: " << timer.MilliSeconds().count() << "ms" << Endl;
                    latch.count_down();
                });
            } else {
                STACK_ASSERT(pbResultDest != nullptr, "pbResultDest is nullptr");
            }
        }
        

        // №3, 4 Set Realsolution and its name
        AddRealSolution(solverConfig, *response);
        
        latch.wait();
        DEBUG_LOG << "Finish DoRunTask" << Endl;
    }

    void TFDESolverService::AddConfig(const TSolverConfig& config, TResults& response) {
        PFDESolver::TSolverConfig pbConfig = config.ToProto();
        response.mutable_task()->Swap(&pbConfig);
    }
    
    void TFDESolverService::SolveTask(IEquationSolver& solver, 
                                      PFDESolver::TResult& response, bool saveMeta) 
    {
        DEBUG_LOG << "Start solve method: " << solver.Name() << Endl;
        IEquationSolver::TResult result = solver.Solve(saveMeta);

        PFDESolver::TResult pbResultSource = result.ToProto();
        response.Swap(&pbResultSource);
    }

    void TFDESolverService::AddRealSolution(const TSolverConfig& config, TResults& response) {
        if (!config.RealSolution.has_value()) {
            DEBUG_LOG << "No real solution" << Endl;
            return;
        }

        INFO_LOG << "Adding real solution: " << config.RealSolutionName.value() << Endl;
        const usize n = config.SpaceCount;
        const usize k = config.TimeCount;

        auto realSolution = NLinalg::TMatrix(k + 1, n + 1);
        for (usize j = 0; j <= k; j++) {
            for (usize i = 0; i <= n; i++) {
                if (i == n) {
                    realSolution[j][i] = config.RightBoundState[j];
                } else {
                    realSolution[j][i] = (*config.RealSolution)[j][i];
                }
            }
        }

        PFDESolver::TMatrix pbRealSolution(realSolution.ToProto());
        response.set_realsolutionname(config.RealSolutionName.value());
        response.mutable_realsolution()->Swap(&pbRealSolution);
    }

    auto TFDESolverService::GetFunction1(std::string expression, std::string varName1) {
        auto calculator = make_shared<ANTLRMathExpParser::MathExpressionCalculator>(expression, std::vector{varName1});
        return [calc=calculator, varName1](f64 varValue) -> f64 {
            calc->SetVar(varName1, varValue);
            return calc->Calc();
        };
    }

    auto TFDESolverService::GetFunction2(std::string expression, std::string varName1, std::string varName2) {
        auto calculator = make_shared<ANTLRMathExpParser::MathExpressionCalculator>(expression, std::vector{varName1, varName2});
        return [calc=calculator, varName1, varName2](f64 varValue1, f64 varValue2) -> f64 {
            calc->SetVar(varName1, varValue1);
            calc->SetVar(varName2, varValue2);
            return calc->Calc();
        };
    }

    TParsedSolverConfig TFDESolverService::ParseClientConfig(const TClientConfig& config) {
        return {
            TSolverConfigBase {
                .SpaceCount = static_cast<usize>((config.rightbound() - config.leftbound()) / config.spacestep()), 
                .TimeCount = static_cast<usize>(config.maxtime() / config.timestep()), 
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
                .BordersAvailable = config.bordersavailable(),
                .StochasticIterationCount = config.has_stochasticiterationcount() ? config.stochasticiterationcount() : 1000, 
                .RealSolutionName = config.has_realsolutionname() ? std::optional(config.realsolutionname()) : std::nullopt,
            },
            /* .DiffusionCoefficient =*/ GetFunction1(config.diffusioncoefficient(), "x"),
            /* .DemolitionCoefficient =*/ GetFunction1(config.demolitioncoefficient(), "x"),
            /* .ZeroTimeState =*/ GetFunction1(config.zerotimestate(), "x"),
            /* .SourceFunction =*/ GetFunction2(config.sourcefunction(), "x", "t"),
            /* .LeftBoundState =*/ GetFunction1(config.leftboundstate(), "t"),
            /* .RightBoundState =*/ GetFunction1(config.rightboundstate(), "t"),
            /* .RealSolution =*/ config.has_realsolution() ? std::optional(GetFunction2(config.realsolution(), "x", "t")) : std::nullopt,
        };
    }

    TFDESolverService::TSolvers TFDESolverService::ParseSolveMethods(const TClientConfig& config, const TSolverConfig& solverConfig) {
        TFDESolverService::TSolvers solvers;
        const std::string& methods = config.solvemethods();
        DEBUG_LOG << "Methods: " << methods << Endl;

        auto view = methods
            | std::ranges::views::split(';')
            | std::ranges::views::transform([](auto &&rng) {
                    return std::string_view(&*rng.begin(), std::ranges::distance(rng));
            });
        
        for (const auto& svMethod : view) {
            std::string strMethod = std::string(svMethod);
            
            auto method = Methods.find(strMethod);
            if (method != Methods.end()) {
                DEBUG_LOG << "Get solver: " << strMethod << Endl;

                if (method->second->Number() < solvers.size()) {
                    solvers[method->second->Number()] = method->second->GetMethod(solverConfig);
                } else {
                    ERROR_LOG << "Method: " << strMethod << " unimplemented" << Endl;
                    NStackTracer::TStackTracer::ThrowWithMessage("Method " + strMethod + " unimplemented");
                }
            } else {
                INFO_LOG << "No solver: " << strMethod << Endl;
            }
        }

        return solvers;
    }
}