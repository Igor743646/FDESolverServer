cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)
project(Timer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} STATIC
    timer.cpp timer.hpp
)
