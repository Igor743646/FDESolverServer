cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)
project(Utils LANGUAGES CXX)

START_BUILD()

add_library(${PROJECT_NAME} STATIC
    "utils.cpp" "utils.hpp"
)


target_link_libraries(${PROJECT_NAME} PUBLIC AsynchronicLogger Boost::stacktrace Boost::exception)
target_include_directories(${PROJECT_NAME} PUBLIC ${AsynchronicLogger_HEADERS_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})

DEFINE_LIB_HEADERS(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}")
