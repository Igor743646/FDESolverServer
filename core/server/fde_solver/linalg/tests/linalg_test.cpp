#include <catch2/catch_test_macros.hpp>
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
}