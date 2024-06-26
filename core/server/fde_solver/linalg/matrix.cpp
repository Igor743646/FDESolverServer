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
        usize length = rows * columns;
        Matrix = new f64[length];
        std::fill_n(Matrix, length, fill);
    }

    TMatrix::TMatrix(usize rows, usize columns, const std::vector<f64>& v)
    : TMatrix(rows, columns) {
        std::memcpy(Matrix, v.data(), Rows * Columns * sizeof(f64));
    }
    
    TMatrix::TMatrix(const TMatrix& matrix) 
    : TMatrix(matrix.Rows, matrix.Columns) {
        std::memcpy(Matrix, matrix.Matrix, Rows * Columns * sizeof(f64));
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
    : TMatrix(1, v.size()) {
        std::memcpy(Matrix, v.data(), Rows * Columns * sizeof(f64));
    }

    TMatrix& TMatrix::operator=(const TMatrix& matrix) {
        Rows = matrix.Rows;
        Columns = matrix.Columns;

        if (Matrix) {
            delete[] Matrix;
        }

        Matrix = new f64[Rows * Columns];
        std::memcpy(Matrix, matrix.Matrix, Rows * Columns * sizeof(f64));

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
        if (Matrix) {
            delete[] Matrix;
        }
    }

    const f64* TMatrix::operator[](usize i) const {
        assert(i < Rows);
        return static_cast<const f64*>(Matrix + i * Columns);
    }

    f64* TMatrix::operator[](usize i) {
        assert(i < Rows);
        return (Matrix + i * Columns);
    }

    TMatrix operator*(const TMatrix& lhs, const TMatrix& rhs) {
        assert(lhs.Columns == rhs.Rows);

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

    std::vector<f64> operator*(const std::vector<f64>& lhs, const TMatrix& rhs) {
        assert(lhs.size() == rhs.Rows);

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
        assert(i < Rows && j < Rows);

        if (i == j)
            return;

        for (usize idx = 0; idx < Columns; idx++) {
            std::swap(Matrix[i * Columns + idx], Matrix[j * Columns + idx]);
        }
    }

    void TMatrix::SwapColumns(usize i, usize j) {
        assert(i < Columns && j < Columns);

        if (i == j)
            return;

        for (usize idx = 0; idx < Rows; idx++) {
            std::swap(Matrix[idx * Columns + i], Matrix[idx * Columns + j]);
        }
    }

    TMatrix::TPluResult TMatrix::LUFactorizing() {
        assert(Columns == Rows);

        std::vector<usize> P(Rows);
        TMatrix L = E(Rows);
        TMatrix U(*this);

        for (usize i = 0; i < Rows; i++) {
            P[i] = i;
        }

        for (usize colId = 0; colId < Columns; colId++) {

            // 1. Находим строку с максимальным по модулю элементом.
            {
                usize rowMax = colId;
                for (usize rowId = colId + 1; rowId < Rows; rowId++) {
                    if (std::abs(U[rowId][colId]) > std::abs(U[rowMax][colId])) {
                        rowMax = rowId;
                    }
                }

                if (U[rowMax][colId] == 0) {
                    continue;
                }

                // 2. Меняем строки в U и обновляем L.
                if (rowMax != colId) {
                    std::swap(P[colId], P[rowMax]);
                    L.SwapRows(colId, rowMax);
                    L.SwapColumns(colId, rowMax);
                    U.SwapRows(colId, rowMax);
                }
            }

            // 3. Алгоритм Гаусса
            for (usize rowId = colId + 1; rowId < Rows; rowId++) {
                f64 koef = U[rowId][colId] / U[colId][colId];

                U[rowId][colId] = 0;
                L[rowId][colId] = koef;

                for (usize t = colId + 1; t < Columns; t++) {
                    U[rowId][t] -= koef * U[colId][t];
                }
            }
        }

        for (usize rowId = 0; rowId < Rows; rowId++) {
            std::memcpy(&L[rowId][rowId], &U[rowId][rowId], (Columns - rowId) * sizeof(f64));
        }

        return std::make_tuple(std::move(P), std::move(L));
    }

    std::optional<std::vector<f64>> TMatrix::Solve(const std::vector<f64>& b) {
        assert(Rows == Columns && Columns == b.size());

        // 1. Делаем LU - разложение
        auto plu = LUFactorizing();

        return TMatrix::Solve(plu, b);
    }

    std::optional<std::vector<f64>> TMatrix::Solve(const TPluResult& plu, const std::vector<f64>& b) {
        auto& [P, LU] = plu;

        assert(P.size() == b.size());
        assert(LU.Shape().first == LU.Shape().second && LU.Shape().second == b.size());

        // 1. Вычисляем P^(T)b = bP = y (1 x n)
        std::vector<f64> y(P.size());
        data_prefetch((const char*)y.data());

        for (usize i = 0; i < P.size(); i++) {
            y[i] = b[P[i]];
        }

        // 2. Вычисляем L * z = y;
        for (usize i = 0; i < P.size(); i++) {
            data_prefetch((const char*)LU[i]);
            for (usize j = 0; j < i; j++) {
                y[i] -= LU[i][j] * y[j];
            }
        }

        // 3. Вычисляем U * x = z
        for (usize i = P.size(); i-- > 0;) {
            data_prefetch((const char*)LU[i]);
            for (usize j = i + 1; j < P.size(); j++) {
                y[i] -= LU[i][j] * y[j];
            }

            if (std::abs(LU[i][i]) < EPSILON) 
                return std::nullopt;

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