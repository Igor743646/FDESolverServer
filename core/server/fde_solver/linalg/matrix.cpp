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

        if (Matrix != nullptr) {
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
        if (Matrix != nullptr) {
            delete[] Matrix;
        }
    }

    std::span<f64> TMatrix::operator[](usize i) const {
        NStackTracer::Assert(i < Rows, std::format("i = {} >= Rows = {}", i, Rows));
        return std::span<f64>(Matrix + i * Columns, Columns);
    }

    std::span<f64> TMatrix::operator[](usize i) {
        NStackTracer::Assert(i < Rows, std::format("i = {} >= Rows = {}", i, Rows));
        return std::span<f64>(Matrix + i * Columns, Columns);
    }

    TMatrix operator*(const TMatrix& lhs, const TMatrix& rhs) {
        NStackTracer::Assert(lhs.Columns == rhs.Rows, 
                             std::format("lhs.Columns = {} != rhs.Rows = {}", lhs.Columns, rhs.Rows));

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

        if (i == j) {
            return;
        }

        for (usize idx = 0; idx < Columns; idx++) {
            std::swap(Matrix[i * Columns + idx], Matrix[j * Columns + idx]);
        }
    }

    void TMatrix::SwapColumns(usize i, usize j) {
        assert(i < Columns && j < Columns);

        if (i == j) {
            return;
        }

        for (usize idx = 0; idx < Rows; idx++) {
            std::swap(Matrix[idx * Columns + i], Matrix[idx * Columns + j]);
        }
    }

    TMatrix::TPluResult TMatrix::LUFactorizing() {
        assert(Columns == Rows);

        std::vector<usize> p(Rows);
        TMatrix l = E(Rows);
        TMatrix u(*this);

        for (usize i = 0; i < Rows; i++) {
            p[i] = i;
        }

        for (usize colId = 0; colId < Columns; colId++) {

            // 1. Находим строку с максимальным по модулю элементом.
            {
                usize rowMax = colId;
                for (usize rowId = colId + 1; rowId < Rows; rowId++) {
                    if (std::abs(u[rowId][colId]) > std::abs(u[rowMax][colId])) {
                        rowMax = rowId;
                    }
                }

                if (u[rowMax][colId] == 0) {
                    continue;
                }

                // 2. Меняем строки в U и обновляем L.
                if (rowMax != colId) {
                    std::swap(p[colId], p[rowMax]);
                    l.SwapRows(colId, rowMax);
                    l.SwapColumns(colId, rowMax);
                    u.SwapRows(colId, rowMax);
                }
            }

            // 3. Алгоритм Гаусса
            for (usize rowId = colId + 1; rowId < Rows; rowId++) {
                f64 koef = u[rowId][colId] / u[colId][colId];

                u[rowId][colId] = 0;
                l[rowId][colId] = koef;

                for (usize t = colId + 1; t < Columns; t++) {
                    u[rowId][t] -= koef * u[colId][t];
                }
            }
        }

        for (usize rowId = 0; rowId < Rows; rowId++) {
            std::memcpy(&l[rowId][rowId], &u[rowId][rowId], (Columns - rowId) * sizeof(f64));
        }

        return std::make_tuple(std::move(p), std::move(l));
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