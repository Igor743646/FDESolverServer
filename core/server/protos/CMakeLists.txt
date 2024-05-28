cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)
project(FDESolverWebServiceProtoFiles 
        LANGUAGES C CXX)

message(STATUS "[****] Start generate ${PROJECT_NAME}")

set(CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/generators/" "${CMAKE_PREFIX_PATH}")
set(CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/Release/generators/" "${CMAKE_PREFIX_PATH}")

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
    OUT_VAR PY_PROTO_SRCS_PB
    PROTOC_OUT_DIR "${PROJECT_SOURCE_DIR}/py")

protobuf_generate(
    TARGET GRPC_Files
    LANGUAGE python
    OUT_VAR PY_PROTO_SRCS_GRPC
    PROTOC_OUT_DIR "${PROJECT_SOURCE_DIR}/py")

protobuf_generate(
    TARGET GRPC_Files
    LANGUAGE grpc
    OUT_VAR PROTO_SRCS_GRPC_GRPC
    PLUGIN protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>
    PROTOC_OUT_DIR "${PROJECT_SOURCE_DIR}/cpp"
    GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc)

protobuf_generate(
    TARGET GRPC_Files
    LANGUAGE grpc
    OUT_VAR PY_PROTO_SRCS_GRPC_GRPC
    PLUGIN protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_python_plugin>
    PROTOC_OUT_DIR "${PROJECT_SOURCE_DIR}/py"
    GENERATE_EXTENSIONS _pb2_grpc.py)

# Create library
add_library(proto_lib STATIC OBJECT ${PROTO_SRCS_PB} ${PROTO_SRCS_GRPC} ${PROTO_SRCS_GRPC_GRPC})
target_link_libraries(proto_lib)
target_include_directories(proto_lib PUBLIC ${protobuf_INCLUDE_DIRS})
target_include_directories(proto_lib PUBLIC ${gRPC_INCLUDE_DIRS})
target_include_directories(proto_lib PUBLIC ${absl_INCLUDE_DIRS})

add_custom_target(py_proto_lib ALL SOURCES ${PY_PROTO_SRCS_PB} ${PY_PROTO_SRCS_GRPC} ${PY_PROTO_SRCS_GRPC_GRPC})

# Copy python files
add_custom_command(TARGET py_proto_lib POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/py/" "${PROJECT_SOURCE_DIR}/../../client/libs/generated/"
    COMMENT "Copy python protos"
)
