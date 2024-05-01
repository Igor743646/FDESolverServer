cmake_minimum_required(VERSION 3.26 FATAL_ERROR)
project(Utils LANGUAGES CXX)

add_library(utils STATIC 
    utils.cpp
)

target_link_libraries(utils PUBLIC logger)
target_include_directories(utils PUBLIC "../logger")
