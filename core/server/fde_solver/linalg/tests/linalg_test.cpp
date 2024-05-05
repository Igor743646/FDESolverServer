#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include "../matrix.hpp"

TEST_CASE("MatrixConstructor", "[initialize]") {
    SECTION ("default constructor") {
        NLinalg::TMatrix matrix;

        REQUIRE(matrix.Shape() == std::pair{0, 0});
    }

    SECTION ("shape constructor with default value") {
        NLinalg::TMatrix matrix(2, 3, 0.2);

        REQUIRE(matrix.Shape() == std::pair{2, 3});
        for (int i = 0; i < 6; i++) {
            REQUIRE(matrix[i / 3][i % 3] == 0.2);
        }
    }
}