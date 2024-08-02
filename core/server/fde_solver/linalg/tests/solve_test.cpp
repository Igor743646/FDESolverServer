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
    std::random_device device;
    std::knuth_b engine(device());
    std::normal_distribution dist(0.0, 20.0);

    auto getMatrix = [&](size_t size) -> NLinalg::TMatrix {
        NLinalg::TMatrix matrix(size, size);
        for (size_t i = 0; i < size; i++) {
            for (size_t j = 0; j < size; j++) {
                matrix[i][j] = dist(engine);
            }    
        }
        return matrix;
    };

    auto getVector = [&](size_t size) -> std::vector<f64> {
        std::vector<f64> data(size);
        for (size_t i = 0; i < size; i++) {
            data[i] = dist(engine);  
        }
        return data;
    };

    BENCHMARK_ADVANCED("Solve Equation 300x300 without constructor")(Catch::Benchmark::Chronometer meter) {
        const usize matrixSize = 300;
        auto matrix = getMatrix(matrixSize);
        auto vector = getVector(matrixSize);
        auto plu = matrix.LUFactorizing();
        REQUIRE(plu.has_value());
        meter.measure([&plu, &vector] { return NLinalg::TMatrix::Solve(plu.value(), vector); });
    };

    BENCHMARK_ADVANCED("Solve Equation 600x600 without constructor")(Catch::Benchmark::Chronometer meter) {
        const usize matrixSize = 600;
        auto matrix = getMatrix(matrixSize);
        auto vector = getVector(matrixSize);
        auto plu = matrix.LUFactorizing();
        REQUIRE(plu.has_value());
        meter.measure([&plu, &vector] { return NLinalg::TMatrix::Solve(plu.value(), vector); });
    };
}
