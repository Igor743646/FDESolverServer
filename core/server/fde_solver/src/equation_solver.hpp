/*
    Equation solver interface
*/

#pragma once

#include <cmath>
#include <functional>
#include <unordered_map>
#include <iostream>
#include <math_utils.hpp>
#include <logger.hpp>
#include <matrix.hpp>
#include <timer.hpp>

#include "config.hpp"

namespace PFDESolver {
    class TResult;
}

namespace NEquationSolver {

    class IEquationSolver {
    protected:
        TSolverConfig Config;
        std::vector<f64> GAlpha;
        std::vector<f64> GGamma;

        void PrefetchData();
        void Init();

    public:

        f64 PowTCGamma, PowSCAlpha;

        struct TResult {
            std::string MethodName;
            NLinalg::TMatrix Field;

            [[nodiscard]] PFDESolver::TResult ToProto() const;
            [[nodiscard]] bool SaveToFile(const std::string& name) const;
        };

        explicit IEquationSolver(const TSolverConfig&);
        explicit IEquationSolver(TSolverConfig&&);
        IEquationSolver(const IEquationSolver&);
        IEquationSolver(IEquationSolver&&) noexcept;
        IEquationSolver& operator=(const IEquationSolver&) = delete;
        IEquationSolver& operator=(IEquationSolver&&) noexcept = delete;
        virtual ~IEquationSolver();

        [[nodiscard]] f64 Space(usize) const;
        [[nodiscard]] f64 Time(usize) const;
        [[nodiscard]] f64 CoefA(usize) const;
        [[nodiscard]] f64 CoefB(usize) const;
        [[nodiscard]] f64 CoefC(usize) const;
        [[nodiscard]] f64 CoefGAlpha(usize) const;
        [[nodiscard]] f64 CoefGGamma(usize) const;

        [[nodiscard]] virtual std::string Name() const = 0;
        virtual TResult Solve(bool saveMeta) final;
        virtual TResult DoSolve(bool saveMeta) = 0;

        [[nodiscard]] const TSolverConfig& GetConfig() const;
        virtual void Validate() const final;

        friend std::ostream& operator<<(std::ostream& out, const IEquationSolver& solver) {
            out << solver.Name() << Endl;
            return out;
        }
    };
}  // namespace NEquationSolver
