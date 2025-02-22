#pragma once

#include "equation_solver.hpp"

namespace NEquationSolver {

    constexpr size_t MethodsCount = 5;

    template<class T>
    concept TMatrixFillRuleConcept = requires (IEquationSolver const *const solver, const NLinalg::TMatrix& result, usize i, usize j) {
        requires !std::is_destructible_v<T>;
        requires !std::is_constructible_v<T>;
        { T::FillMatrix(solver, i, j) } -> std::convertible_to<f64>;
        { T::FillDestination(solver, result, i, j) } -> std::convertible_to<f64>;
        { T::Name() } -> std::convertible_to<std::string>;
    };

    template<class T>
    concept TProbabilitiesFillRuleConcept = requires (IEquationSolver const *const solver, const std::span<f64>& probabilities, usize i, usize probIdx) {
        requires !std::is_destructible_v<T>;
        requires !std::is_constructible_v<T>;
        { T::FillProbabilities(solver, probabilities, i, probIdx) } -> std::convertible_to<f64>;
        { T::Name() } -> std::convertible_to<std::string>;
    };

    struct TMFDESRule {
        static std::string Name() { return "Grunwald-Letnikov approximation"; }; 
        static f64 FillMatrix(IEquationSolver const *, usize, usize);
        static f64 FillDestination(IEquationSolver const *, const NLinalg::TMatrix&, usize, usize);
        static f64 FillProbabilities(IEquationSolver const *, const std::span<f64>&, usize, usize);
        TMFDESRule() = delete;
        TMFDESRule(const TMFDESRule&) = delete;
        TMFDESRule(TMFDESRule&&) = delete;
        TMFDESRule& operator=(const TMFDESRule&) = delete;
        TMFDESRule& operator=(TMFDESRule&&) = delete;
        ~TMFDESRule() = delete;
    };

    struct TRLFDESRule {
        static std::string Name() { return "Riemann-Liouville approximation"; };
        static f64 FillMatrix(IEquationSolver const *, usize, usize);
        static f64 FillDestination(IEquationSolver const *, const NLinalg::TMatrix&, usize, usize);
        static f64 FillProbabilities(IEquationSolver const *, const std::span<f64>&, usize, usize);
        static f64 CoefGMatrix(IEquationSolver const *, usize);
        static f64 CoefGDestination(IEquationSolver const *, usize);
        TRLFDESRule() = delete;
        TRLFDESRule(const TRLFDESRule&) = delete;
        TRLFDESRule(TRLFDESRule&&) = delete;
        TRLFDESRule& operator=(const TRLFDESRule&) = delete;
        TRLFDESRule& operator=(TRLFDESRule&&) = delete;
        ~TRLFDESRule() = delete;
    };

    struct TRLFDESRule2 {
        static std::string Name() { return "Riemann-Liouville approximation"; };
        static f64 FillMatrix(IEquationSolver const *, usize, usize);
        static f64 FillDestination(IEquationSolver const *, const NLinalg::TMatrix&, usize, usize);
        static f64 FillProbabilities(IEquationSolver const *, const std::span<f64>&, usize, usize);
        static f64 CoefGMatrixLeft(IEquationSolver const *, usize, usize);
        static f64 CoefGMatrixRight(IEquationSolver const *, usize, usize);
        static f64 CoefGDestination(IEquationSolver const *, usize, usize);
        TRLFDESRule2() = delete;
        TRLFDESRule2(const TRLFDESRule2&) = delete;
        TRLFDESRule2(TRLFDESRule2&&) = delete;
        TRLFDESRule2& operator=(const TRLFDESRule2&) = delete;
        TRLFDESRule2& operator=(TRLFDESRule2&&) = delete;
        ~TRLFDESRule2() = delete;
    };

    static_assert(TMatrixFillRuleConcept<TMFDESRule>, "Wrong type TMFDESRule for TMatrixFillRuleConcept");
    static_assert(TMatrixFillRuleConcept<TRLFDESRule>, "Wrong type TRLFDESRule for TMatrixFillRuleConcept");
    static_assert(TMatrixFillRuleConcept<TRLFDESRule2>, "Wrong type TRLFDESRule2 for TMatrixFillRuleConcept");
    static_assert(TProbabilitiesFillRuleConcept<TMFDESRule>, "Wrong type TMFDESRule for TProbabilitiesFillRuleConcept");
    static_assert(TProbabilitiesFillRuleConcept<TRLFDESRule>, "Wrong type TRLFDESRule for TProbabilitiesFillRuleConcept");
}  // namespace NEquationSolver
