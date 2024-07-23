#include "equation_solver.hpp"
#include <cassert>
#include <config.pb.h>
#include <result.pb.h>
#include <matrix.pb.h>

namespace NEquationSolver {

    PFDESolver::TResult IEquationSolver::TResult::ToProto() const {
        PFDESolver::TResult res;
        res.set_methodname(MethodName);
        res.mutable_field()->Swap(new PFDESolver::TMatrix(std::move(Field.ToProto())));
        return res;
    }

    bool IEquationSolver::TResult::SaveToFile(std::string name) const {
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

    IEquationSolver::IEquationSolver(const TSolverConfig& config) : Config(config) {
        Init();
        PrefetchData();
        Validate();
    }

    IEquationSolver::IEquationSolver(TSolverConfig&& config) : Config(std::move(config)) {
        Init();
        PrefetchData();
        Validate();
    }

    IEquationSolver::IEquationSolver(const IEquationSolver& solver) : IEquationSolver(solver.Config) {}
    IEquationSolver::IEquationSolver(IEquationSolver&& solver) : IEquationSolver(std::move(solver.Config)) {}

    void IEquationSolver::Init() {
        Config.SpaceCount = static_cast<usize>((Config.RightBound - Config.LeftBound) / Config.SpaceStep);
        Config.TimeCount = static_cast<usize>(Config.MaxTime / Config.TimeStep);
        PowTCGamma = std::pow(Config.TimeStep, Config.Gamma);
        PowSCAlpha = std::pow(Config.SpaceStep, Config.Alpha);
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

    IEquationSolver::~IEquationSolver() {
        
    }

    f64 IEquationSolver::CoefG(f64 a, usize i) const {
        assert((a == Config.Alpha) || (a == Config.Gamma));

        if (a == Config.Alpha) {
            assert(i < GAlpha.size());
            return GAlpha[i];
        }

        assert(i < GGamma.size());
        return GGamma[i];
    }

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
        * (Config.DiffusionCoefficient[i] / 2.0) 
        * (PowTCGamma / PowSCAlpha);
    }

    f64 IEquationSolver::CoefB(usize i) const {
        return (1.0 - Config.Beta) 
        * (Config.DiffusionCoefficient[i] / 2.0) 
        * (PowTCGamma / PowSCAlpha);
    }

    f64 IEquationSolver::CoefC(usize j) const {
        return Config.DemolitionCoefficient[j] * PowTCGamma / 2.0 / Config.SpaceStep;
    }

    const TSolverConfig& IEquationSolver::GetConfig() const {
        return Config;
    }

    void IEquationSolver::Validate() const {
        DEBUG_LOG << Config.DiffusionCoefficient << Endl;
        auto diffusionCMaxId = std::ranges::max_element(Config.DiffusionCoefficient.begin(), Config.DiffusionCoefficient.end());
        f64 const diffusionCMax = *diffusionCMaxId;
        auto demolitionCMaxId = std::ranges::max_element(Config.DemolitionCoefficient.begin(), Config.DemolitionCoefficient.end());
        f64 const demolitionCMax = *demolitionCMaxId;

        f64 left = diffusionCMax * PowTCGamma / PowSCAlpha;
        f64 right = Config.Gamma / Config.Alpha;
        INFO_LOG << "Left: " << left << " (" << (diffusionCMaxId - Config.DiffusionCoefficient.begin()) << " Right: " << right << Endl;
        if (left > right) {
            WARNING_LOG << "May be problem with condition" << Endl
                        << "\t\tD * pow(tau, gamma) / pow(h, alpha): " << left << Endl
                        << "\t\tgamma/alpha: " << right << Endl;

            WARNING_LOG << "May make h >= " << std::pow(PowTCGamma * diffusionCMax / right, 1.0/Config.Alpha) << Endl;
            WARNING_LOG << "Or tau <= " << std::pow(right * PowSCAlpha / diffusionCMax, 1.0/Config.Gamma) << Endl;
        }

        left = 2.0 * Config.SpaceStep * (CoefA(diffusionCMaxId - Config.DiffusionCoefficient.begin()) * GAlpha[2] + CoefB(diffusionCMaxId - Config.DiffusionCoefficient.begin()));
        right = demolitionCMax * PowTCGamma;
        if (left > right) {
            WARNING_LOG << "May be problem with condition" << Endl
                        << "\t\t2h * (C+ * g_a_2 + C-): " << left << Endl
                        << "\t\tpow(tau, gamma)*V: " << right << Endl;
        }

        left = 2.0 * Config.SpaceStep * (CoefB(diffusionCMaxId - Config.DiffusionCoefficient.begin()) * GAlpha[2] + CoefA(diffusionCMaxId - Config.DiffusionCoefficient.begin()));
        right = demolitionCMax * PowTCGamma;
        if (left < right) {
            WARNING_LOG << "May be problem with condition" << Endl
                        << "\t\t2h * (C- * g_a_2 + C+): " << left << Endl
                        << "\t\tpow(tau, gamma)*V: " << right << Endl;
        }
    }

    IEquationSolver::TResult IEquationSolver::Solve(bool saveMeta) {
        try {
            return DoSolve(saveMeta);
        } catch(...) {
            ERROR_LOG << "Something wrong with solving" << Endl;
            std::rethrow_exception(std::current_exception());
        }
    }
}
