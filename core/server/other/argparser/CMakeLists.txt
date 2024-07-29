cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)
project(ArgumentParser LANGUAGES CXX)

START_BUILD()

add_library(${PROJECT_NAME} STATIC
    argument_parser.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC MathUtils)
target_include_directories(${PROJECT_NAME} PUBLIC ${MathUtils_HEADERS_DIR})

DEFINE_LIB_HEADERS(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}")
