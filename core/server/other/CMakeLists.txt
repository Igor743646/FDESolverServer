cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)
project(Other LANGUAGES C CXX)

START_BUILD()

add_subdirectory(logger)
add_subdirectory(stacktracer)
add_subdirectory(timer)
add_subdirectory(utils)
add_subdirectory(math)
