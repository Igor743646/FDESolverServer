#include "server_impl.hpp"


TFDESolverServerImpl::TStatus TFDESolverServerImpl::RunTask(TServerContext* context, const TClientConfig* request, TResults* response) {
    gpr_log(GPR_INFO, "Start task");
    auto start = std::chrono::system_clock::now();

    try {
        DoRunTask(context, request, response);
        gpr_log(GPR_INFO, "End task");
    } catch (const std::exception& e) {
        gpr_log(GPR_ERROR, "Error:");
        NStackTracer::TStackTracer::CatchAndPrintStack(e);
        return TStatus(::grpc::StatusCode::ABORTED, e.what());
    } catch (...) {
        gpr_log(GPR_ERROR, "Error: unknown error");
        return TStatus(::grpc::StatusCode::UNKNOWN, "Unexpected exception");
    }

    auto stop = std::chrono::system_clock::now();
    f64 workTime = std::chrono::duration<f64, std::milli>(stop - start).count();

    context->AddTrailingMetadata("work-time", std::to_string(workTime));
    context->AddTrailingMetadata("version", "0.1");
    
    return TStatus::OK;
}

void TFDESolverServerImpl::DoRunTask(TServerContext* context, const TClientConfig* request, TResults* response) {
    NEquationSolver::TSolverConfig solverConfig = ParseClientConfig(*request);
    TSolvers solvers = ParseSolveMethods(*request, solverConfig);
    for (auto& solver : solvers) {
        gpr_log(GPR_INFO, solver->Name().c_str());
        SolveTaskAndSave(*solver, *response, true);
    }
}

void TFDESolverServerImpl::SolveTaskAndSave(NEquationSolver::IEquationSolver& solver, 
                                            PFDESolver::TResults& results, bool saveMeta) 
{
    NEquationSolver::IEquationSolver::TResult result = solver.Solve(saveMeta);

    auto pbResult = results.add_results();
    pbResult->Swap(new PFDESolver::TResult(result.ToProto()));

    const auto& config = solver.GetConfig();
    results.mutable_task()->Swap(new PFDESolver::TSolverConfig(config.ToProto()));

    if (saveMeta && config.RealSolution.has_value()) {
        INFO_LOG << "Adding real solution" << Endl;
        const usize n = config.SpaceCount;
        const usize k = config.TimeCount;

        results.set_realsolutionname(*config.RealSolutionName);
        auto realSolution = NLinalg::TMatrix(k + 1, n + 1);
        for (usize j = 0; j <= k; j++) {
            for (usize i = 0; i <= n; i++) {
                if (i == n) {
                    realSolution[j][i] = config.RightBoundState(static_cast<f64>(j) * config.TimeStep);
                } else {
                    realSolution[j][i] = (*config.RealSolution)(config.LeftBound + static_cast<f64>(i) * config.SpaceStep, static_cast<f64>(j) * config.TimeStep);
                }
            }
        }

        PFDESolver::TMatrix pbRealSolution(std::move(realSolution.ToProto()));
        results.mutable_realsolution()->Swap(&pbRealSolution);
    }
}


auto TFDESolverServerImpl::GetFunction1(std::string expression, std::string varName1) {
    auto calculator = make_shared<ANTLRMathExpParser::MathExpressionCalculator>(expression, std::vector{varName1});
    return [calc=calculator, varName1](f64 varValue) -> f64 {
        calc->SetVar(varName1, varValue);
        return calc->Calc();
    };
}

auto TFDESolverServerImpl::GetFunction2(std::string expression, std::string varName1, std::string varName2) {
    auto calculator = make_shared<ANTLRMathExpParser::MathExpressionCalculator>(expression, std::vector{varName1, varName2});
    return [calc=calculator, varName1, varName2](f64 varValue1, f64 varValue2) -> f64 {
        calc->SetVar(varName1, varValue1);
        calc->SetVar(varName2, varValue2);
        return calc->Calc();
    };
}

NEquationSolver::TSolverConfig TFDESolverServerImpl::ParseClientConfig(const TClientConfig& config) {
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

TFDESolverServerImpl::TSolvers TFDESolverServerImpl::ParseSolveMethods(const TClientConfig&, const NEquationSolver::TSolverConfig& solverConfig) {
    // Unimplemented
    TFDESolverServerImpl::TSolvers solvers;

    solvers[0] = std::make_unique<NEquationSolver::TMatrixFDES<NEquationSolver::TMFDESRule>>(solverConfig);
    solvers[1] = std::make_unique<NEquationSolver::TMatrixFDES<NEquationSolver::TRLFDESRule>>(solverConfig);
    solvers[2] = std::make_unique<NEquationSolver::TStochasticFDES<NEquationSolver::TMFDESRule>>(solverConfig);
    solvers[3] = std::make_unique<NEquationSolver::TStochasticFDES<NEquationSolver::TRLFDESRule>>(solverConfig);

    return solvers;
}