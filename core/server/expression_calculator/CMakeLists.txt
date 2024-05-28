cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)
project(ExpressionParser C CXX)

add_subdirectory(grammar)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(math_expression_calculator STATIC
    "src/math_expression_calculator.cpp"
    "src/math_expression_calculator.h"
)

target_link_libraries(math_expression_calculator PUBLIC math_expression_grammar)
target_include_directories(math_expression_calculator PUBLIC "${ANTLR_SampleGrammarLexer_OUTPUT_DIR}")
target_include_directories(math_expression_calculator PUBLIC "${ANTLR_SampleGrammarParser_OUTPUT_DIR}")
target_include_directories(math_expression_calculator PUBLIC "${ANTLR_SampleGrammarVisitor_OUTPUT_DIR}")
