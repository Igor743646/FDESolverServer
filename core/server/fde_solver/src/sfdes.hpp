#pragma once

#include "equation_solver.hpp"
#include "rules_impl.hpp"
#include <matrix.hpp>

namespace NEquationSolver {

    template<TProbabilitiesFillRuleConcept TFiller = TMFDESRule>
    class TStochasticFDES : public IEquationSolver {
    protected:
        using IEquationSolver::TResult;
        using TMatrix = NLinalg::TMatrix;

    public: 

        explicit TStochasticFDES(const TSolverConfig& config) : IEquationSolver(config) {}
        explicit TStochasticFDES(TSolverConfig&& config) :  IEquationSolver(std::move(config)) {}

        virtual std::string Name() const override {
            return "Stochastic method with " + TFiller::Name();
        }

    private:
        
        std::vector<std::discrete_distribution<i64>> CreateDistributions() const {
            const i64 n = static_cast<i64>(Config.SpaceCount);
            const i64 k = static_cast<i64>(Config.TimeCount);
            const i64 probsLen = n + 1 + n + k + 1;

            std::vector<std::discrete_distribution<i64>> result;
            std::vector<f64> probabilities(probsLen, 0.0);

            for (i64 i = 0; i < n - 1; i++) {
                for (i64 p = 0; p < probsLen; p++) {
                    probabilities[p] = std::abs(TFiller::FillProbabilities(this, probabilities, i + 1, p));
                }
                result.emplace_back(probabilities.begin(), probabilities.end());
            }

            return result;
        }

    public:

        virtual TResult DoSolve(bool /*saveMeta*/) override {
            INFO_LOG << "Start solving Stochastic fractional-derivative equation solver..." << Endl;
            
            const i64 n = static_cast<i64>(Config.SpaceCount);
            const i64 k = static_cast<i64>(Config.TimeCount);
            const usize count = Config.StochasticIterationCount;

            DEBUG_LOG << "n: " << n <<  " k: " <<  k <<  " count: " << count  << Endl;

            NLinalg::TMatrix result(k + 1, n + 1, 0.0);
            std::vector<std::discrete_distribution<i64>> distributions = CreateDistributions();

            // Учёт начального и граничных условий
            for (i64 i = 0; i <= n; i++) {
                result[0][i] = Config.ZeroTimeState[i];
            }

            for (i64 j = 1; j <= k; j++) {
                result[j][0] = Config.LeftBoundState[j];
                result[j][n] = Config.RightBoundState[j];
            }

            // Симуляция
            NTimer::TTimer timer;
            std::random_device device;
            std::knuth_b engine(device());

            const f64 koefNorm = 1.0 / static_cast<f64>(count);

            NLinalg::TMatrix timeForCeil(k + 1, n + 1, 0.0);

            for (i64 j = 1; j <= k; j++) {
                for (i64 i = 1; i < n; i++) {
                    NTimer::TTimer ceilTimer;
                    f64 resultji = 0.0;
                    
                    for (i64 _ = 0; _ < (i64)count; _++) {
                        i64 x = i, y = j;
                        f64 sf = 0.0;

                        while (y > 0 && x < n && x > 0) {
                            const i64 idx = distributions[x - 1](engine);
                            sf += Config.SourceFunction[y][x];

                            if (idx <= 2 * n + k) {
                                y--;
                                if (idx - n - n <= 0) {
                                    x -= idx - n;
                                } else {
                                    y -= idx - n - n;
                                }
                            } else {
                                break;
                            }
                        }

                        resultji += sf * PowTCGamma;

                        if (y <= 0 && (x >= 0) && (x <= n)) {
                            resultji += Config.ZeroTimeState[x];
                        } else if (Config.BordersAvailable) {
                            if (y > 0) {
                                if (x <= 0) {
                                    resultji += Config.LeftBoundState[y];
                                } else if (x >= n) {
                                    resultji += Config.RightBoundState[y];
                                }
                            }
                        }
                    }
                    
                    result[j][i] = resultji * koefNorm;
                    timeForCeil[j][i] = ceilTimer.MilliSeconds().count();
                    timeForCeil[0][i] += timeForCeil[j][i];
                    timeForCeil[0][0] += timeForCeil[j][i];
                }
            }

            DEBUG_LOG << "Simulation time: " << timer.MilliSeconds().count() << "ms" << Endl;
            DEBUG_LOG << "Simulation time by ceil: " << Endl << timeForCeil << Endl;

            TResult res = {
                .MethodName = Name(),
                .Config = Config, 
                .Field = std::move(result),
            };

            return res;
        }
    };
}
