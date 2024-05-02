cmake_minimum_required(VERSION 3.26 FATAL_ERROR)
project(LinalgLib LANGUAGES CXX)

add_library(linalg STATIC OBJECT 
    matrix.cpp
)

target_link_libraries(linalg PUBLIC utils proto_lib)
target_include_directories(linalg PUBLIC "../utils/")
target_include_directories(linalg PUBLIC ${protobuf_INCLUDE_DIRS})
target_include_directories(linalg PUBLIC ${gRPC_INCLUDE_DIRS})
target_include_directories(linalg PUBLIC ${absl_INCLUDE_DIRS})
target_include_directories(linalg PUBLIC "${CMAKE_SOURCE_DIR}/core/protos/cpp")
