cmake_minimum_required(VERSION 3.26.4 FATAL_ERROR)
project(FDESolverWebServiceCore 
        LANGUAGES C CXX)

message(STATUS "[**] Start build ${PROJECT_NAME}")

add_subdirectory(protos)
add_subdirectory(server)
