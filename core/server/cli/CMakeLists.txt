cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)
project(FDESolverCLI
        LANGUAGES C CXX)

START_BUILD()

# cli
add_executable(${PROJECT_NAME}_${CMAKE_BUILD_TYPE})
target_sources(${PROJECT_NAME}_${CMAKE_BUILD_TYPE} PUBLIC "cli.cpp")
target_link_libraries(${PROJECT_NAME}_${CMAKE_BUILD_TYPE} PUBLIC
  MathExpressionParser
  MathExpressionParserGrammar
  FDESolverProtoFiles
  FDESolverLib
  FDESolverService
  ArgumentParser
  gRPC::grpc gRPC::grpc++ 
  protobuf::protobuf
  antlr4_static
)

target_include_directories(${PROJECT_NAME}_${CMAKE_BUILD_TYPE} PUBLIC ${FDESolverProtoFiles_HEADERS_DIR})
target_include_directories(${PROJECT_NAME}_${CMAKE_BUILD_TYPE} PUBLIC ${MathExpressionParser_HEADERS_DIR})
target_include_directories(${PROJECT_NAME}_${CMAKE_BUILD_TYPE} PUBLIC ${FDESolverLib_HEADERS_DIR})
target_include_directories(${PROJECT_NAME}_${CMAKE_BUILD_TYPE} PUBLIC ${ArgumentParser_HEADERS_DIR})
target_include_directories(${PROJECT_NAME}_${CMAKE_BUILD_TYPE} PUBLIC ${FDESolverService_HEADERS_DIR})
