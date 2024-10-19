#include "service_impl.hpp"

#include <timer.hpp>
#include <logger.hpp>
#include <future>
// #include <boost/thread/latch.hpp>
#include <latch>

namespace NFDESolverService {

    void TFDESolverService::DoRunTask(const TClientConfig* request, TResults* response) {
        STACK_ASSERT(request != nullptr, "Request is nullptr");
        STACK_ASSERT(response != nullptr, "Response is nullptr");

        TParsedSolverConfig parsedSolverConfig = ParseClientConfig(*request);
        TSolverConfig solverConfig(parsedSolverConfig);
        const TSolvers solvers = ParseSolveMethods(*request, solverConfig);

        std::latch latch(solvers.size());

        // №1 Set Config to results
        AddConfig(solverConfig, *response);

        // №2 Set Results to results
        for (const auto& solver : solvers) {
            PFDESolver::TResult* pbResultDest = response->add_results();

            STACK_ASSERT(solver       != nullptr, "solver is nullptr");
            STACK_ASSERT(pbResultDest != nullptr, "pbResultDest is nullptr");

            boost::asio::post(ThreadPool, [this, pbResultDest, solver=solver.get(), &latch]() {
                NTimer::TTimer timer;
                SolveTask(*solver, *pbResultDest, true);
                INFO_LOG << "Method " << solver->Name() << " elapsed: " << timer.MilliSeconds().count() << "ms" << Endl;
                latch.count_down();
            });
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
        PFDESolver::TMatrix pbRealSolution((*config.RealSolution).ToProto());
        response.set_realsolutionname(config.RealSolutionName.value());
        response.mutable_realsolution()->Swap(&pbRealSolution);
    }

    template<std::convertible_to<std::string>... Args>
    auto TFDESolverService::GetFunction(std::string exp, const Args... args) { //NOLINT
        auto calculator = std::make_shared<ANTLRMathExpParser::TMathExpressionCalculator>(exp, std::vector<std::string>{std::string(args)...});

        return [calc = calculator, args...](auto... vars) -> f64 {
            static_assert(sizeof...(args) == sizeof...(vars));
            std::array<std::string, sizeof...(args)> varNames = {args...};
            std::array<f64, sizeof...(vars)> varValues = {vars...};
            for (usize i = 0; i < sizeof...(vars); i++) {
                calc->SetVar(varNames.at(i), varValues.at(i));
            }
            return calc->Calc();
        };
    }

    TParsedSolverConfig TFDESolverService::ParseClientConfig(const TClientConfig& config) {
        return {
            TSolverConfigBase::FromProto(config),
            /* .DiffusionCoefficient =*/ GetFunction(config.leftdiffusioncoefficient(), "x"),
            /* .DiffusionCoefficient =*/ GetFunction(config.rightdiffusioncoefficient(), "x"),
            /* .DemolitionCoefficient =*/ GetFunction(config.demolitioncoefficient(), "x"),
            /* .ZeroTimeState =*/ GetFunction(config.zerotimestate(), "x"),
            /* .SourceFunction =*/ GetFunction(config.sourcefunction(), "x", "t"),
            /* .LeftBoundState =*/ GetFunction(config.leftboundstate(), "t"),
            /* .RightBoundState =*/ GetFunction(config.rightboundstate(), "t"),
            /* .RealSolution =*/ config.has_realsolution() ? std::optional(GetFunction(config.realsolution(), "x", "t")) : std::nullopt,
        };
    }

    TFDESolverService::TSolvers TFDESolverService::ParseSolveMethods(const TClientConfig& config, const TSolverConfig& solverConfig) {
        TFDESolverService::TSolvers solvers;
        solvers.reserve(NEquationSolver::MethodsCount);

        const std::string& methods = config.solvemethods();
        DEBUG_LOG << "Methods: " << methods << Endl;

        auto view = methods
            | std::ranges::views::split(';')
            | std::ranges::views::transform([](auto &&rng) {
                    return std::string_view(&*rng.begin(), std::ranges::distance(rng));
            });
        
        for (const auto& svMethod : view) {
            std::string strMethod(svMethod);
            
            auto method = Methods.find(strMethod);
            if (method == Methods.end()) {
                WARNING_LOG << "No solver: " << strMethod << Endl;
                continue;
            }

            DEBUG_LOG << "Get solver: " << strMethod << Endl;
            auto& builder = method->second;

            STACK_ASSERT(builder->Number() < NEquationSolver::MethodsCount, std::format("Method: {} unimplemented", strMethod));
            solvers.push_back(builder->GetMethod(solverConfig));
        }

        return solvers;
    }
}  // namespace NFDESolverService