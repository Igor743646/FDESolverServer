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
add_subdirectory(service)
add_subdirectory(cli)

# server
add_executable(${PROJECT_NAME}_${CMAKE_BUILD_TYPE})
target_sources(${PROJECT_NAME}_${CMAKE_BUILD_TYPE} PUBLIC "src/server.cpp" "src/server_impl.cpp")
target_link_libraries(${PROJECT_NAME}_${CMAKE_BUILD_TYPE} PUBLIC
  FDESolverService
)

target_include_directories(${PROJECT_NAME}_${CMAKE_BUILD_TYPE} PUBLIC ${FDESolverProtoFiles_HEADERS_DIR})
target_include_directories(${PROJECT_NAME}_${CMAKE_BUILD_TYPE} PUBLIC ${MathExpressionParser_HEADERS_DIR})
target_include_directories(${PROJECT_NAME}_${CMAKE_BUILD_TYPE} PUBLIC ${FDESolverLib_HEADERS_DIR})
target_include_directories(${PROJECT_NAME}_${CMAKE_BUILD_TYPE} PUBLIC ${FDESolverService_HEADERS_DIR})
