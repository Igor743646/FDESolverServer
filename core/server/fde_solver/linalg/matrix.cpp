#include "matrix.hpp"
#include <cassert>
#include <optional>
#ifdef __linux__
    #define data_prefetch(pointer) __builtin_prefetch((pointer), 0, 1)  
#else 
    #include <intrin.h>
    #define data_prefetch(pointer) _mm_prefetch((pointer), _MM_HINT_T1)    
#endif
#include <matrix.pb.h>

namespace NLinalg {

    TMatrix::TMatrix() 
    : Rows(0), Columns(0), Matrix(nullptr) {}

    TMatrix::TMatrix(usize rows, usize columns, f64 fill) 
    : Rows(rows), Columns(columns) {
        const usize length = BufferLength();
        Matrix = new f64[length];
        std::fill_n(Matrix, length, fill);
    }

    TMatrix::TMatrix(usize rows, usize columns, const std::vector<f64>& v)
    : Rows(rows), Columns(columns) {
        const usize length = BufferLength();
        Matrix = new f64[length];
        std::memcpy(Matrix, v.data(), length * sizeof(f64));
    }
    
    TMatrix::TMatrix(const TMatrix& matrix) 
    : Rows(matrix.Rows), Columns(matrix.Columns) {
        const usize length = BufferLength();
        Matrix = new f64[length];
        std::memcpy(Matrix, matrix.Matrix, length * sizeof(f64));
    }

    TMatrix::TMatrix(TMatrix&& matrix) 
    : Rows(matrix.Rows), Columns(matrix.Columns) {
        Matrix = matrix.Matrix;
        matrix.Matrix = nullptr;
    }

    TMatrix::TMatrix(usize rows) 
    : TMatrix(rows, rows) {
    }

    TMatrix::TMatrix(const std::vector<f64>& v) 
    : TMatrix(1, v.size(), v) {
    }

    TMatrix& TMatrix::operator=(const TMatrix& matrix) {
        if (BufferLength() != matrix.BufferLength()) {
            if (Matrix != nullptr) {
                delete[] Matrix;
            }

            Rows = matrix.Rows;
            Columns = matrix.Columns;
            Matrix = new f64[BufferLength()];
        }

        std::memcpy(Matrix, matrix.Matrix, BufferLength() * sizeof(f64));

        return *this;
    }

    TMatrix& TMatrix::operator=(TMatrix&& matrix) {
        Rows = matrix.Rows;
        Columns = matrix.Columns;
        Matrix = matrix.Matrix;
        matrix.Matrix = nullptr;

        return *this;
    }

    TMatrix::~TMatrix() {
        if (Matrix != nullptr) {
            delete[] Matrix;
        }
    }

    inline usize TMatrix::BufferLength() const {
        return Rows * Columns;
    }

    const std::span<f64> TMatrix::operator[](usize i) const {
        STACK_ASSERT(i < Rows, "i >= Rows");
        return std::span<f64>(Matrix + i * Columns, Columns);
    }

    std::span<f64> TMatrix::operator[](usize i) {
        STACK_ASSERT(i < Rows, "i >= Rows");
        return std::span<f64>(Matrix + i * Columns, Columns);
    }

    [[deprecated]] TMatrix operator*(const TMatrix& lhs, const TMatrix& rhs) {
        STACK_ASSERT(lhs.Columns == rhs.Rows, "lhs.Columns != rhs.Rows");
        TMatrix result(lhs.Rows, rhs.Columns);

        for (usize i = 0; i < lhs.Rows; i++) {
            for (usize j = 0; j < rhs.Columns; j++) {
                for (usize k = 0; k < lhs.Columns; k++) {
                    result[i][j] += lhs[i][k] * rhs[k][j];
                }
            }
        }

        return result;
    }

    [[deprecated]] std::vector<f64> operator*(const std::vector<f64>& lhs, const TMatrix& rhs) {
        STACK_ASSERT(lhs.size() == rhs.Rows, "lhs.size() != rhs.Rows");

        std::vector<f64> result(rhs.Columns, 0.0);
        for (usize i = 0; i < rhs.Columns; i++) {
            for (usize j = 0; j < lhs.size(); j++) {
                result[i] += rhs[j][i] * lhs[j];
            }
        }

        return result;
    }

    std::pair<usize, usize> TMatrix::Shape() const {
        return {Rows, Columns};
    }

    void TMatrix::SwapRows(usize i, usize j) {
        STACK_ASSERT(i < Rows && j < Rows, "i >= Rows or j >= Rows");
        if (i == j) {
            return;
        }

        for (usize idx = 0; idx < Columns; idx++) {
            std::swap(Matrix[i * Columns + idx], Matrix[j * Columns + idx]);
        }
    }

