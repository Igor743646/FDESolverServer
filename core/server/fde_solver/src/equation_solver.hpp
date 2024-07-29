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

    protected:

        void PrefetchData();
        void Init();

    public:

        f64 PowTCGamma, PowSCAlpha;

        struct TResult {
            std::string MethodName;
            const TSolverConfig& Config;
            NLinalg::TMatrix Field;

            PFDESolver::TResult ToProto() const;
            bool SaveToFile(std::string name) const;
        };

        explicit IEquationSolver(const TSolverConfig&);
        explicit IEquationSolver(TSolverConfig&&);
        IEquationSolver(const IEquationSolver&);
        IEquationSolver(IEquationSolver&&);
        virtual ~IEquationSolver();

        f64 Space(usize) const;
        f64 Time(usize) const;
        f64 CoefA(usize) const;
        f64 CoefB(usize) const;
        f64 CoefC(usize) const;
        f64 CoefG(f64, usize) const;
        f64 CoefGAlpha(usize) const;
        f64 CoefGGamma(usize) const;

        virtual std::string Name() const = 0;
        virtual TResult Solve(bool saveMeta) final;
        virtual TResult DoSolve(bool saveMeta) = 0;

        const TSolverConfig& GetConfig() const;
        virtual void Validate() const final;

        friend std::ostream& operator<<(std::ostream& out, const IEquationSolver& solver) {
            out << solver.Name() << Endl;
            return out;
        }
    };
}
