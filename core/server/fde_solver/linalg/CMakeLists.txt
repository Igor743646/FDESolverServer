cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)
project(LinalgLib LANGUAGES CXX)

START_BUILD()

add_library(${PROJECT_NAME} STATIC 
    "matrix.cpp"
)

target_link_libraries(${PROJECT_NAME} PUBLIC 
    MathUtils 
    StackTracer 
    AsynchronicLogger 
    FDESolverProtoFiles
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

list(APPEND TESTS "matrix" "plu" "solve")

foreach(test_name IN LISTS TESTS)
    add_executable(${PROJECT_NAME}_${test_name}_test
        "tests/${test_name}_test.cpp"
    )

    target_link_libraries(${PROJECT_NAME}_${test_name}_test PUBLIC 
        LinalgLib
        Catch2::Catch2WithMain
    )

    target_include_directories(${PROJECT_NAME}_${test_name}_test PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR} 
        ${MathUtils_HEADERS_DIR}
        ${AsynchronicLogger_HEADERS_DIR}
        ${StackTracer_HEADERS_DIR}
        ${FDESolverProtoFiles_HEADERS_DIR}
        ${protobuf_INCLUDE_DIRS}
        ${gRPC_INCLUDE_DIRS}
        ${Catch2_INCLUDE_DIRS}
    )
endforeach()
