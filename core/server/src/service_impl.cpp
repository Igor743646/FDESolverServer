#include "service_impl.hpp"

#include <timer.hpp>

namespace NFDESolverService {

    void TFDESolverService::DoRunTask(const TClientConfig* request, TResults* response) {
        TParsedSolverConfig parsedSolverConfig = ParseClientConfig(*request);
        TSolverConfig solverConfig(parsedSolverConfig);
        TSolvers solvers = ParseSolveMethods(*request, solverConfig);

        for (auto& solver : solvers) {
            if (solver) {
                NTimer::TTimer timer;
                SolveTask(*solver, *response, true);

                INFO_LOG << std::format("Method {} elapsed: {}", solver->Name(), timer.MilliSeconds()) << Endl;
            }
        }

        if (solverConfig.RealSolution.has_value()) {
            AddRealSolution(solverConfig, *response);
        }
    }
    
    void TFDESolverService::SolveTask(IEquationSolver& solver, 
                                      TResults& response, bool saveMeta) 
    {
        DEBUG_LOG << "Start solve method: " << solver.Name() << Endl;
        IEquationSolver::TResult result = solver.Solve(saveMeta);

        PFDESolver::TResult* pbResultDest = response.add_results();
        PFDESolver::TResult pbResultSource = result.ToProto();
        pbResultDest->Swap(&pbResultSource);

        const auto& config = solver.GetConfig();
        PFDESolver::TSolverConfig pbConfig = config.ToProto();
        response.mutable_task()->Swap(&pbConfig);
    }

    void TFDESolverService::AddRealSolution(const TSolverConfig& config, TResults& response) {
        INFO_LOG << "Adding real solution" << Endl;
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

        PFDESolver::TMatrix pbRealSolution(std::move(realSolution.ToProto()));
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
        
        for (const auto strMethod : std::views::split(methods, ';')) {
            auto method = Methods.find(std::string(std::string_view(strMethod)));
            if (method != Methods.end()) {
                DEBUG_LOG << "Get solver: " << std::string_view(strMethod) << Endl;

                if (method->second->Number() < solvers.size()) {
                    solvers[method->second->Number()] = method->second->GetMethod(solverConfig);
                } else {
                    ERROR_LOG << "Method: " << std::string_view(strMethod) << " unimplemented" << Endl;
                    NStackTracer::TStackTracer::Throw<std::runtime_error>(
                        std::format("Method {} unimplemented", std::string_view(strMethod)));
                }
            } else {
                INFO_LOG << "No solver: " << std::string_view(strMethod) << Endl;
            }
        }

        return solvers;
    }
}