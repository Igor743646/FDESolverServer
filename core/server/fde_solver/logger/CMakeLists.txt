cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)
project(AsynchronicLogger LANGUAGES CXX)

add_library(logger STATIC
    logger.cpp
)
