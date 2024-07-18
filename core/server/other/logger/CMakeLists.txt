cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)
project(AsynchronicLogger LANGUAGES CXX)

START_BUILD()

add_library(${PROJECT_NAME} STATIC
    logger.cpp
)

DEFINE_LIB_HEADERS(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}")
