cmake_minimum_required(VERSION 3.26.4 FATAL_ERROR)
project(FDESolverWebServiceServer
        LANGUAGES C CXX)

message(STATUS "[***] Start build ${PROJECT_NAME}")

set(CMAKE_PREFIX_PATH "C:/vcpkg/installed/x64-windows" "${CMAKE_PREFIX_PATH}")
find_package(Protobuf REQUIRED CONFIG)
find_package(absl REQUIRED)

# server
add_executable(main "main.cpp")
target_link_libraries(main proto_lib)
target_include_directories(main PUBLIC "${PROJECT_SOURCE_DIR}/protos/cpp/")

# Copy dll's for protobuf
add_custom_command(TARGET main POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:main> $<TARGET_RUNTIME_DLLS:main>
  COMMAND_EXPAND_LISTS
  COMMENT "Copy dll's to binary ($<TARGET_FILE_DIR:main>/$<TARGET_FILE_NAME:main>) list: ($<TARGET_RUNTIME_DLLS:main>)"
)
