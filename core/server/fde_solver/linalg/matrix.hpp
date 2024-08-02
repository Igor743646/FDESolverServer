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
        TMatrix(TMatrix&&) noexcept;
        explicit TMatrix(const std::vector<f64>&);
        explicit TMatrix(usize);
        TMatrix& operator=(const TMatrix&);
        TMatrix& operator=(TMatrix&&) noexcept;
        ~TMatrix();

        [[nodiscard]] const f64* Data() const noexcept;
        [[nodiscard]] size_t BufferLength() const noexcept;
        [[nodiscard]] std::pair<usize, usize> Shape() const noexcept(
            noexcept(std::pair<usize, usize>(Rows, Columns))
        );
        void Reset() noexcept;
        void SwapRows(usize, usize);
        void SwapColumns(usize, usize);

        static TMatrix E(usize);
        void TransponseQuad();
        [[nodiscard]] std::optional<TPluResult> LUFactorizing() const;

        /// @brief Метод решения линейного матричного уравнения через PLU разложение
        /// @brief Ax = b => PLUx = b => LUx = P^(-1)b = P^(T)b
        /// @param b вектор значений с правой стороны уравнения
        /// @exception Метод вызывает исключение, если матрица не квадратная
        /// @return Возвращает x - решение системы уравнений вида Ax = b 
        [[nodiscard]] std::optional<std::vector<f64>> Solve(const std::vector<f64>&) const;
        static std::optional<std::vector<f64>> Solve(const TPluResult&, const std::vector<f64>&);
        
        std::span<f64> operator[](usize) const;
        std::span<f64> operator[](usize);
        friend TMatrix operator*(const TMatrix&, const TMatrix&);
        friend std::vector<f64> operator*(const std::vector<f64>&, const TMatrix&);
        bool operator==(const TMatrix&) const;

        friend std::ostream& operator<<(std::ostream&, const TMatrix&);
        [[nodiscard]] PFDESolver::TMatrix ToProto() const;

    private:

        usize Rows, Columns;
        alignas(gCACHELINE_SIZE) f64* Matrix;
    };
}  // namespace NLinalg
