#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <matrix.hpp>
#include <matrix.pb.h>
#include <random>

TEST_CASE("Matrix Constructor", "[initialize]") {
    SECTION ("Default constructor") {
        NLinalg::TMatrix matrix;

        REQUIRE(matrix.Shape() == std::pair<usize, usize>{0, 0});
        REQUIRE(matrix.Data() == nullptr);
    }

    SECTION ("Shape constructor with default value") {
        NLinalg::TMatrix matrix(2, 3, 0.2);

        REQUIRE(matrix.Shape() == std::pair<usize, usize>{2, 3});
        for (int i = 0; i < matrix.BufferLength(); i++) {
            REQUIRE(matrix[i / 3][i % 3] == 0.2);
        }
    }

    SECTION ("Constructor with one size value") {
        NLinalg::TMatrix matrix(3);

        REQUIRE(matrix.Shape() == std::pair<usize, usize>{3, 3});
        for (int i = 0; i < matrix.BufferLength(); i++) {
            REQUIRE(matrix[i / 3][i % 3] == 0.0);
        }
    }

    SECTION ("Shape constructor with vector init") {
        const NLinalg::TMatrix matrix(3, 2, {1, 2, 3, 4, 5, 6});

        REQUIRE(matrix.Shape() == std::pair<usize, usize>{3, 2});
        for (int i = 0; i < matrix.BufferLength(); i++) {
            REQUIRE(matrix[i / 2][i % 2] == i + 1);
        }
    }

    SECTION ("Constructor with vector init") {
        const NLinalg::TMatrix matrix({1, 2, 3, 4, 5, 6});

        REQUIRE(matrix.Shape() == std::pair<usize, usize>{1, 6});
        for (int i = 0; i < matrix.BufferLength(); i++) {
            REQUIRE(matrix[0][i] == i + 1);
        }
    }

    SECTION ("Copy constructor") {
        const NLinalg::TMatrix matrix1(3, 2, {1, 2, 3, 4, 5, 6});
        NLinalg::TMatrix matrix2(matrix1);

        REQUIRE(matrix1.Shape() == std::pair<usize, usize>{3, 2});
        REQUIRE(matrix2.Shape() == std::pair<usize, usize>{3, 2});
        for (int i = 0; i < matrix2.BufferLength(); i++) {
            REQUIRE(matrix1[i / 2][i % 2] == i + 1);
            REQUIRE(matrix2[i / 2][i % 2] == i + 1);
            REQUIRE(matrix1.Data() != matrix2.Data());
        }
    }

    SECTION ("Move constructor") {
        NLinalg::TMatrix matrix1(3, 2, {1, 2, 3, 4, 5, 6}); // NOLINT(readability-magic-numbers)
        NLinalg::TMatrix matrix2(std::move(matrix1));

        REQUIRE(matrix1.Shape() == std::pair<usize, usize>{0, 0});
        REQUIRE(matrix1.Data() == nullptr);
        REQUIRE(matrix2.Shape() == std::pair<usize, usize>{3, 2});
        for (int i = 0; i < matrix2.BufferLength(); i++) {
            REQUIRE(matrix2[i / 2][i % 2] == i + 1);
        }
    }
}


TEST_CASE("Matrix Functions", "[func]") {
    SECTION ("ToProto") {
        const NLinalg::TMatrix matrix(3, 2, {1, 2, 3, 4, 5, 6});
        PFDESolver::TMatrix pbMatrix = matrix.ToProto();

        REQUIRE(pbMatrix.rows() == 3);
        REQUIRE(pbMatrix.columns() == 2);
        REQUIRE(pbMatrix.data_size() == 6);
        for (int i = 0; i < matrix.BufferLength(); i++) {
            REQUIRE(pbMatrix.data(i) == matrix[i / 2][i % 2]);
        }
    }
}



TEST_CASE("Matrix Functions Benchmark", "[func, benchmark]") {
    auto getMatrix = [](size_t size) -> NLinalg::TMatrix {
        NLinalg::TMatrix matrix(size, size);
        for (size_t i = 0; i < size; i++) {
            for (size_t j = 0; j < size; j++) {
                matrix[i][j] = static_cast<f64>(i * size + j);
            }    
        }
        return matrix;
    };

    BENCHMARK_ADVANCED("SwapRow 600x600 without constructor")(Catch::Benchmark::Chronometer meter) {
        const usize matrixSize = 600;
        NLinalg::TMatrix matrix = getMatrix(matrixSize);
        std::srand(static_cast<ui32>(std::time(nullptr)));
        auto i = static_cast<usize>(std::rand() % matrixSize);
        auto j = static_cast<usize>(std::rand() % matrixSize);
        meter.measure([i, j, &matrix] () { matrix.SwapRows(i, j); });
    };

    BENCHMARK_ADVANCED("SwapRow 5000x5000 without constructor")(Catch::Benchmark::Chronometer meter) {
        const usize matrixSize = 5000;
        NLinalg::TMatrix matrix = getMatrix(matrixSize);
        std::srand(static_cast<ui32>(std::time(nullptr)));
        auto i = static_cast<usize>(std::rand() % matrixSize);
        auto j = static_cast<usize>(std::rand() % matrixSize);
        meter.measure([i, j, &matrix] () { matrix.SwapRows(i, j); });
    };
}
