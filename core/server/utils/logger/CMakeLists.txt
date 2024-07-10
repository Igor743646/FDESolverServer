cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)
project(AsynchronicLogger LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} STATIC
    logger.cpp
)
