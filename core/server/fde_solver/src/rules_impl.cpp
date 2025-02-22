#include "rules_impl.hpp"

namespace NEquationSolver {
    f64 TMFDESRule::FillMatrix(IEquationSolver const *const solver, usize i, usize j) {
        f64 result = 0.0;

        if (i + 1 >= j) {
            result += solver->CoefA(i) * solver->CoefGAlpha(i - j + 1);
        }

        if (i <= j + 1) {
            result += solver->CoefB(i) * solver->CoefGAlpha(j - i + 1);
        }

        if (i == j) {
            result -= 1.0;
        }

        if (i == j + 1) {
            result -= solver->CoefC(i);
        }

        if (i + 1 == j) {
            result += solver->CoefC(i);
        }

        return result;
    }

    // Math: d_i^k = \sum_{j=1}^{k}{g_{\gamma, j}(u_i^{k-j} - u_i^0)} - (u_i^0 + \tau^\gamma f(x_i, t_j))
    f64 TMFDESRule::FillDestination(IEquationSolver const *const solver, const NLinalg::TMatrix& result, usize i, usize k) {
        f64 di = 0.0;

        di -= result[0][i];
        di -= solver->PowTCGamma * solver->GetConfig().SourceFunction[k][i];

        for (usize j = 1; j <= k; j++) {
            di += solver->CoefGGamma(j) * (result[k-j][i] - result[0][i]);
        }

        return di;
    }

    f64 TMFDESRule::FillProbabilities(IEquationSolver const *const solver, const std::span<f64>& probabilities, usize i, usize probIdx) {
        const usize n = solver->GetConfig().SpaceCount;
        const usize k = solver->GetConfig().TimeCount;
        const f64 alpha = solver->GetConfig().Alpha;
        const f64 gamma = solver->GetConfig().Gamma;

        if (probIdx < n - 1) {
            return solver->CoefB(i) * solver->CoefGAlpha(n - probIdx + 1); 
        } 
        if (probIdx == n - 1) {
            return solver->CoefA(i) + solver->CoefB(i) * solver->CoefGAlpha(2) + solver->CoefC(i);
        } 
        if (probIdx == n) {
            return gamma - alpha * (solver->CoefA(i) + solver->CoefB(i));
        } 
        if (probIdx == n + 1) {
            return solver->CoefA(i) * solver->CoefGAlpha(2) + solver->CoefB(i) - solver->CoefC(i);
        } 
        if (n + 1 < probIdx && probIdx <= 2 * n) {
            return solver->CoefA(i) * solver->CoefGAlpha(probIdx - n + 1);
        } 
        if (2 * n < probIdx && probIdx <= 2 * n + k) {
            return -solver->CoefGGamma(probIdx - 2 * n + 1);
        } 

        return 1.0 - std::accumulate(probabilities.begin(), probabilities.end(), 0.0);
    }

    /*
        // Math: \theta_0 = \frac{1}{(2-\alpha)\Gamma(2-\alpha)2^{2-\alpha}}
        // Math: \theta_1 = \theta_0 * (3^{2-\alpha}-2)
        // Math: \theta_k = \theta_0 * ((2k+1)^{2-\alpha}-2(2k-1)^{2-\alpha}+(2k-3)^{2-\alpha})
    */
    f64 TRLFDESRule::CoefGMatrix(IEquationSolver const *const solver, usize k) {
        const f64 alpha = solver->GetConfig().Alpha;
        const f64 th0 = 1.0 / (NFunctions::Gamma(3.0 - alpha) * (std::pow(2.0, 2.0 - alpha)));
        
        if (k == 0) {
            return th0;
        }
        
        if (k == 1) {
            return th0 * (std::pow(3, 2 - alpha) - 2);
        }
        
        return th0 * (std::pow(2 * k - 3, 2 - alpha) - 2 * std::pow(2 * k - 1, 2 - alpha) + std::pow(2 * k + 1, 2 - alpha));
    }

    f64 TRLFDESRule::FillMatrix(IEquationSolver const *const solver, usize i, usize j) {
        const usize n = solver->GetConfig().SpaceCount;
        f64 result = 0.0;

        if (i + 1 >= j) { // Left
            if (i + 1 == j) {
                result += solver->CoefA(i) * CoefGMatrix(solver, 0);
            } else if (j == 0 && i > 0) {
                result -= solver->CoefA(i) * CoefGMatrix(solver, i);
            } else {
                result += solver->CoefA(i) * (CoefGMatrix(solver, i - j + 1) - CoefGMatrix(solver, i - j));
            }
        }

        if (i <= j + 1) { // Right
            if (i == j + 1) {
                result += solver->CoefB(i) * CoefGMatrix(solver, 0);
            } else if (j == n && i < n) {
                result -= solver->CoefB(i) * CoefGMatrix(solver, n - i);
            } else {
                result += solver->CoefB(i) * (CoefGMatrix(solver, j - i + 1) - CoefGMatrix(solver, j - i));
            }
        }

        if (i == j) {
            result -= CoefGDestination(solver, 0);
        }

        if (i == j + 1) {
            result -= solver->CoefC(i);
        }

        if (i + 1 == j) {
            result += solver->CoefC(i);
        }

        return result;
    }

