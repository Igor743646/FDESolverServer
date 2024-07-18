cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)
project(LinalgLib LANGUAGES CXX)

add_library(${PROJECT_NAME} STATIC 
    "matrix.cpp"
)

target_link_libraries(${PROJECT_NAME} PUBLIC 
    MathUtils 
    StackTracer 
    AsynchronicLogger 
    FDESolverProtoFiles 
    gRPC::grpc 
    gRPC::grpc++ 
    protobuf::protobuf
)
target_include_directories(${PROJECT_NAME} PUBLIC 
    ${MathUtils_HEADERS_DIR}
    ${AsynchronicLogger_HEADERS_DIR}
    ${StackTracer_HEADERS_DIR}
    ${FDESolverProtoFiles_HEADERS_DIR}
    ${protobuf_INCLUDE_DIRS}
    ${gRPC_INCLUDE_DIRS}
)

DEFINE_LIB_HEADERS(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}")

# Tests

add_executable(${PROJECT_NAME}_test
    "tests/linalg_test.cpp"
)

target_link_libraries(${PROJECT_NAME}_test PUBLIC 
    LinalgLib 
    MathUtils 
    StackTracer 
    AsynchronicLogger 
    Catch2::Catch2WithMain 
    FDESolverProtoFiles 
    gRPC::grpc gRPC::grpc++ 
    protobuf::protobuf
)
target_include_directories(${PROJECT_NAME}_test PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR} 
    ${MathUtils_HEADERS_DIR}
    ${AsynchronicLogger_HEADERS_DIR}
    ${StackTracer_HEADERS_DIR}
    ${FDESolverProtoFiles_HEADERS_DIR}
    ${protobuf_INCLUDE_DIRS}
    ${gRPC_INCLUDE_DIRS}
    ${Catch2_INCLUDE_DIRS}
)
