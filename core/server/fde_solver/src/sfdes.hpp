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

        TStochasticFDES(const TSolverConfig& config) : IEquationSolver(config) {}
        TStochasticFDES(TSolverConfig&& config) :  IEquationSolver(std::move(config)) {}

        virtual std::string Name() const override {
            return "Stochastic method with " + TFiller::Name();
        }

        virtual TResult DoSolve(bool /*saveMeta*/) override {
            INFO_LOG << "Start solving Stochastic fractional-derivative equation solver..." << Endl;
            const i64 n = static_cast<i64>(Config.SpaceCount);
            const i64 k = static_cast<i64>(Config.TimeCount);
            const usize count = Config.StochasticIterationCount;

            DEBUG_LOG << "n: " << n <<  " k: " <<  k <<  " count: " << count  << Endl;

            NLinalg::TMatrix result(k + 1, n + 1, 0.0);
            NLinalg::TMatrix probabilities(n - 1, 2 * n + 2 + k, 0.0);
            NLinalg::TMatrix prefsumProbs(n - 1, 2 * n + 2 + k, 0.0);

            for (i64 i = 0; i < n - 1; i++) {
                for (i64 p = 0; p < 2 * n + 2 + k; p++) {
                    probabilities[i][p] = TFiller::FillProbabilities(this, probabilities, i + 1, p);
                }
                std::inclusive_scan(probabilities[i].begin(), probabilities[i].end(), prefsumProbs[i].begin());
            }

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
            std::knuth_b engine(device());  // knuth better than mt19937
            std::uniform_real_distribution<f64> generator(0.0, 1.0);

            for (i64 j = 1; j <= k; j++) {
                for (i64 i = 1; i < n; i++) {
                    for (usize _ = 0; _ < count; _++) {
                        i64 x = i, y = j;
                        f64 sf = 0.0;

                        while (y > 0 && x < n && x > 0) {
                            f64 rnd = generator(engine);
                            i64 idx = std::lower_bound(prefsumProbs[x - 1].begin(), prefsumProbs[x - 1].end(), rnd) - prefsumProbs[x - 1].begin();

                            sf += Config.SourceFunction[y][x];

                            if (idx <= 2 * n) { // перемещение по пространству
                                x += n - idx;
                                y--;
                            } else if (idx <= 2 * n + k) { // перемещение по времени
                                y -= idx - 2 * n + 1;
                            } else {
                                break;
                            }
                        }

                        result[j][i] += sf * PowTCGamma;

                        if (y <= 0 && (x >= 0) && (x <= n)) {
                            result[j][i] += Config.ZeroTimeState[x];
                        } else if (Config.BordersAvailable) {
                            if (x <= 0 && y > 0) {
                                result[j][i] += Config.LeftBoundState[y];
                            } else if (x >= n && y > 0) {
                                result[j][i] += Config.RightBoundState[y];
                            }
                        }
                    }
                    
                }
            }

            // Scaling
            for (i64 j = 1; j <= k; j++) {
                for (i64 i = 1; i < n; i++) {
                    result[j][i] /= static_cast<f64>(count);
                }
            }
            
            DEBUG_LOG << "Simulation time: " << timer.MilliSeconds().count() << "ms" << Endl;

            TResult res = {
                .MethodName = Name(),
                .Config = Config, 
                .Field = std::move(result),
            };

            return res;
        }
    };
}
