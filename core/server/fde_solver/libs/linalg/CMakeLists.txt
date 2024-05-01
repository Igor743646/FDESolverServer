cmake_minimum_required(VERSION 3.26 FATAL_ERROR)
project(LinalgLib LANGUAGES CXX)

add_library(linalg STATIC OBJECT 
    matrix.cpp
)

target_link_libraries(linalg PUBLIC utils proto_lib)
target_include_directories(linalg PUBLIC "../utils/")
