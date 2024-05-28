cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)
project(Utils LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(utils STATIC
    utils.cpp utils.hpp
)

target_link_libraries(utils PUBLIC logger)
target_include_directories(utils PUBLIC "../logger")
