cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)
project(MathUtils LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} STATIC
    math_utils.cpp math_utils.hpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC AsynchronicLogger)
target_include_directories(${PROJECT_NAME} PUBLIC "../logger")
