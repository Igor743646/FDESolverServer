cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)
project(FDESolverServer
        LANGUAGES C CXX)

include("${CMAKE_SOURCE_DIR}/cmake/check_os.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/output.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/library_vars.cmake")

START_BUILD()

add_subdirectory(protos)
add_subdirectory(other)
add_subdirectory(expression_calculator)
add_subdirectory(fde_solver)

# server
add_executable(${PROJECT_NAME}_${CMAKE_BUILD_TYPE})
target_sources(${PROJECT_NAME}_${CMAKE_BUILD_TYPE} PUBLIC "src/server.cpp" "src/server_impl.cpp" "src/server_impl.hpp" "src/service_impl.cpp" "src/service_impl.hpp")
target_link_libraries(${PROJECT_NAME}_${CMAKE_BUILD_TYPE} PUBLIC
  MathExpressionParser
  MathExpressionParserGrammar
  FDESolverProtoFiles
  FDESolverLib
  gRPC::grpc gRPC::grpc++ 
  protobuf::protobuf
  antlr4_static
)

target_include_directories(${PROJECT_NAME}_${CMAKE_BUILD_TYPE} PUBLIC ${FDESolverProtoFiles_HEADERS_DIR})
target_include_directories(${PROJECT_NAME}_${CMAKE_BUILD_TYPE} PUBLIC ${MathExpressionParser_HEADERS_DIR})
target_include_directories(${PROJECT_NAME}_${CMAKE_BUILD_TYPE} PUBLIC ${FDESolverLib_HEADERS_DIR})