    /*
        // Math: \theta_0 = \frac{1}{(1-\gamma)\Gamma(1-\gamma)}
        // Math: \theta_k = \theta_0 * (k^{1-\gamma} - (k-1)^{1-\gamma})
    */
    f64 TRLFDESRule::CoefGDestination(IEquationSolver const *const solver, usize k) {
        const f64 gamma = solver->GetConfig().Gamma;
        const f64 th0 = 1.0 / NFunctions::Gamma(2.0 - gamma);
        
        if (k == 0) {
            return th0;
        } 
        
        return th0 * (std::pow(k, 1.0 - gamma) - std::pow(k - 1, 1.0 - gamma));
    }

    // Math: d_i^k = -\theta_{k}u_i^0 + \sum_{j=1}^{k-1}{(\theta_{k-j+1}-\theta_{k-j})u_i^{j}} - \tau^\gamma f(x_i, t_j)
    f64 TRLFDESRule::FillDestination(IEquationSolver const *const solver, const NLinalg::TMatrix& result, usize i, usize k) {
        f64 di = 0.0;

        di -= CoefGDestination(solver, k) * result[0][i];
        for (usize j = 1; j < k; j++) {
            di += (CoefGDestination(solver, k-j+1) - CoefGDestination(solver, k-j)) * result[j][i];
        }

        di -= solver->PowTCGamma * solver->GetConfig().SourceFunction[k][i];
        
        return di;
    }

    f64 TRLFDESRule::FillProbabilities(IEquationSolver const *const solver, const std::span<f64>& probabilities, usize i, usize probIdx) {
        const usize n = solver->GetConfig().SpaceCount;
        const usize k = solver->GetConfig().TimeCount;

        if (probIdx == n) {
            return ((CoefGMatrix(solver, 1) - CoefGMatrix(solver, 0)) * (solver->CoefA(i) + solver->CoefB(i)) + (CoefGDestination(solver, 0) - CoefGDestination(solver, 2))) / CoefGDestination(solver, 0);
        }
        if (probIdx == n - 1) {
            f64 result = (CoefGMatrix(solver, 0) * solver->CoefA(i) + (CoefGMatrix(solver, 2) - CoefGMatrix(solver, 1)) * solver->CoefB(i) + solver->CoefC(i)) / CoefGDestination(solver, 0);
            if (i == n - 1) {
                result -= (CoefGMatrix(solver, 2) * solver->CoefB(i)) / CoefGDestination(solver, 0);
            }
            return result;
        }
        if (probIdx == n + 1) {
            f64 result = ((CoefGMatrix(solver, 2) - CoefGMatrix(solver, 1)) * solver->CoefA(i) + CoefGMatrix(solver, 0) * solver->CoefB(i) - solver->CoefC(i)) / CoefGDestination(solver, 0);
            if (i == 1) {
                result -= (CoefGMatrix(solver, 2) * solver->CoefA(i)) / CoefGDestination(solver, 0);
            }
            return result;
        } 

        if (i <= probIdx && probIdx < n - 1) {
            if (probIdx == i) {
                return -CoefGMatrix(solver, n - probIdx) * solver->CoefB(i) / CoefGDestination(solver, 0); 
            }
            return (CoefGMatrix(solver, n - probIdx + 1) - CoefGMatrix(solver, n - probIdx)) * solver->CoefB(i) / CoefGDestination(solver, 0); 
        }

        if (n + 1 < probIdx && probIdx <= n + i) {
            if (probIdx == n + i) {
                return -CoefGMatrix(solver, probIdx - n) * solver->CoefA(i) / CoefGDestination(solver, 0);
            }
            return (CoefGMatrix(solver, probIdx - n + 1) - CoefGMatrix(solver, probIdx - n)) * solver->CoefA(i) / CoefGDestination(solver, 0);
        }

        if (2 * n < probIdx && probIdx <= 2 * n + k) {
            if (probIdx == 2 * n + k) {
                return CoefGDestination(solver, probIdx - 2 * n + 2) / CoefGDestination(solver, 0);
            }
            return (CoefGDestination(solver, probIdx - 2 * n + 1) - CoefGDestination(solver, probIdx - 2 * n + 2)) / CoefGDestination(solver, 0);
        }

        if (probIdx == 2 * n + k + 1) {
            return 1.0 - std::accumulate(probabilities.begin(), probabilities.end(), 0.0);
        }

        return 0.0;
    }
}  // namespace NEquationSolver

namespace NEquationSolver {
    
