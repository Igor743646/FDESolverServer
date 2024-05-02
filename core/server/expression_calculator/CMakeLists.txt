cmake_minimum_required(VERSION 3.26.4 FATAL_ERROR)
project(ExpressionParser C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/generators/" "${CMAKE_PREFIX_PATH}")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/" "${CMAKE_MODULE_PATH}")

find_package(antlr4-runtime REQUIRED CONFIG)
include(def_antlr_target)

antlr_target(SampleGrammarLexer "grammar/TLexer.g4" LEXER
             PACKAGE antlrcpptest)
antlr_target(SampleGrammarParser "grammar/TParser.g4" PARSER
             PACKAGE antlrcpptest
             DEPENDS_ANTLR SampleGrammarLexer
             COMPILE_FLAGS -visitor -listener -lib ${ANTLR_SampleGrammarLexer_OUTPUT_DIR})

add_library(math_expression_calculator STATIC OBJECT
    math_expression_calculator.cpp math_expression_calculator.h
    ${ANTLR_SampleGrammarLexer_CXX_OUTPUTS}
    ${ANTLR_SampleGrammarParser_CXX_OUTPUTS}
    ${ANTLR_SampleGrammarVisitor_CXX_OUTPUTS}
)

target_link_libraries(math_expression_calculator PUBLIC antlr4_static)
target_include_directories(math_expression_calculator PUBLIC "${ANTLR_SampleGrammarLexer_OUTPUT_DIR}")
target_include_directories(math_expression_calculator PUBLIC "${ANTLR_SampleGrammarParser_OUTPUT_DIR}")
target_include_directories(math_expression_calculator PUBLIC "${ANTLR_SampleGrammarVisitor_OUTPUT_DIR}")
