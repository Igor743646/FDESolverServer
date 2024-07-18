cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)
project(MathUtils LANGUAGES CXX)

START_BUILD()

add_library(${PROJECT_NAME} STATIC
    math_utils.cpp math_utils.hpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC AsynchronicLogger)
target_include_directories(${PROJECT_NAME} PUBLIC ${AsynchronicLogger_HEADERS_DIR})

DEFINE_LIB_HEADERS(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}")
