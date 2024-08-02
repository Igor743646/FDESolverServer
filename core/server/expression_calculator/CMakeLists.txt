cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)
project(MathExpressionParser C CXX)

START_BUILD()

add_subdirectory(grammar)

add_library(${PROJECT_NAME} STATIC OBJECT
    "src/math_expression_calculator.cpp"
    "src/math_expression_calculator.hpp"
)
target_link_libraries(${PROJECT_NAME} PUBLIC
    MathExpressionParserGrammar
    StackTracer
    Utils
    MathUtils
    antlr4_static
)
target_include_directories(${PROJECT_NAME} PUBLIC 
    ${ANTLR_SampleGrammarLexer_OUTPUT_DIR}
    ${ANTLR_SampleGrammarParser_OUTPUT_DIR}
    ${ANTLR_SampleGrammarVisitor_OUTPUT_DIR}
    ${StackTracer_HEADERS_DIR}
    ${Utils_HEADERS_DIR}
    ${MathUtils_HEADERS_DIR}
)

DEFINE_LIB_HEADERS(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Tests

add_executable(${PROJECT_NAME}_test "src/tests/mec_test.cpp")

target_link_libraries(${PROJECT_NAME}_test PUBLIC
    Catch2::Catch2WithMain
    ${PROJECT_NAME}
)
target_include_directories(${PROJECT_NAME}_test PUBLIC 
    ${ANTLR_SampleGrammarLexer_OUTPUT_DIR}
    ${ANTLR_SampleGrammarParser_OUTPUT_DIR}
    ${ANTLR_SampleGrammarVisitor_OUTPUT_DIR}
    ${Catch2_INCLUDE_DIRS}
    ${MathExpressionParser_HEADERS_DIR}
)

