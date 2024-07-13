#pragma once

#include <math_utils.hpp>
#include <functional>
#include <matrix.hpp>

namespace PFDESolver {
    struct TSolverConfig;
}

namespace NEquationSolver {

    /// @brief This structure contains fields that are standard data types.
    /// @brief There are no function.
    struct TSolverConfigBase {
        usize SpaceCount, TimeCount;    // количество ячеек по x и t координатах соответственно

        f64 LeftBound, RightBound;   // границы отрезка сетки по x координате
        f64 MaxTime;                 // граница отрезка времени по t координате
        f64 Alpha, Gamma;            // степени производных по x и t координатах соответственно
        f64 SpaceStep, TimeStep;     // шаги по сетки по x и t координатах соответственно
        f64 Beta;                    // коэффициент доли лево/правосторонней производных [-1; +1]

        f64 AlphaLeft, BetaLeft;     // коэффициенты граничных условий третьего рода для x == L
        f64 AlphaRight, BetaRight;   // коэффициенты граничных условий третьего рода для x == R

        bool BordersAvailable;                  // стоит ли учитывать граничные условия
        usize StochasticIterationCount = 10;    // количество итераций для стохастического алгоритма

        std::optional<std::string> RealSolutionName;                // latex формула функции c эталонным решением обрамленная $$
    };

    struct TParsedSolverConfig : public TSolverConfigBase {
        std::function<f64(f64)> DiffusionCoefficient;               // коэффициент диффузии при дробной производной по пространству
        std::function<f64(f64)> DemolitionCoefficient;              // коэффициент сноса при производной первой степени
        std::function<f64(f64)> ZeroTimeState;                      // начальное условие при t = 0, u(x, 0) = psi(x)
        std::function<f64(f64, f64)> SourceFunction;                // функция источник
        std::function<f64(f64)> LeftBoundState;                     // граничное условие u(L, t) = phiL(t)
        std::function<f64(f64)> RightBoundState;                    // граничное условие u(R, t) = phiR(t)
        std::optional<std::function<f64(f64, f64)>> RealSolution;   // функция с эталонным решением
    };

    struct TSolverConfig : TSolverConfigBase {
        std::vector<f64> DiffusionCoefficient;              // коэффициент диффузии при дробной производной по пространству
        std::vector<f64> DemolitionCoefficient;             // коэффициент сноса при производной первой степени
        std::vector<f64> ZeroTimeState;                     // начальное условие при t = 0, u(x, 0) = psi(x)
        NLinalg::TMatrix SourceFunction;                    // функция источник
        std::vector<f64> LeftBoundState;                    // граничное условие u(L, t) = phiL(t)
        std::vector<f64> RightBoundState;                   // граничное условие u(R, t) = phiR(t)
        std::optional<NLinalg::TMatrix> RealSolution;       // функция с эталонным решением

        TSolverConfig(const TSolverConfig&) = default;
        TSolverConfig(TSolverConfig&&) = default;
        TSolverConfig& operator=(const TSolverConfig&) = default;
        TSolverConfig& operator=(TSolverConfig&&) = default;
        TSolverConfig(const TParsedSolverConfig&);

        ::PFDESolver::TSolverConfig ToProto() const;
        friend std::ostream& operator<<(std::ostream&, const TSolverConfig&);
    };
}
