#pragma once

#include <math_utils.hpp>
#include <logger.hpp>
#include <stacktracer.hpp>
#include <fstream>
#include <span>

namespace PFDESolver {
    class TMatrix;
}

namespace NLinalg {
    class TMatrix {
    public:

        using TPluResult = std::tuple<std::vector<usize>, TMatrix>;

        TMatrix();
        TMatrix(usize, usize, f64 = 0.0);
        TMatrix(usize, usize, const std::vector<f64>&);
        TMatrix(usize, usize, const std::vector<std::vector<f64>>&);
        TMatrix(const TMatrix&);
        TMatrix(TMatrix&&);
        explicit TMatrix(const std::vector<f64>&);
        explicit TMatrix(usize);
        TMatrix& operator=(const TMatrix&);
        TMatrix& operator=(TMatrix&&);
        ~TMatrix();

        const f64* Data() const noexcept;
        size_t BufferLength() const noexcept;
        std::pair<usize, usize> Shape() const noexcept;
        void Reset() noexcept;
        void SwapRows(usize, usize);
        void SwapColumns(usize, usize);

        static TMatrix E(usize);
        void TransponseQuad();
        std::optional<TPluResult> LUFactorizing();

        /// @brief Метод решения линейного матричного уравнения через PLU разложение \\
        /// @brief Ax = b => PLUx = b => LUx = P^(-1)b = P^(T)b
        /// @param b вектор значений с правой стороны уравнения
        /// @exception Метод вызывает исключение, если матрица не квадратная
        /// @return Возвращает x - решение системы уравнений вида Ax = b 
        std::optional<std::vector<f64>> Solve(const std::vector<f64>&);
        static std::optional<std::vector<f64>> Solve(const TPluResult&, const std::vector<f64>&);
        
        const std::span<f64> operator[](usize) const;
        std::span<f64> operator[](usize);
        friend TMatrix operator*(const TMatrix&, const TMatrix&);
        friend std::vector<f64> operator*(const std::vector<f64>&, const TMatrix&);
        bool operator==(const TMatrix&) const;

        friend std::ostream& operator<<(std::ostream& out, const TMatrix& m);
        PFDESolver::TMatrix ToProto() const;

    private:

        usize Rows, Columns;
        f64* Matrix;
    };
}
