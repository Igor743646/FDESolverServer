#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <math_expression_calculator.hpp>

constexpr std::array<std::pair<const char*, double>, 19> DATA1 = {
    std::pair{"", 0},
    std::pair{"3 + 3", 6},
    std::pair{"3 - 3", 0},
    std::pair{"3 * 3", 9},
    std::pair{"3 / 3", 1},
    std::pair{"0 / 3", 0},
    std::pair{"0 * 100000", 0},
    std::pair{"(5)", 5},
    std::pair{"0 * (0 - 0)", 0},
    std::pair{"3000 - (20000)", -17000},
    std::pair{"100 / 25", 4},
    std::pair{"3 - 7 * (5 - 1)", -25},
    std::pair{"((((3.3)) + (((3.2)))))", 6.5},
    std::pair{"2 - 2 * 2 / 2 - 2 + 2", 0},
    std::pair{"-3 * -3", 9},
    std::pair{"2 ? 1 ? 5 : 3 : 0", 5},
    std::pair{"0 ? (1 ? 5 : 3) : 0", 0},
    std::pair{"sin(3.14159265358979323846264)", 0},
    std::pair{"pow(2, 1 + sin(3.14159265358979323846264))", 2},
};

TEST_CASE("Math Expression Calculator", "[initialize]") {
    SECTION ("Without parameters") {
        for (auto& [test_expr, result] : DATA1) {
            INFO("expr: " << test_expr);
            ANTLRMathExpParser::MathExpressionCalculator mec(test_expr, {});
            double val = mec.Calc();
            REQUIRE_THAT(val, Catch::Matchers::WithinAbs(result, 0.0000001));
        }
    }

    SECTION ("Random") {
        ANTLRMathExpParser::MathExpressionCalculator mec("randomf()", {});
        double val1 = mec.Calc();
        double val2 = mec.Calc();
        REQUIRE(val1 != val2);
    }
}
