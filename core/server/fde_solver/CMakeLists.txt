cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)
project(FDESolverLib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/Release/generators/" "${CMAKE_PREFIX_PATH}")
set(CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/generators/" "${CMAKE_PREFIX_PATH}")

find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC REQUIRED)

add_subdirectory(logger)
add_subdirectory(linalg)
add_subdirectory(utils)

add_library(solvers STATIC
    "src/equation_solver.cpp"
    "src/mfdes.cpp"
    "src/sfdes.cpp"
    "src/rules_impl.cpp"
)
target_link_libraries(solvers PUBLIC logger linalg proto_lib)
target_include_directories(solvers PUBLIC 
    "logger/"
    "linalg/"
    "utils/"
    ${Protobuf_INCLUDE_DIRS})
