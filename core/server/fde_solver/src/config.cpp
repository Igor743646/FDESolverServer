#include "config.hpp"

#include <config.pb.h>

namespace NEquationSolver {

    TSolverConfigBase TParsedSolverConfig::Base() const {
        return TSolverConfigBase {
            .SpaceCount                 = SpaceCount, 
            .TimeCount                  = TimeCount, 
            .LeftBound                  = LeftBound,  
            .RightBound                 = RightBound, 
            .MaxTime                    = MaxTime, 
            .Alpha                      = Alpha,  
            .Gamma                      = Gamma, 
            .SpaceStep                  = SpaceStep,  
            .TimeStep                   = TimeStep, 
            .Beta                       = Beta, 
            .AlphaLeft                  = AlphaLeft,  
            .BetaLeft                   = BetaLeft, 
            .AlphaRight                 = AlphaRight,  
            .BetaRight                  = BetaRight, 
            .BordersAvailable           = BordersAvailable, 
            .StochasticIterationCount   = StochasticIterationCount,
            .RealSolutionName           = RealSolutionName,
        };
    }

    TSolverConfig::TSolverConfig(const TParsedSolverConfig& parsedConfig) : TSolverConfigBase(parsedConfig.Base()) {
        const usize n = parsedConfig.SpaceCount + 1;
        const usize k = parsedConfig.TimeCount + 1;
        LeftDiffusionCoefficient.resize(n);
        RightDiffusionCoefficient.resize(n);
        DemolitionCoefficient.resize(n);
        ZeroTimeState.resize(n);
        SourceFunction = NLinalg::TMatrix(k, n);
        LeftBoundState.resize(k);
        RightBoundState.resize(k);

        for (usize i = 0; i < n; i++) {
            f64 x = parsedConfig.LeftBound + static_cast<f64>(i) * parsedConfig.SpaceStep;
            
            LeftDiffusionCoefficient[i] = parsedConfig.LeftDiffusionCoefficient(x);
            RightDiffusionCoefficient[i] = parsedConfig.RightDiffusionCoefficient(x);
            DemolitionCoefficient[i] = parsedConfig.DemolitionCoefficient(x);
            ZeroTimeState[i] = parsedConfig.ZeroTimeState(x);
        }

        for (usize j = 0; j < k; j++) {
            f64 t = parsedConfig.TimeStep * static_cast<f64>(j);
            LeftBoundState[j] = parsedConfig.LeftBoundState(t);
            RightBoundState[j] = parsedConfig.RightBoundState(t);
            
            for (usize i = 0; i < n; i++) {
                f64 x = parsedConfig.LeftBound + static_cast<f64>(i) * parsedConfig.SpaceStep;
                SourceFunction[j][i] = parsedConfig.SourceFunction(x, t);
            }
        }

        if (parsedConfig.RealSolution.has_value()) {
            RealSolution = NLinalg::TMatrix(k, n);
            for (usize j = 0; j < k; j++) {
                f64 t = parsedConfig.TimeStep * static_cast<f64>(j);
                (*RealSolution)[j][0] = LeftBoundState[j];
                for (usize i = 0; i < n; i++) {
                    f64 x = parsedConfig.LeftBound + static_cast<f64>(i) * parsedConfig.SpaceStep;
                    (*RealSolution)[j][i] = parsedConfig.RealSolution.value()(x, t);
                }
                (*RealSolution)[j][parsedConfig.SpaceCount] = RightBoundState[j];
            }
        }
    }

    PFDESolver::TSolverConfig TSolverConfig::ToProto() const {
        PFDESolver::TSolverConfig config;

        config.set_spacecount(static_cast<usize>((RightBound - LeftBound) / SpaceStep));
        config.set_timecount(static_cast<usize>(MaxTime / TimeStep));
        config.set_leftbound(LeftBound);
        config.set_rightbound(RightBound);
        config.set_maxtime(MaxTime);
        config.set_alpha(Alpha);
        config.set_gamma(Gamma);
        config.set_spacestep(SpaceStep);
        config.set_timestep(TimeStep);
        config.set_beta(Beta);
        config.set_alphaleft(AlphaLeft);
        config.set_alpharight(AlphaRight);
        config.set_betaleft(BetaLeft);
        config.set_betaright(BetaRight);
        config.set_bordersavailable(BordersAvailable);

        return config;
    }

    TSolverConfigBase TSolverConfigBase::FromProto(const ::PFDESolver::TClientConfig& config) {
        return TSolverConfigBase {
            .SpaceCount                 = static_cast<usize>((config.rightbound() - config.leftbound()) / config.spacestep()), 
            .TimeCount                  = static_cast<usize>(config.maxtime() / config.timestep()), 
            .LeftBound                  = config.leftbound(), 
            .RightBound                 = config.rightbound(),
            .MaxTime                    = config.maxtime(),
            .Alpha                      = config.alpha(), 
            .Gamma                      = config.gamma(),
            .SpaceStep                  = config.spacestep(), 
            .TimeStep                   = config.timestep(),
            .Beta                       = config.beta(),
            .AlphaLeft                  = config.alphaleft(), 
            .BetaLeft                   = config.betaleft(),
            .AlphaRight                 = config.alpharight(), 
            .BetaRight                  = config.betaright(),
            .BordersAvailable           = config.bordersavailable(),
            .StochasticIterationCount   = config.has_stochasticiterationcount() ? config.stochasticiterationcount() : TSolverConfigBase::DefaultIterationCount, 
            .RealSolutionName           = config.has_realsolutionname() ? std::optional(config.realsolutionname()) : std::nullopt,
        };
    }
}  // namespace NEquationSolver

namespace NEquationSolver {
    std::ostream& operator<<(std::ostream& out, const NEquationSolver::TSolverConfig& config) {

        out << "Space count N:\t\t"       << config.SpaceCount << Endl;
        out << "Time count K:\t\t"        << config.TimeCount << Endl;
        out << "Left bound L:\t\t"        << config.LeftBound << Endl;
        out << "Right bound R:\t\t"       << config.RightBound << Endl;
        out << "Max time:\t\t"            << config.MaxTime << Endl;
        out << "Alpha:\t\t\t"             << config.Alpha << Endl;
        out << "Gamma:\t\t\t"             << config.Gamma << Endl;
        out << "Space step:\t\t"          << config.SpaceStep << Endl;
        out << "Time step:\t\t"           << config.TimeStep << Endl;
        out << "Beta:\t\t\t"              << config.Beta << Endl;
        out << "Borders available:\t"     << config.BordersAvailable;

        return out;
    }
}  // namespace NEquationSolver
