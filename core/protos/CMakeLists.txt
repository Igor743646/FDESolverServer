cmake_minimum_required(VERSION 3.26.4 FATAL_ERROR)
project(FDESolverWebServiceProtoFiles 
        LANGUAGES C CXX)

message(STATUS "[***] Start generate ${PROJECT_NAME}")

set(CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/generators/" "${CMAKE_PREFIX_PATH}")

find_package(Protobuf REQUIRED CONFIG)
find_package(gRPC REQUIRED)

add_custom_target(PB_Files SOURCES "config.proto" "matrix.proto" "result.proto")
add_custom_target(GRPC_Files SOURCES "server.proto")

protobuf_generate(
    TARGET PB_Files
    LANGUAGE cpp
    OUT_VAR PROTO_SRCS_PB
    PROTOC_OUT_DIR "${PROJECT_SOURCE_DIR}/cpp")

protobuf_generate(
    TARGET GRPC_Files
    LANGUAGE cpp
    OUT_VAR PROTO_SRCS_GRPC
    PROTOC_OUT_DIR "${PROJECT_SOURCE_DIR}/cpp")

protobuf_generate(
    TARGET PB_Files
    LANGUAGE python
    PROTOC_OUT_DIR "${PROJECT_SOURCE_DIR}/py")

protobuf_generate(
    TARGET GRPC_Files
    LANGUAGE python
    PROTOC_OUT_DIR "${PROJECT_SOURCE_DIR}/py")

protobuf_generate(
    TARGET GRPC_Files
    LANGUAGE grpc
    OUT_VAR PROTO_SRCS_GRPC_GRPC
    # DEPENDENCIES PB_Files
    PLUGIN protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>
    PROTOC_OUT_DIR "${PROJECT_SOURCE_DIR}/cpp"
    GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc)

protobuf_generate(
    TARGET GRPC_Files
    LANGUAGE grpc
    PLUGIN protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_python_plugin>
    PROTOC_OUT_DIR "${PROJECT_SOURCE_DIR}/py"
    GENERATE_EXTENSIONS _pb2_grpc.py)

# Create library
add_library(proto_lib STATIC OBJECT ${PROTO_SRCS_PB} ${PROTO_SRCS_GRPC} ${PROTO_SRCS_GRPC_GRPC})
target_link_libraries(proto_lib)

target_include_directories(proto_lib PUBLIC ${protobuf_INCLUDE_DIRS})
target_include_directories(proto_lib PUBLIC ${gRPC_INCLUDE_DIRS})
target_include_directories(proto_lib PUBLIC ${absl_INCLUDE_DIRS})
