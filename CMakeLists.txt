cmake_minimum_required(VERSION 3.26.4 FATAL_ERROR)
project(FDESolverWebService 
        VERSION 0.1
        DESCRIPTION "Client-Server FDE solver"
        LANGUAGES C CXX)

message(STATUS "[*] Start build ${PROJECT_NAME}")

add_subdirectory(core)
