#include "config.hpp"

#include <config.pb.h>

namespace NEquationSolver {
    TSolverConfig::TSolverConfig(const TParsedSolverConfig& parsedConfig) : TSolverConfigBase(parsedConfig) {
        DiffusionCoefficient.resize(parsedConfig.SpaceCount+1);
        DemolitionCoefficient.resize(parsedConfig.SpaceCount+1);
        ZeroTimeState.resize(parsedConfig.SpaceCount+1);
        SourceFunction = NLinalg::TMatrix(parsedConfig.TimeCount+1, parsedConfig.SpaceCount+1);
        LeftBoundState.resize(parsedConfig.TimeCount+1);
        RightBoundState.resize(parsedConfig.TimeCount+1);

        for (usize i = 0; i <= parsedConfig.SpaceCount; i++) {
            f64 x = parsedConfig.LeftBound + static_cast<f64>(i) * parsedConfig.SpaceStep;
            
            DiffusionCoefficient[i] = parsedConfig.DiffusionCoefficient(x);
            DemolitionCoefficient[i] = parsedConfig.DemolitionCoefficient(x);
            ZeroTimeState[i] = parsedConfig.ZeroTimeState(x);
        }

        for (usize j = 0; j <= parsedConfig.TimeCount; j++) {
            f64 t = parsedConfig.TimeStep * static_cast<f64>(j);
            LeftBoundState[j] = parsedConfig.LeftBoundState(t);
            RightBoundState[j] = parsedConfig.RightBoundState(t);
            
            for (usize i = 0; i <= parsedConfig.SpaceCount; i++) {
                f64 x = parsedConfig.LeftBound + static_cast<f64>(i) * parsedConfig.SpaceStep;
                SourceFunction[j][i] = parsedConfig.SourceFunction(x, t);
            }
        }

        if (parsedConfig.RealSolution.has_value()) {
            RealSolution = NLinalg::TMatrix(parsedConfig.TimeCount+1, parsedConfig.SpaceCount+1);
            for (usize j = 0; j <= parsedConfig.TimeCount; j++) {
                f64 t = parsedConfig.TimeStep * static_cast<f64>(j);
                for (usize i = 0; i <= parsedConfig.SpaceCount; i++) {
                    f64 x = parsedConfig.LeftBound + static_cast<f64>(i) * parsedConfig.SpaceStep;
                    (*RealSolution)[j][i] = parsedConfig.RealSolution.value()(x, t);
                }
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
}

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
}
