cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)
project(MathExpressionParser C CXX)

set(CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/Release/generators/" "${CMAKE_PREFIX_PATH}")
set(CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/generators/" "${CMAKE_PREFIX_PATH}")

find_package(antlr4-runtime REQUIRED CONFIG)

add_subdirectory(grammar)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/generators/" "${CMAKE_PREFIX_PATH}")

add_library(${PROJECT_NAME} STATIC OBJECT
    "src/math_expression_calculator.cpp"
    "src/math_expression_calculator.hpp"
)

target_link_libraries(${PROJECT_NAME} PUBLIC MathExpressionParserGrammar StackTracer Utils antlr4_static)
target_include_directories(${PROJECT_NAME} PUBLIC "${ANTLR_SampleGrammarLexer_OUTPUT_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${ANTLR_SampleGrammarParser_OUTPUT_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${ANTLR_SampleGrammarVisitor_OUTPUT_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/utils/stacktracer")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/utils/other")

# Tests

find_package(Catch2 REQUIRED CONFIG)

add_executable(${PROJECT_NAME}_test "src/tests/mec_test.cpp")

target_link_libraries(${PROJECT_NAME}_test PUBLIC Catch2::Catch2WithMain ${PROJECT_NAME} MathExpressionParserGrammar antlr4_static)
target_include_directories(${PROJECT_NAME}_test PUBLIC "${ANTLR_SampleGrammarLexer_OUTPUT_DIR}")
target_include_directories(${PROJECT_NAME}_test PUBLIC "${ANTLR_SampleGrammarParser_OUTPUT_DIR}")
target_include_directories(${PROJECT_NAME}_test PUBLIC "${ANTLR_SampleGrammarVisitor_OUTPUT_DIR}")
target_include_directories(${PROJECT_NAME}_test PUBLIC ${Catch2_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME}_test PUBLIC "src")
