cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)
project(FDESolverServer
        LANGUAGES C CXX)

message(STATUS "[***] Start build ${PROJECT_NAME}")

add_subdirectory(protos)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall")

add_subdirectory(expression_calculator)
add_subdirectory(fde_solver)

set(CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/generators/" "${CMAKE_PREFIX_PATH}")
find_package(Protobuf REQUIRED CONFIG)
find_package(gRPC REQUIRED)
find_package(absl REQUIRED)

# server
add_executable(server "src/server.cpp")
target_link_libraries(server PUBLIC
  math_expression_calculator
  math_expression_grammar
  solvers
  gRPC::grpc++
  proto_lib 
)
target_include_directories(server PUBLIC "protos/cpp/")
target_include_directories(server PUBLIC "expression_calculator/src/")
target_include_directories(server PUBLIC "fde_solver/src/")
target_include_directories(server PUBLIC "fde_solver/utils/src/")

# # Copy dll's for protobuf
# add_custom_command(TARGET server POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:server> $<TARGET_RUNTIME_DLLS:server>
#   COMMAND_EXPAND_LISTS
#   COMMENT "Copy dll's to binary ($<TARGET_FILE_DIR:server>/$<TARGET_FILE_NAME:server>) list: ($<TARGET_RUNTIME_DLLS:server>)"
# )
