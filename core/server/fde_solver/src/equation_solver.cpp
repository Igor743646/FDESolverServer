#include "equation_solver.hpp"
#include <cassert>
#include <config.pb.h>
#include <result.pb.h>
#include <matrix.pb.h>

#undef max
#undef min

namespace NEquationSolver {

    PFDESolver::TResult IEquationSolver::TResult::ToProto() const {
        PFDESolver::TResult res;
        res.set_methodname(MethodName);
        res.mutable_field()->Swap(new PFDESolver::TMatrix(std::move(Field.ToProto())));
        return res;
    }

    bool IEquationSolver::TResult::SaveToFile(const std::string& name) const {
        INFO_LOG << "Save result in file: " << name << Endl;
        std::ofstream binaryFile(name, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);

        if (binaryFile.is_open()) {
            PFDESolver::TResult res = ToProto();
            binaryFile << res.SerializeAsString();
        } else {
            ERROR_LOG << "Bad openning" << Endl;
            return false;
        }

        return true;
    }

    IEquationSolver::IEquationSolver(const TSolverConfig& config) 
    : Config(config), 
      PowTCGamma(std::pow(Config.TimeStep, Config.Gamma)),
      PowSCAlpha(std::pow(Config.SpaceStep, Config.Alpha))
    {
        Init();
        PrefetchData();
        Validate();
    }

    IEquationSolver::IEquationSolver(TSolverConfig&& config) 
    : Config(std::move(config)), 
      PowTCGamma(std::pow(Config.TimeStep, Config.Gamma)),
      PowSCAlpha(std::pow(Config.SpaceStep, Config.Alpha)) 
    {
        Init();
        PrefetchData();
        Validate();
    }

    IEquationSolver::IEquationSolver(const IEquationSolver& solver) : IEquationSolver(solver.Config) {}
    IEquationSolver::IEquationSolver(IEquationSolver&& solver) noexcept : IEquationSolver(std::move(solver.Config)) {}

    void IEquationSolver::Init() {
        Config.SpaceCount = static_cast<usize>((Config.RightBound - Config.LeftBound) / Config.SpaceStep);
        Config.TimeCount = static_cast<usize>(Config.MaxTime / Config.TimeStep);
    }

    void IEquationSolver::PrefetchData() {
        GAlpha.resize(Config.SpaceCount + 2, 1.0);
        GGamma.resize(Config.TimeCount + 2, 1.0);

        for (usize i = 1; i < Config.SpaceCount + 2; i++) {
            GAlpha[i] = (static_cast<f64>(i) - 1.0 - Config.Alpha) / static_cast<f64>(i) * GAlpha[i - 1];
        }

        for (usize i = 1; i < Config.TimeCount + 2; i++) {
            GGamma[i] = (static_cast<f64>(i) - 1.0 - Config.Gamma) / static_cast<f64>(i) * GGamma[i - 1];
        }
    }

    IEquationSolver::~IEquationSolver() = default;

    f64 IEquationSolver::CoefGAlpha(usize i) const {
        assert(i < GAlpha.size());
        return GAlpha[i];
    }

    f64 IEquationSolver::CoefGGamma(usize j) const {
        assert(j < GGamma.size());
        return GGamma[j];
    }

    f64 IEquationSolver::Space(usize i) const {
        return Config.LeftBound + static_cast<f64>(i) * Config.SpaceStep;
    }

    f64 IEquationSolver::Time(usize j) const {
        return Config.TimeStep * static_cast<f64>(j);
    }

    f64 IEquationSolver::CoefA(usize i) const {
        return (1.0 + Config.Beta) 
        * (Config.LeftDiffusionCoefficient[i] / 2.0) 
        * (PowTCGamma / PowSCAlpha);
    }

    f64 IEquationSolver::CoefB(usize i) const {
        return (1.0 - Config.Beta) 
        * (Config.RightDiffusionCoefficient[i] / 2.0) 
        * (PowTCGamma / PowSCAlpha);
    }

    f64 IEquationSolver::CoefC(usize j) const {
        return Config.DemolitionCoefficient[j] * PowTCGamma / 2.0 / Config.SpaceStep;
    }