    f64 TRLFDESRule2::CoefGMatrixLeft(IEquationSolver const *const solver, usize j, usize i) {
        const f64 alpha = solver->GetConfig().Alpha;
        const f64 gamma = solver->GetConfig().Gamma;
        const f64 th0 = NFunctions::Gamma(2.0 - gamma) / (NFunctions::Gamma(3.0 - alpha) * (std::pow(2.0, 2.0 - alpha)));
        
        if (j == i + 1) {
            return th0;
        }

        if (j == i) {
            return th0 * (std::pow(3, 2 - alpha) - 3);
        }

        if (j + 1 == i) {
            return th0 * (std::pow(5, 2 - alpha) - 3 * std::pow(3, 2 - alpha) + 3);
        }

        if (j == 0) {
            return th0 * (2 * (2 - alpha) * (std::pow(2*i+1, 1 - alpha) - std::pow(2*i-1, 1 - alpha)) - (std::pow(2*i+1, 2 - alpha) - 2 * std::pow(2*i-1, 2 - alpha) + std::pow(2*i-3, 2 - alpha)));
        }
        
        return th0 * (std::pow(2*(i-j) + 3, 2 - alpha) - 3 * std::pow(2*(i-j) + 1, 2 - alpha) + 3 * std::pow(2*(i-j) -1, 2 - alpha) - std::pow(2*(i-j) - 3, 2 - alpha));
    }

    f64 TRLFDESRule2::CoefGMatrixRight(IEquationSolver const *const solver, usize j, usize i) {
        const f64 alpha = solver->GetConfig().Alpha;
        const f64 gamma = solver->GetConfig().Gamma;
        const usize n = solver->GetConfig().SpaceCount;
        const f64 th0 = NFunctions::Gamma(2.0 - gamma) / (NFunctions::Gamma(3.0 - alpha) * (std::pow(2.0, 2.0 - alpha)));
        
        if (j + 1 == i) {
            return th0;
        }

        if (j == i) {
            return th0 * (std::pow(3, 2 - alpha) - 3);
        }

        if (j == i + 1) {
            return th0 * (std::pow(5, 2 - alpha) - 3 * std::pow(3, 2 - alpha) + 3);
        }

        if (j == n) {
            return th0 * (2 * (2 - alpha) * (std::pow(2*(n - i)+1, 1 - alpha) - std::pow(2*(n - i)-1, 1 - alpha)) - (std::pow(2*(n - i)+1, 2 - alpha) - 2 * std::pow(2*(n - i)-1, 2 - alpha) + std::pow(2*(n - i)-3, 2 - alpha)));
        }
        
        return th0 * (std::pow(2*(j-i) + 3, 2 - alpha) - 3 * std::pow(2*(j-i) + 1, 2 - alpha) + 3 * std::pow(2*(j-i) -1, 2 - alpha) - std::pow(2*(j-i) - 3, 2 - alpha));
    }

    f64 TRLFDESRule2::FillMatrix(IEquationSolver const *const solver, usize i, usize j) {
        const f64 gamma = solver->GetConfig().Gamma;
        const usize n = solver->GetConfig().SpaceCount;
        f64 result = 0.0;

        if (j <= i + 1) { // Left
            result += solver->CoefA(i) * CoefGMatrixLeft(solver, j, i);
        }

        if (i <= j + 1) { // Right
            result += solver->CoefB(i) * CoefGMatrixRight(solver, j, i);
        }

        if (i == j) {
            result -= 1;
        }

        if (i == j + 1) {
            result -= NFunctions::Gamma(2.0 - gamma) * solver->CoefC(i);
        }

        if (i + 1 == j) {
            result += NFunctions::Gamma(2.0 - gamma) * solver->CoefC(i);
        }

        return result;
    }

    /*
        // Math: \theta_0 = \frac{1}{(1-\gamma)\Gamma(1-\gamma)}
        // Math: \theta_k = \theta_0 * (k^{1-\gamma} - (k-1)^{1-\gamma})
    */
    f64 TRLFDESRule2::CoefGDestination(IEquationSolver const *const solver, usize j, usize k) {
        const f64 gamma = solver->GetConfig().Gamma;
        
        if (j == k) {
            return 1;
        }

        if (j + 1 == k) {
            return std::pow(2, 1.0 - gamma) - 2;
        }

        if (j == 0) {
            return (1 - gamma) * std::pow(k, -gamma) - std::pow(k, 1 - gamma) + std::pow(k - 1, 1 - gamma);
        }
        
        return (std::pow(k - j + 1, 1.0 - gamma) - 2 * std::pow(k - j, 1.0 - gamma) + std::pow(k - j - 1, 1.0 - gamma));
    }

    // Math: d_i^k = -\theta_{k}u_i^0 + \sum_{j=1}^{k-1}{(\theta_{k-j+1}-\theta_{k-j})u_i^{j}} - \tau^\gamma f(x_i, t_j)
    f64 TRLFDESRule2::FillDestination(IEquationSolver const *const solver, const NLinalg::TMatrix& result, usize i, usize k) {
        const f64 gamma = solver->GetConfig().Gamma;

        f64 di = 0.0;

        for (usize j = 0; j < k; j++) {
            di += CoefGDestination(solver, j, k) * result[j][i];
        }

        di -= solver->PowTCGamma * NFunctions::Gamma(2.0 - gamma) * solver->GetConfig().SourceFunction[k][i];
        
        return di;
    }
}
