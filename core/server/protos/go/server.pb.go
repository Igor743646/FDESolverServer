// Code generated by protoc-gen-go. DO NOT EDIT.
// versions:
// 	protoc-gen-go v1.34.2
// 	protoc        v3.21.12
// source: server.proto

package protos

import (
	protoreflect "google.golang.org/protobuf/reflect/protoreflect"
	protoimpl "google.golang.org/protobuf/runtime/protoimpl"
	reflect "reflect"
)

const (
	// Verify that this generated code is sufficiently up-to-date.
	_ = protoimpl.EnforceVersion(20 - protoimpl.MinVersion)
	// Verify that runtime/protoimpl is sufficiently up-to-date.
	_ = protoimpl.EnforceVersion(protoimpl.MaxVersion - 20)
)

var File_server_proto protoreflect.FileDescriptor

var file_server_proto_rawDesc = []byte{
	0x0a, 0x0c, 0x73, 0x65, 0x72, 0x76, 0x65, 0x72, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x12, 0x10,
	0x50, 0x46, 0x44, 0x45, 0x53, 0x6f, 0x6c, 0x76, 0x65, 0x72, 0x53, 0x65, 0x72, 0x76, 0x65, 0x72,
	0x1a, 0x0c, 0x63, 0x6f, 0x6e, 0x66, 0x69, 0x67, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x1a, 0x0c,
	0x72, 0x65, 0x73, 0x75, 0x6c, 0x74, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x32, 0x50, 0x0a, 0x10,
	0x54, 0x46, 0x44, 0x45, 0x53, 0x6f, 0x6c, 0x76, 0x65, 0x72, 0x53, 0x65, 0x72, 0x76, 0x65, 0x72,
	0x12, 0x3c, 0x0a, 0x07, 0x52, 0x75, 0x6e, 0x54, 0x61, 0x73, 0x6b, 0x12, 0x19, 0x2e, 0x50, 0x46,
	0x44, 0x45, 0x53, 0x6f, 0x6c, 0x76, 0x65, 0x72, 0x2e, 0x54, 0x43, 0x6c, 0x69, 0x65, 0x6e, 0x74,
	0x43, 0x6f, 0x6e, 0x66, 0x69, 0x67, 0x1a, 0x14, 0x2e, 0x50, 0x46, 0x44, 0x45, 0x53, 0x6f, 0x6c,
	0x76, 0x65, 0x72, 0x2e, 0x54, 0x52, 0x65, 0x73, 0x75, 0x6c, 0x74, 0x73, 0x22, 0x00, 0x42, 0x09,
	0x5a, 0x07, 0x2f, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x73, 0x62, 0x06, 0x70, 0x72, 0x6f, 0x74, 0x6f,
	0x33,
}

var file_server_proto_goTypes = []any{
	(*TClientConfig)(nil), // 0: PFDESolver.TClientConfig
	(*TResults)(nil),      // 1: PFDESolver.TResults
}
var file_server_proto_depIdxs = []int32{
	0, // 0: PFDESolverServer.TFDESolverServer.RunTask:input_type -> PFDESolver.TClientConfig
	1, // 1: PFDESolverServer.TFDESolverServer.RunTask:output_type -> PFDESolver.TResults
	1, // [1:2] is the sub-list for method output_type
	0, // [0:1] is the sub-list for method input_type
	0, // [0:0] is the sub-list for extension type_name
	0, // [0:0] is the sub-list for extension extendee
	0, // [0:0] is the sub-list for field type_name
}

func init() { file_server_proto_init() }
func file_server_proto_init() {
	if File_server_proto != nil {
		return
	}
	file_config_proto_init()
	file_result_proto_init()
	type x struct{}
	out := protoimpl.TypeBuilder{
		File: protoimpl.DescBuilder{
			GoPackagePath: reflect.TypeOf(x{}).PkgPath(),
			RawDescriptor: file_server_proto_rawDesc,
			NumEnums:      0,
			NumMessages:   0,
			NumExtensions: 0,
			NumServices:   1,
		},
		GoTypes:           file_server_proto_goTypes,
		DependencyIndexes: file_server_proto_depIdxs,
	}.Build()
	File_server_proto = out.File
	file_server_proto_rawDesc = nil
	file_server_proto_goTypes = nil
	file_server_proto_depIdxs = nil
}
