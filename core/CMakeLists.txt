cmake_minimum_required(VERSION 3.26.4 FATAL_ERROR)
project(FDESolverWebServiceCore 
        LANGUAGES C CXX)

message(STATUS "[**] Start build ${PROJECT_NAME}")

# Copy python files
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/client/")
file(COPY "client/" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/client/)

add_subdirectory(protos)
add_subdirectory(server)
