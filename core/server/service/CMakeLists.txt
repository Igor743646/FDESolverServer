cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)
project(FDESolverService LANGUAGES CXX)

START_BUILD()

add_library(${PROJECT_NAME} STATIC
    "service_impl.cpp"
)
target_link_libraries(${PROJECT_NAME} PUBLIC 
    MathExpressionParser
    FDESolverLib
)

target_include_directories(${PROJECT_NAME} PUBLIC ${FDESolverProtoFiles_HEADERS_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${MathExpressionParser_HEADERS_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${FDESolverLib_HEADERS_DIR})

DEFINE_LIB_HEADERS(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}")
