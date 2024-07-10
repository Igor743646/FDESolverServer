cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)
project(LinalgLib LANGUAGES CXX)

add_library(${PROJECT_NAME} STATIC 
    "matrix.cpp"
)

target_link_libraries(${PROJECT_NAME} PUBLIC MathUtils StackTracer AsynchronicLogger proto_lib)
target_include_directories(${PROJECT_NAME} PUBLIC "../../utils/logger")
target_include_directories(${PROJECT_NAME} PUBLIC "../../utils/math")
target_include_directories(${PROJECT_NAME} PUBLIC "../../utils/stacktracer")
target_include_directories(${PROJECT_NAME} PUBLIC ${protobuf_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${gRPC_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${absl_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC "../../protos/cpp")

# Tests

find_package(Catch2 3 REQUIRED CONFIG)

add_executable(linalg_test
    "tests/linalg_test.cpp"
)

target_link_libraries(linalg_test PUBLIC LinalgLib MathUtils StackTracer AsynchronicLogger Catch2::Catch2WithMain proto_lib gRPC::grpc++)
target_include_directories(linalg_test PUBLIC "../../utils/math")
target_include_directories(linalg_test PUBLIC "../../utils/logger")
target_include_directories(${PROJECT_NAME} PUBLIC "../../utils/stacktracer")
target_include_directories(linalg_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(linalg_test PUBLIC ${protobuf_INCLUDE_DIRS})
target_include_directories(linalg_test PUBLIC ${gRPC_INCLUDE_DIRS})
target_include_directories(linalg_test PUBLIC ${absl_INCLUDE_DIRS})
target_include_directories(linalg_test PUBLIC "../../protos/cpp")
target_include_directories(linalg_test PUBLIC ${catch2_INCLUDE_DIRS_RELEASE})
