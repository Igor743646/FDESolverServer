cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)
project(FDESolverLib LANGUAGES CXX)

add_subdirectory(linalg)

add_library(${PROJECT_NAME} STATIC
    "src/config.cpp"
    "src/equation_solver.cpp"
    "src/mfdes.cpp"
    "src/sfdes.cpp"
    "src/rules_impl.cpp"
)
target_link_libraries(${PROJECT_NAME} PUBLIC 
    MathUtils 
    Timer 
    AsynchronicLogger
    LinalgLib 
    FDESolverProtoFiles 
    gRPC::grpc 
    gRPC::grpc++ 
    protobuf::protobuf
)
target_include_directories(${PROJECT_NAME} PUBLIC 
    ${AsynchronicLogger_HEADERS_DIR}
    ${Timer_HEADERS_DIR}
    ${LinalgLib_HEADERS_DIR}
    ${MathUtils_HEADERS_DIR}
    ${Protobuf_INCLUDE_DIRS}
)

DEFINE_LIB_HEADERS(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Tests

list(APPEND TESTS "binsearch")

foreach(test_name IN LISTS TESTS)
    add_executable(${PROJECT_NAME}_${test_name}_test
        "tests/${test_name}_test.cpp"
    )

    target_link_libraries(${PROJECT_NAME}_${test_name}_test PUBLIC 
        MathUtils 
        Catch2::Catch2WithMain 
    )
    target_include_directories(${PROJECT_NAME}_${test_name}_test PUBLIC
        ${Catch2_INCLUDE_DIRS}
        ${MathUtils_HEADERS_DIR}
    )
endforeach()
