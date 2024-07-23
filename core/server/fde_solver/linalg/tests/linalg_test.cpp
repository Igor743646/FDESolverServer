#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <matrix.hpp>

TEST_CASE("MatrixConstructor", "[initialize]") {
    SECTION ("default constructor") {
        NLinalg::TMatrix matrix;

        REQUIRE(matrix.Shape() == std::pair<usize, usize>{0, 0});
    }

    SECTION ("shape constructor with default value") {
        NLinalg::TMatrix matrix(2, 3, 0.2);

        REQUIRE(matrix.Shape() == std::pair<usize, usize>{2, 3});
        for (int i = 0; i < 6; i++) {
            REQUIRE(matrix[i / 3][i % 3] == 0.2);
        }
    }

    SECTION ("default constructor with one size value") {
        NLinalg::TMatrix matrix(3);

        REQUIRE(matrix.Shape() == std::pair<usize, usize>{3, 3});
        for (int i = 0; i < 9; i++) {
            REQUIRE(matrix[i / 3][i % 3] == 0.0);
        }
    }

    SECTION ("constructor with vector init") {
        NLinalg::TMatrix matrix(3, 2, {1, 2, 3, 4, 5, 6});

        REQUIRE(matrix.Shape() == std::pair<usize, usize>{3, 2});
        for (int i = 0; i < 6; i++) {
            REQUIRE(matrix[i / 2][i % 2] == i + 1);
        }
    }

    auto pluTest = [](size_t size) -> NLinalg::TMatrix::TPluResult {
        NLinalg::TMatrix matrix(size, size);
        for (size_t i = 0; i < size; i++) {
            for (size_t j = 0; j < size; j++) {
                matrix[i][j] = (double)(i * size + j);
            }    
        }

        return matrix.LUFactorizing();
    };

    BENCHMARK ("PLU 300x300") {
       return pluTest(300);
    };

    BENCHMARK ("PLU 600x600") {
        return pluTest(600);
    };
}
