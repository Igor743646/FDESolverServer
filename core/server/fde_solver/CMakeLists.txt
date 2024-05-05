cmake_minimum_required(VERSION 3.26 FATAL_ERROR)
project(FDESolverLib LANGUAGES CXX)

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
