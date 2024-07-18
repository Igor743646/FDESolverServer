cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)
project(Utils LANGUAGES CXX)

START_BUILD()

add_library(${PROJECT_NAME} STATIC
    "utils.cpp" "utils.hpp"
)

DEFINE_LIB_HEADERS(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}")
