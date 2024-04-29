cmake_minimum_required(VERSION 3.26.4 FATAL_ERROR)
project(FDESolverWebServiceServer
        LANGUAGES C CXX)

message(STATUS "[***] Start build ${PROJECT_NAME}")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall")
set(CMAKE_BUILD_TYPE "Release")

add_subdirectory(expression_calculator)

include(../protos/cmake/common.cmake)
set(CMAKE_PREFIX_PATH "C:/vcpkg/installed/x64-windows" "${CMAKE_PREFIX_PATH}")
find_package(Protobuf REQUIRED CONFIG)
find_package(gRPC REQUIRED)
find_package(absl REQUIRED)

# server
add_executable(main "main.cpp")
target_link_libraries(main 
  math_expression_calculator 
  absl::check
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
  proto_lib 
)
target_include_directories(main PUBLIC "${PROJECT_SOURCE_DIR}/protos/cpp/")
target_include_directories(main PUBLIC "${PROJECT_SOURCE_DIR}/server/expression_calculator/")

# Copy dll's for protobuf
add_custom_command(TARGET main POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:main> $<TARGET_RUNTIME_DLLS:main>
  COMMAND_EXPAND_LISTS
  COMMENT "Copy dll's to binary ($<TARGET_FILE_DIR:main>/$<TARGET_FILE_NAME:main>) list: ($<TARGET_RUNTIME_DLLS:main>)"
)

file(COPY "C:/vcpkg/installed/x64-windows/bin/zlib1.dll" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Debug/")
file(COPY "C:/vcpkg/installed/x64-windows/bin/libssl-3-x64.dll" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Debug/")
file(COPY "C:/vcpkg/installed/x64-windows/bin/libcrypto-3-x64.dll" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Debug/")
