cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)
project(FDESolverProtoFiles 
        LANGUAGES C CXX)

START_BUILD()

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
add_library(${PROJECT_NAME} STATIC ${PROTO_SRCS_PB} ${PROTO_SRCS_GRPC} ${PROTO_SRCS_GRPC_GRPC})
target_link_libraries(${PROJECT_NAME} PUBLIC 
    gRPC::grpc 
    gRPC::grpc++ 
    protobuf::protobuf
)

target_include_directories(${PROJECT_NAME} PUBLIC ${protobuf_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${gRPC_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${absl_INCLUDE_DIRS})

add_custom_target(PY_${PROJECT_NAME} ALL SOURCES ${PY_PROTO_SRCS_PB} ${PY_PROTO_SRCS_GRPC} ${PY_PROTO_SRCS_GRPC_GRPC})

# Copy python files
add_custom_command(TARGET PY_${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/py/" "${PROJECT_SOURCE_DIR}/../../client/libs/generated/"
    COMMENT "Copy python protos"
)

DEFINE_LIB_HEADERS(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/cpp")
