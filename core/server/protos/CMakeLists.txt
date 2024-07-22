cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)
project(FDESolverProtoFiles 
        LANGUAGES C CXX)

START_BUILD()

add_custom_target(PB_Files SOURCES "config.proto" "matrix.proto" "result.proto")
add_custom_target(GRPC_Files SOURCES "server.proto")

# CPP
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

# GO
protobuf_generate(
    TARGET PB_Files
    LANGUAGE go
    OUT_VAR GO_PROTO_SRCS_PB
    PLUGIN "protoc-gen-go=$ENV{GOPATH}/bin/protoc-gen-go.exe"
    PLUGIN_OPTIONS "paths=source_relative"
    PROTOC_OUT_DIR "${PROJECT_SOURCE_DIR}/go"
    GENERATE_EXTENSIONS .pb.go)

protobuf_generate(
    TARGET GRPC_Files
    LANGUAGE go
    OUT_VAR GO_PROTO_SRCS_GRPC
    PLUGIN "protoc-gen-go=$ENV{GOPATH}/bin/protoc-gen-go.exe"
    PLUGIN_OPTIONS "paths=source_relative"
    PROTOC_OUT_DIR "${PROJECT_SOURCE_DIR}/go"
    GENERATE_EXTENSIONS .pb.go)

# GRPC
protobuf_generate(
    TARGET GRPC_Files
    LANGUAGE grpc
    OUT_VAR PROTO_SRCS_GRPC_GRPC
    PLUGIN protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>
    PROTOC_OUT_DIR "${PROJECT_SOURCE_DIR}/cpp"
    GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc)

protobuf_generate(
    TARGET GRPC_Files
    LANGUAGE go-grpc
    OUT_VAR GO_PROTO_SRCS_GRPC_GRPC
    PLUGIN "protoc-gen-go-grpc=$ENV{GOPATH}/bin/protoc-gen-go-grpc.exe"
    PLUGIN_OPTIONS "paths=source_relative"
    PROTOC_OUT_DIR "${PROJECT_SOURCE_DIR}/go"
    GENERATE_EXTENSIONS _grpc.pb.go)

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
DEFINE_LIB_HEADERS(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/cpp")

add_custom_target(GO_${PROJECT_NAME} ALL SOURCES ${GO_PROTO_SRCS_PB} ${GO_PROTO_SRCS_GRPC} ${GO_PROTO_SRCS_GRPC_GRPC})

# Copy go files
add_custom_command(TARGET GO_${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/go/" "${PROJECT_SOURCE_DIR}/../../client/protos/"
    COMMENT "Copy go protos"
)
