cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)
project(MathExpressionParserGrammar C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED 23)
set(CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/Debug/generators/" "${CMAKE_PREFIX_PATH}")
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

message(STATUS "ANTLR4 Lexer files: ${ANTLR_SampleGrammarLexer_CXX_OUTPUTS}")
message(STATUS "ANTLR4 Parser files: ${ANTLR_SampleGrammarParser_CXX_OUTPUTS}")
message(STATUS "ANTLR4 Visitor files: ${ANTLR_SampleGrammarVisitor_CXX_OUTPUTS}")

add_library(${PROJECT_NAME} STATIC OBJECT
    ${ANTLR_SampleGrammarLexer_CXX_OUTPUTS}
    ${ANTLR_SampleGrammarParser_CXX_OUTPUTS}
    ${ANTLR_SampleGrammarVisitor_CXX_OUTPUTS}
)

target_link_libraries(${PROJECT_NAME} PUBLIC antlr4_static)
target_include_directories(${PROJECT_NAME} PUBLIC "${ANTLR_SampleGrammarLexer_OUTPUT_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${ANTLR_SampleGrammarParser_OUTPUT_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${ANTLR_SampleGrammarVisitor_OUTPUT_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${antlr4-runtime_INCLUDE_DIRS}")
