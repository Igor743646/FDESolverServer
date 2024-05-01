cmake_minimum_required(VERSION 3.26.4 FATAL_ERROR)
project(ExpressionParser C CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# compiler must be 23
set(CMAKE_CXX_STANDARD 23)

# required if linking to static library
add_definitions(-DANTLR4CPP_EXPORTS)
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MD")
# using /MD flag for antlr4_runtime (for Visual C++ compilers only)
set(ANTLR4_WITH_STATIC_CRT OFF)
# add external build for antlrcpp
include(ExternalAntlr4Cpp)
include(FindANTLR)

# set variable pointing to the antlr tool that supports C++
# this is not required if the jar file can be found under PATH environment
set(ANTLR_EXECUTABLE ${CMAKE_CURRENT_SOURCE_DIR}/bin/antlr-4.13.1-complete.jar)

# add macros to generate ANTLR Cpp code from grammar
find_package(ANTLR REQUIRED)

# Call macro to add lexer and grammar to your build dependencies.
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

# include generated files in project environment
target_include_directories(math_expression_calculator PUBLIC "${ANTLR4_INCLUDE_DIRS}")
target_include_directories(math_expression_calculator PUBLIC "${ANTLR_SampleGrammarLexer_OUTPUT_DIR}")
target_include_directories(math_expression_calculator PUBLIC "${ANTLR_SampleGrammarParser_OUTPUT_DIR}")
target_include_directories(math_expression_calculator PUBLIC "${ANTLR_SampleGrammarVisitor_OUTPUT_DIR}")
