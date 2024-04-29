cmake_minimum_required(VERSION 3.26.4 FATAL_ERROR)
project(FDESolverWebServiceProtoFiles 
        LANGUAGES C CXX)

message(STATUS "[***] Start generate ${PROJECT_NAME}")

set(CMAKE_PREFIX_PATH "C:/vcpkg/installed/x64-windows" "${CMAKE_PREFIX_PATH}")

# Include for gRPC generating
include(cmake/common.cmake)

find_package(Protobuf REQUIRED CONFIG)
find_package(gRPC REQUIRED)

# Proto files
set(proto_messages_files "config" "matrix" "result")

# Generate protos
foreach(proto_file ${proto_messages_files})
    get_filename_component(proto_file_absolute_path "${proto_file}.proto" ABSOLUTE)
    get_filename_component(proto_file_path "${proto_file_absolute_path}" PATH)
    set(myserver_proto_src "${proto_file_path}/cpp/${proto_file}.pb.cc")
    set(myserver_proto_hdr "${proto_file_path}/cpp/${proto_file}.pb.h")

    set(proto_sources "${myserver_proto_src}" "${proto_sources}")
    set(proto_headeres "${myserver_proto_hdr}" "${proto_headeres}")

    add_custom_command(
        OUTPUT "${myserver_proto_src}" "${myserver_proto_hdr}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --cpp_out "${PROJECT_SOURCE_DIR}/cpp"
             --proto_path "${proto_file_path}"
             --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
             "${proto_file_absolute_path}"
        DEPENDS "${proto_file_absolute_path}"
        MAIN_DEPENDENCY "${proto_file_absolute_path}"
        BYPRODUCTS "${myserver_proto_src}" "${myserver_proto_hdr}"
        COMMENT "Generate headers and sources from proto: ${proto_file_absolute_path}")
endforeach()

# Create library
add_library(proto_lib STATIC OBJECT ${proto_sources} ${proto_headeres})
target_link_libraries(proto_lib
    absl::check
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})

target_include_directories(proto_lib PUBLIC "${PROJECT_SOURCE_DIR}/cpp")
target_include_directories(proto_lib PUBLIC "${Protobuf_INCLUDE_DIRS}")