    const TSolverConfig& IEquationSolver::GetConfig() const {
        return Config;
    }

    void IEquationSolver::CheckDiffusionCoefficient() const {
        const auto minLeftDiffCoef = std::ranges::min(Config.LeftDiffusionCoefficient);
        const auto minRightDiffCoef = std::ranges::min(Config.RightDiffusionCoefficient);

        if (minLeftDiffCoef < 0.0) {
            WARNING_LOG << std::format("Problem with left diffusion coefficient: "
                                       "D: {} > 0", minLeftDiffCoef) << Endl;

        }

        if (minRightDiffCoef < 0.0) {
            WARNING_LOG << std::format("Problem with right diffusion coefficient: "
                                       "D: {} > 0", minRightDiffCoef) << Endl;

        }
    }

    void IEquationSolver::CheckMainStabilityCondition() const {
        const f64 maxLeftDiffCoef = std::ranges::max(Config.LeftDiffusionCoefficient);
        const f64 maxRightDiffCoef = std::ranges::max(Config.RightDiffusionCoefficient);
        const f64 maxDiffCoef = std::max(maxLeftDiffCoef, maxRightDiffCoef);

        const f64 left = maxDiffCoef * PowTCGamma / PowSCAlpha;
        const f64 right = Config.Gamma / Config.Alpha;
        
        if (left > right) {
            WARNING_LOG << std::format("Problem with main stability condition: "
                                       "D * pow(tau, gamma) / pow(h, alpha): {} <= gamma/alpha: {}", left, right) << Endl;
            
            WARNING_LOG << "May make h >= " << std::pow(PowTCGamma * maxDiffCoef / right, 1.0/Config.Alpha) << Endl;
            WARNING_LOG << "Or tau <= " << std::pow(right * PowSCAlpha / maxDiffCoef, 1.0/Config.Gamma) << Endl;
        }
    }

    void IEquationSolver::CheckStochasticMethodStabilityCondition1() const {
        // const auto [minDiffCoef, maxDiffCoef] = std::ranges::minmax_element(Config.DiffusionCoefficient);
        // const auto [minDemCoef, maxDemCoef] = std::ranges::minmax_element(Config.DemolitionCoefficient);
        // const auto minDiffCoefPos = std::ranges::distance(Config.DiffusionCoefficient.begin(), minDiffCoef);

        // {
        //     const f64 left = (*maxDemCoef) * PowTCGamma / 2.0 / Config.SpaceStep;
        //     const f64 right = CoefA(minDiffCoefPos) * GAlpha[2] + CoefB(minDiffCoefPos);
        //     if (left > right) {
        //         WARNING_LOG << std::format("Problem with stochastic method stability condition: "
        //                                    "V * pow(tau, gamma) / 2h: {} <= C+ * g_a_2 + C-: {}", left, right) << Endl;

        //     }
        // }

        // {
        //     const f64 left = (*minDemCoef) * PowTCGamma / 2.0 / Config.SpaceStep;
        //     const f64 right = -(CoefB(minDiffCoefPos) * GAlpha[2] + CoefA(minDiffCoefPos));
        //     if (left < right) {
        //         WARNING_LOG << std::format("Problem with stochastic method stability condition: "
        //                                    "V * pow(tau, gamma) / 2h: {} >= C- * g_a_2 + C+: {}", left, right) << Endl;

        //     }
        // }
    }

    void IEquationSolver::CheckStochasticMethodStabilityCondition2() const {

    }

    void IEquationSolver::Validate() const {
        CheckDiffusionCoefficient();
        CheckMainStabilityCondition();
        CheckStochasticMethodStabilityCondition1();
        CheckStochasticMethodStabilityCondition2();
    }

    IEquationSolver::TResult IEquationSolver::Solve(bool saveMeta) {
        try {
            return DoSolve(saveMeta);
        } catch(...) {
            ERROR_LOG << "Something wrong with solving" << Endl;
            std::rethrow_exception(std::current_exception());
        }
    }
}  // namespace NEquationSolver
