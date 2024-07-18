cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)
project(MathExpressionParserGrammar C CXX)

START_BUILD()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/" "${CMAKE_MODULE_PATH}")

include(def_antlr_target)

antlr_target(SampleGrammarLexer "TLexer.g4" LEXER
             PACKAGE ANTLRMathExpParser)

antlr_target(SampleGrammarParser "TParser.g4" PARSER
             PACKAGE ANTLRMathExpParser
             DEPENDS_ANTLR SampleGrammarLexer
             COMPILE_FLAGS -visitor -listener -atn -lib "${ANTLR_SampleGrammarLexer_OUTPUT_DIR}")

add_library(${PROJECT_NAME} STATIC
    ${ANTLR_SampleGrammarLexer_CXX_OUTPUTS}
    ${ANTLR_SampleGrammarParser_CXX_OUTPUTS}
    ${ANTLR_SampleGrammarVisitor_CXX_OUTPUTS}
)
target_link_libraries(${PROJECT_NAME} PUBLIC antlr4_static)
target_include_directories(${PROJECT_NAME} PUBLIC 
    ${ANTLR_SampleGrammarLexer_OUTPUT_DIR}
    ${ANTLR_SampleGrammarParser_OUTPUT_DIR}
    ${ANTLR_SampleGrammarVisitor_OUTPUT_DIR}
    ${antlr4-runtime_INCLUDE_DIRS}
)