    void TMatrix::SwapColumns(usize i, usize j) {
        STACK_ASSERT(i < Columns && j < Columns, "i >= Columns or j >= Columns");
        if (i == j) {
            return;
        }

        for (usize idx = 0; idx < Rows; idx++) {
            std::swap(Matrix[idx * Columns + i], Matrix[idx * Columns + j]);
        }
    }

    TMatrix::TPluResult TMatrix::LUFactorizing() {
        STACK_ASSERT(Columns == Rows, "Columns != Rows");

        std::vector<usize> p(Rows);
        TMatrix lu(*this);

        for (usize i = 0; i < Rows; i++) {
            p[i] = i;
        }

        for (usize colId = 0; colId < Columns; colId++) {

            // 1. Находим строку с максимальным по модулю элементом.
            {
                usize rowMax = colId;
                for (usize rowId = colId + 1; rowId < Rows; rowId++) {
                    if (std::abs(lu[rowId][colId]) > std::abs(lu[rowMax][colId])) {
                        rowMax = rowId;
                    }
                }

                if (lu[rowMax][colId] == 0.0) {
                    continue;
                }

                // 2. Меняем строки в U и обновляем L.
                if (rowMax != colId) {
                    std::swap(p[colId], p[rowMax]);
                    lu.SwapRows(colId, rowMax);
                }
            }

            // 3. Алгоритм Гаусса
            for (usize rowId = colId + 1; rowId < Rows; rowId++) {
                const f64 koef = lu[rowId][colId] /= lu[colId][colId];

                for (usize t = colId + 1; t < Columns; t++) {
                    lu[rowId][t] -= koef * lu[colId][t];
                }
            }
        }

        return std::make_tuple(std::move(p), std::move(lu));
    }

    std::optional<std::vector<f64>> TMatrix::Solve(const std::vector<f64>& b) {
        // 1. Делаем LU - разложение
        auto plu = LUFactorizing();
        return TMatrix::Solve(plu, b);
    }

    std::optional<std::vector<f64>> TMatrix::Solve(const TPluResult& plu, const std::vector<f64>& b) {
        auto& [P, LU] = plu;

        STACK_ASSERT(P.size() == b.size(), "P.size() != b.size()");
        STACK_ASSERT(LU.Rows == LU.Columns && LU.Columns == b.size(), "Bad size plu with b vector");

        // 1. Вычисляем P^(T)b = bP = y (1 x n)
        std::vector<f64> y(P.size());

        for (usize i = 0; i < P.size(); i++) {
            y[i] = b[P[i]];
        }

        // 2. Вычисляем L * z = y;
        for (usize i = 0; i < P.size(); i++) {
            for (usize j = 0; j < i; j++) {
                y[i] -= LU[i][j] * y[j];
            }
        }

        // 3. Вычисляем U * x = z
        for (usize i = P.size(); i-- > 0;) {
            for (usize j = i + 1; j < P.size(); j++) {
                y[i] -= LU[i][j] * y[j];
            }

            if (std::abs(LU[i][i]) < EPSILON) {
                return std::nullopt;
            }
            
            y[i] /= LU[i][i];
        }

        return y;
    }

    TMatrix TMatrix::E(usize n) {
        TMatrix matrix(n);

        for (usize i = 0; i < n; i++) {
            matrix[i][i] = 1.0;
        }

        return matrix;
    }

    void TMatrix::TransponseQuad() {
        STACK_ASSERT(Rows == Columns, "Rows != Columns");
        for (usize i = 0; i < Rows - 1; i++) {
            for (usize j = i + 1; j < Columns; j++) {
                std::swap(Matrix[i * Columns + j], Matrix[j * Columns + i]);
            }
        }
    }

    std::ostream& operator<<(std::ostream& out, const TMatrix& m) {
        out << std::fixed;
        out.precision(9);
        out << "[";
        for (usize i = 0; i < m.Rows; i++) {
            out << ((i == 0) ? "[" : " [");
            for (usize j = 0; j < m.Columns; j++) {
                out << m[i][j];
                out << ((j + 1 < m.Columns) ? " " : "");
            }

            out << ((i + 1 < m.Rows) ? "]\n" : "]");
        }
        out << "]";
        return out;
    }

    PFDESolver::TMatrix TMatrix::ToProto() const {
        PFDESolver::TMatrix matrix;
        matrix.set_rows(Rows);
        matrix.set_columns(Columns);
        for (usize i = 0; i < Rows * Columns; i++) {
            matrix.add_data(Matrix[i]);
        }
        return matrix;
    }
}