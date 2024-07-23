#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <matrix.hpp>
#include <random>

TEST_CASE("Solve Equation", "[plu, solve]") {
    SECTION ("Solve 1x1") {
        NLinalg::TMatrix matrix(1, 1, {3.0});

        auto plu = matrix.LUFactorizing().value();

        auto result = NLinalg::TMatrix::Solve(plu, std::vector<f64>({2}));

        REQUIRE(result.has_value());
        REQUIRE(result.value()[0] == 0.6666666666666666666666667);
    }
}


TEST_CASE("Solve Equation Benchmark", "[plu, solve, benchmark]") {
    auto getMatrix = [](size_t size) -> NLinalg::TMatrix {
        NLinalg::TMatrix matrix(size, size);
        for (size_t i = 0; i < size; i++) {
            for (size_t j = 0; j < size; j++) {
                matrix[i][j] = (f64)(std::rand() % 20) + (f64)(std::rand() % 1000000) / 1000000.;
            }    
        }
        return matrix;
    };

    auto getVector = [](size_t size) -> std::vector<f64> {
        std::vector<f64> v(size);
        for (size_t i = 0; i < size; i++) {
            v[i] = (f64)(std::rand() % 20) + (f64)(std::rand() % 1000) / 1000.;  
        }
        return v;
    };

    BENCHMARK_ADVANCED("Solve Equation 300x300 without constructor")(Catch::Benchmark::Chronometer meter) {
        auto matrix = getMatrix(300);
        auto vector = getVector(300);
        auto plu = matrix.LUFactorizing();
        REQUIRE(plu.has_value());
        meter.measure([&plu, &vector] { return NLinalg::TMatrix::Solve(plu.value(), vector); });
    };

    BENCHMARK_ADVANCED("Solve Equation 600x600 without constructor")(Catch::Benchmark::Chronometer meter) {
        auto matrix = getMatrix(600);
        auto vector = getVector(600);
        auto plu = matrix.LUFactorizing();
        REQUIRE(plu.has_value());
        meter.measure([&plu, &vector] { return NLinalg::TMatrix::Solve(plu.value(), vector); });
    };
}
