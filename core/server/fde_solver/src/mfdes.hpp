#pragma once

#include "equation_solver.hpp"
#include "rules_impl.hpp"
#include <matrix.hpp>
#include <concepts>
#include <cstring>

namespace NEquationSolver {

    template<TMatrixFillRuleConcept TFiller = TMFDESRule>
    class TMatrixFDES : public IEquationSolver {
    protected:
        using IEquationSolver::TResult;
        using TMatrix = NLinalg::TMatrix;

    public:

        TMatrixFDES(const TSolverConfig& config) : IEquationSolver(config) { }
        TMatrixFDES(TSolverConfig&& config) : IEquationSolver(std::move(config)) { }

        virtual std::string Name() const override {
            return "Matrix method with " + TFiller::Name();
        }

        virtual TResult DoSolve(bool /*saveMeta*/) override {
            INFO_LOG << "Start solving Matrix fractional-derivative equation solver..." << Endl;
            const usize n = Config.SpaceCount;
            const usize k = Config.TimeCount;

            DEBUG_LOG << "n: " << n << " k: " << k << Endl;

            NLinalg::TMatrix result(k + 1, n + 1, 0.0);
            
            // Math: u_i^0=\psi(x_i)
            // for (usize i = 0; i <= n; i++) {
            //     result[0][i] = Config.ZeroTimeState[i];
            // }
            std::memcpy(result[0].data(), Config.ZeroTimeState.data(), (n + 1) * sizeof(f64));

            TMatrix matrix(n + 1);
            FillMatrix(matrix);

            auto plu = matrix.LUFactorizing();

            if (!plu.has_value()) {
                NStackTracer::TStackTracer::ThrowWithMessage("PLU Decomposition error");
            }
            
            // Math: Au^k=d^k
            std::vector<f64> d(n + 1, 0.0);
            for (usize t = 1; t <= k; t++) {
                // create d-vector
                FillDestination(d, result, t);
                
                // solve system
                const auto r = TMatrix::Solve(plu.value(), d).value();
                std::memcpy(result[t].data(), r.data(), r.size() * sizeof(f64));
            }

            TResult res = {
                .MethodName = Name(),
                .Config = Config, 
                .Field = std::move(result)
            };

            return res;
        }

    private:

        void FillMatrix(TMatrix& matrix) {
            const usize n = Config.SpaceCount;

            // create matrix A
            for (usize i = 0; i <= n; i++) {
                for (usize j = 0; j <= n; j++) {
                    matrix[i][j] = TFiller::FillMatrix(this, i, j);
                }
            }

            //// 1-border conditions
            if (Config.BordersAvailable) {
                std::fill(matrix[0].begin(), matrix[0].end(), 0.0);
                std::fill(matrix[n].begin(), matrix[n].end(), 0.0);

                matrix[0][0] = 1.0;
                matrix[n][n] = 1.0;
            }
        }

        void FillDestination(std::vector<f64>& d, const NLinalg::TMatrix& result, usize k) {
            const usize n = Config.SpaceCount;
            
            for (usize i = 0; i <= n; i++) {
                d[i] = TFiller::FillDestination(this, result, i, k);
            }

            // borders
            if (Config.BordersAvailable) {
                d[0] = Config.LeftBoundState[k];
                d[n] = Config.RightBoundState[k];
            }
        }
    };
}