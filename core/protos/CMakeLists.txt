cmake_minimum_required(VERSION 3.26.4 FATAL_ERROR)
project(FDESolverWebServiceProtoFiles 
        LANGUAGES C CXX)

message(STATUS "[***] Start generate ${PROJECT_NAME}")

set(CMAKE_PREFIX_PATH "C:/vcpkg/installed/x64-windows" "${CMAKE_PREFIX_PATH}")

# Include for gRPC generating
include(cmake/common.cmake)

find_package(Protobuf REQUIRED CONFIG)
find_package(gRPC REQUIRED)

function(generate_protobuf)
    set(flags GRPC_MODE)
    set(args)
    set(listArgs SOURCES)

    cmake_parse_arguments(arg "${flags}" "${args}" "${listArgs}" ${ARGN})

    if (arg_GRPC_MODE)
        message(STATUS "[GRPC] proto generate files: ${arg_SOURCES}")
        # Generate protos
        foreach(proto_file ${arg_SOURCES})
            get_filename_component(proto_file_absolute_path "${proto_file}.proto" ABSOLUTE)
            get_filename_component(proto_file_path "${proto_file_absolute_path}" PATH)
            set(myserver_proto_src "${proto_file_path}/cpp/${proto_file}.pb.cc")
            set(myserver_proto_hdr "${proto_file_path}/cpp/${proto_file}.pb.h")
            set(myserver_proto_grpc_src "${proto_file_path}/cpp/${proto_file}.grpc.pb.cc")
            set(myserver_proto_grpc_hdr "${proto_file_path}/cpp/${proto_file}.grpc.pb.h")

            set(proto_sources "${myserver_proto_src}" "${proto_sources}" PARENT_SCOPE)
            set(proto_headeres "${myserver_proto_hdr}" "${proto_headeres}" PARENT_SCOPE)
            set(proto_sources "${myserver_proto_grpc_src}" "${proto_sources}" PARENT_SCOPE)
            set(proto_headeres "${myserver_proto_grpc_hdr}" "${proto_headeres}" PARENT_SCOPE)

            add_custom_command(
                OUTPUT "${myserver_proto_src}" "${myserver_proto_hdr}"
                COMMAND ${_PROTOBUF_PROTOC}
                ARGS --cpp_out "${PROJECT_SOURCE_DIR}/cpp"
                    --grpc_out "${PROJECT_SOURCE_DIR}/cpp"
                    --proto_path "${proto_file_path}"
                    --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
                    "${proto_file_absolute_path}"
                DEPENDS "${proto_file_absolute_path}"
                MAIN_DEPENDENCY "${proto_file_absolute_path}"
                BYPRODUCTS "${myserver_proto_src}" "${myserver_proto_hdr}"
                COMMENT "Generate headers and sources from proto: ${proto_file_absolute_path}")
        endforeach()
    else()
        message(STATUS "[PB] proto generate files: ${arg_SOURCES}")
        # Generate protos
        foreach(proto_file ${arg_SOURCES})
            message(STATUS "Generate from file: ${proto_file}.proto")
            get_filename_component(proto_file_absolute_path "${proto_file}.proto" ABSOLUTE)
            get_filename_component(proto_file_path "${proto_file_absolute_path}" PATH)
            set(myserver_proto_src "${proto_file_path}/cpp/${proto_file}.pb.cc")
            set(myserver_proto_hdr "${proto_file_path}/cpp/${proto_file}.pb.h")

            set(proto_sources "${myserver_proto_src}" "${proto_sources}" PARENT_SCOPE)
            set(proto_headeres "${myserver_proto_hdr}" "${proto_headeres}" PARENT_SCOPE)

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
    endif()

endfunction()

generate_protobuf(SOURCES "config")
generate_protobuf(SOURCES "matrix")
generate_protobuf(SOURCES "result")
generate_protobuf(SOURCES "server" GRPC_MODE)

# Create library
add_library(proto_lib STATIC OBJECT ${proto_sources} ${proto_headeres})
target_link_libraries(proto_lib
    absl::check
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})

target_include_directories(proto_lib PUBLIC "${PROJECT_SOURCE_DIR}/cpp")
target_include_directories(proto_lib PUBLIC "${Protobuf_INCLUDE_DIRS}")
