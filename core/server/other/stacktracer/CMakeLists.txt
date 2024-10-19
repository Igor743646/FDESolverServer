cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)
project(StackTracer LANGUAGES CXX)

START_BUILD()

add_library(${PROJECT_NAME} STATIC
    "stacktracer.cpp" "stacktracer.hpp"
)
target_link_libraries(${PROJECT_NAME} PUBLIC Utils Boost::thread)
target_include_directories(${PROJECT_NAME} PUBLIC ${Utils_HEADERS_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})

DEFINE_LIB_HEADERS(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}")
