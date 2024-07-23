#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <matrix.hpp>
#include <random>

TEST_CASE("PLU Decomposition", "[plu]") {
    SECTION ("PLU 1x1") {
        NLinalg::TMatrix matrix(1, 1, {3.0});

        auto [P, LU] = matrix.LUFactorizing().value();

        REQUIRE(P[0] == 0);
        REQUIRE(LU[0][0] == 3.0);
    }

    SECTION ("PLU 2x2") {
        NLinalg::TMatrix matrix(2, 2, {
            3., 4., 
            2., 5.,
        });
        NLinalg::TMatrix answer(2, 2, {
            3., 4., 
            2./3., 7./3.,
        });

        auto [P, LU] = matrix.LUFactorizing().value();

        REQUIRE_THAT(P, Catch::Matchers::Equals(std::vector<usize>{ 0, 1 }));
        REQUIRE(LU == answer);
    }

    SECTION ("PLU 3x3 with nulls column") {
        NLinalg::TMatrix matrix(3, 3, {
            3.,     1., 1.,
            3.,     1., 2.,
            1., 1. / 3, 3.,
        });

        REQUIRE(matrix.LUFactorizing().has_value() == false);
    }

    SECTION ("PLU 5x5") {
        NLinalg::TMatrix matrix(5, 5, {
            {4, 4, 5, 9, 0},
            {2, 6, 4, 7, 2},
            {2, 9, 2, 5, 9},
            {8, 2, 9, 7, 2},
            {4, 4, 3, 3, 1},
        });
        NLinalg::TMatrix answer(5, 5, {
            {     8,        2,         9,         7,         2},
            {1. / 4,  17. / 2,   -1. / 4,   13. / 4,   17. / 2},
            {1. / 4, 11. / 17,  65. / 34, 107. / 34,        -4},
            {1. / 2,  6. / 17,   4. / 13,  44. / 13, -36. / 13},
            {1. / 2,  6. / 17, -48. / 65,    1. / 5,  -27. / 5},
        });

        auto [P, LU] = matrix.LUFactorizing().value();

        REQUIRE(LU == answer);
        REQUIRE_THAT(P, Catch::Matchers::Equals(std::vector<usize>{ 3, 2, 1, 0, 4 }));
    }
}


TEST_CASE("PLU Benchmark", "[plu, benchmark]") {
    auto getMatrix = [](size_t size) -> NLinalg::TMatrix {
        NLinalg::TMatrix matrix(size, size);
        for (size_t i = 0; i < size; i++) {
            for (size_t j = 0; j < size; j++) {
                matrix[i][j] = (f64)(i * size + j);
            }    
        }
        return matrix;
    };

    BENCHMARK_ADVANCED("PLU 300x300 without constructor")(Catch::Benchmark::Chronometer meter) {
        NLinalg::TMatrix matrix = getMatrix(300);
        meter.measure([&matrix] { return matrix.LUFactorizing(); });
    };

    BENCHMARK_ADVANCED("PLU 600x600 without constructor")(Catch::Benchmark::Chronometer meter) {
        NLinalg::TMatrix matrix = getMatrix(600);
        meter.measure([&matrix] { return matrix.LUFactorizing(); });
    };
}
