cmake_minimum_required(VERSION 3.26 FATAL_ERROR)
project(LinalgLib LANGUAGES CXX)

add_library(linalg STATIC 
    "matrix.cpp"
)

target_link_libraries(linalg PUBLIC utils proto_lib)
target_include_directories(linalg PUBLIC "../utils/")
target_include_directories(linalg PUBLIC ${protobuf_INCLUDE_DIRS})
target_include_directories(linalg PUBLIC ${gRPC_INCLUDE_DIRS})
target_include_directories(linalg PUBLIC ${absl_INCLUDE_DIRS})
target_include_directories(linalg PUBLIC "${CMAKE_SOURCE_DIR}/core/protos/cpp")

# Tests

find_package(Catch2 REQUIRED CONFIG)

add_executable(linalg_test
    "tests/linalg_test.cpp"
)

target_link_libraries(linalg_test PUBLIC linalg utils Catch2::Catch2WithMain proto_lib gRPC::grpc++)
target_include_directories(linalg_test PUBLIC "../utils/")
target_include_directories(linalg_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(linalg_test PUBLIC ${protobuf_INCLUDE_DIRS})
target_include_directories(linalg_test PUBLIC ${gRPC_INCLUDE_DIRS})
target_include_directories(linalg_test PUBLIC ${absl_INCLUDE_DIRS})
target_include_directories(linalg_test PUBLIC "${CMAKE_SOURCE_DIR}/core/protos/cpp")
target_include_directories(linalg_test PUBLIC ${catch2_INCLUDE_DIRS_RELEASE})
