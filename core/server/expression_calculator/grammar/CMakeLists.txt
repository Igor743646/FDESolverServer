cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)
project(ExpressionParserGrammar C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)
set(CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/Release/generators/" "${CMAKE_PREFIX_PATH}")
set(CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/generators/" "${CMAKE_PREFIX_PATH}")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake/" "${CMAKE_MODULE_PATH}")

find_package(antlr4-runtime REQUIRED CONFIG)
include(def_antlr_target)

antlr_target(SampleGrammarLexer "TLexer.g4" LEXER
             PACKAGE ANTLRMathExpParser)

antlr_target(SampleGrammarParser "TParser.g4" PARSER
             PACKAGE ANTLRMathExpParser
             DEPENDS_ANTLR SampleGrammarLexer
             COMPILE_FLAGS -visitor -listener -atn -lib "${ANTLR_SampleGrammarLexer_OUTPUT_DIR}")

add_library(math_expression_grammar STATIC
    ${ANTLR_SampleGrammarLexer_CXX_OUTPUTS}
    ${ANTLR_SampleGrammarParser_CXX_OUTPUTS}
    ${ANTLR_SampleGrammarVisitor_CXX_OUTPUTS}
)

target_link_libraries(math_expression_grammar PUBLIC antlr4_static)
target_include_directories(math_expression_grammar PUBLIC "${ANTLR_SampleGrammarLexer_OUTPUT_DIR}")
target_include_directories(math_expression_grammar PUBLIC "${ANTLR_SampleGrammarParser_OUTPUT_DIR}")
target_include_directories(math_expression_grammar PUBLIC "${ANTLR_SampleGrammarVisitor_OUTPUT_DIR}")
