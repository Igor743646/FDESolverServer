// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: server.proto

#include "server.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG

namespace _pb = ::PROTOBUF_NAMESPACE_ID;
namespace _pbi = _pb::internal;

namespace PFDESolverServer {
}  // namespace PFDESolverServer
static constexpr ::_pb::EnumDescriptor const** file_level_enum_descriptors_server_2eproto = nullptr;
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_server_2eproto = nullptr;
const uint32_t TableStruct_server_2eproto::offsets[1] = {};
static constexpr ::_pbi::MigrationSchema* schemas = nullptr;
static constexpr ::_pb::Message* const* file_default_instances = nullptr;

const char descriptor_table_protodef_server_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\014server.proto\022\020PFDESolverServer\032\014config"
  ".proto\032\014result.proto2O\n\020TFDESolverServer"
  "\022;\n\007RunTask\022\031.PFDESolver.TClientConfig\032\023"
  ".PFDESolver.TResult\"\000b\006proto3"
  ;
static const ::_pbi::DescriptorTable* const descriptor_table_server_2eproto_deps[2] = {
  &::descriptor_table_config_2eproto,
  &::descriptor_table_result_2eproto,
};
static ::_pbi::once_flag descriptor_table_server_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_server_2eproto = {
    false, false, 149, descriptor_table_protodef_server_2eproto,
    "server.proto",
    &descriptor_table_server_2eproto_once, descriptor_table_server_2eproto_deps, 2, 0,
    schemas, file_default_instances, TableStruct_server_2eproto::offsets,
    nullptr, file_level_enum_descriptors_server_2eproto,
    file_level_service_descriptors_server_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_server_2eproto_getter() {
  return &descriptor_table_server_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_server_2eproto(&descriptor_table_server_2eproto);
namespace PFDESolverServer {

// @@protoc_insertion_point(namespace_scope)
}  // namespace PFDESolverServer
PROTOBUF_NAMESPACE_OPEN
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
