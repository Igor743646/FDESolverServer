cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)
project(ExpressionParser C CXX)

add_subdirectory(grammar)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/generators/" "${CMAKE_PREFIX_PATH}")

add_library(math_expression_calculator STATIC
    "src/math_expression_calculator.cpp"
    "src/math_expression_calculator.hpp"
)

target_link_libraries(math_expression_calculator PUBLIC math_expression_grammar)
target_include_directories(math_expression_calculator PUBLIC "${ANTLR_SampleGrammarLexer_OUTPUT_DIR}")
target_include_directories(math_expression_calculator PUBLIC "${ANTLR_SampleGrammarParser_OUTPUT_DIR}")
target_include_directories(math_expression_calculator PUBLIC "${ANTLR_SampleGrammarVisitor_OUTPUT_DIR}")

# Tests

find_package(Catch2 REQUIRED CONFIG)

add_executable(math_expression_calculator_test "src/tests/mec_test.cpp")

target_link_libraries(math_expression_calculator_test PUBLIC Catch2::Catch2WithMain math_expression_calculator)
target_include_directories(math_expression_calculator_test PUBLIC "${ANTLR_SampleGrammarLexer_OUTPUT_DIR}")
target_include_directories(math_expression_calculator_test PUBLIC "${ANTLR_SampleGrammarParser_OUTPUT_DIR}")
target_include_directories(math_expression_calculator_test PUBLIC "${ANTLR_SampleGrammarVisitor_OUTPUT_DIR}")
target_include_directories(math_expression_calculator_test PUBLIC ${Catch2_INCLUDE_DIRS})
target_include_directories(math_expression_calculator_test PUBLIC "src")
