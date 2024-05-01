cmake_minimum_required(VERSION 3.26 FATAL_ERROR)
project(FDESolverLib LANGUAGES CXX)

find_package(Protobuf CONFIG REQUIRED)

add_subdirectory(logger)
add_subdirectory(linalg)
add_subdirectory(utils)

add_library(solvers STATIC
    equation_solver.cpp
    mfdes.cpp
    sfdes.cpp
    rules_impl.cpp
)
target_link_libraries(solvers PUBLIC logger linalg proto_lib)
target_include_directories(solvers PUBLIC 
    "logger/"
    "linalg/"
    "utils/"
    ${Protobuf_INCLUDE_DIRS})
