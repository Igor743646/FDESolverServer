cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)
project(StackTracer LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/generators/" "${CMAKE_PREFIX_PATH}")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)

add_library(${PROJECT_NAME} STATIC
    "stacktracer.cpp" "stacktracer.hpp"
)
target_link_libraries(${PROJECT_NAME} PUBLIC Boost::stacktrace Boost::exception)
target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
