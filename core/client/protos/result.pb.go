// Code generated by protoc-gen-go. DO NOT EDIT.
// versions:
// 	protoc-gen-go v1.35.1
// 	protoc        v3.21.12
// source: result.proto

package protos

import (
	protoreflect "google.golang.org/protobuf/reflect/protoreflect"
	protoimpl "google.golang.org/protobuf/runtime/protoimpl"
	reflect "reflect"
	sync "sync"
)

const (
	// Verify that this generated code is sufficiently up-to-date.
	_ = protoimpl.EnforceVersion(20 - protoimpl.MinVersion)
	// Verify that runtime/protoimpl is sufficiently up-to-date.
	_ = protoimpl.EnforceVersion(protoimpl.MaxVersion - 20)
)

type TResult struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	MethodName string   `protobuf:"bytes,1,opt,name=MethodName,proto3" json:"MethodName,omitempty"`
	Field      *TMatrix `protobuf:"bytes,2,opt,name=Field,proto3" json:"Field,omitempty"`
}

func (x *TResult) Reset() {
	*x = TResult{}
	mi := &file_result_proto_msgTypes[0]
	ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
	ms.StoreMessageInfo(mi)
}

func (x *TResult) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*TResult) ProtoMessage() {}

func (x *TResult) ProtoReflect() protoreflect.Message {
	mi := &file_result_proto_msgTypes[0]
	if x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use TResult.ProtoReflect.Descriptor instead.
func (*TResult) Descriptor() ([]byte, []int) {
	return file_result_proto_rawDescGZIP(), []int{0}
}

func (x *TResult) GetMethodName() string {
	if x != nil {
		return x.MethodName
	}
	return ""
}

func (x *TResult) GetField() *TMatrix {
	if x != nil {
		return x.Field
	}
	return nil
}

type TResults struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	Task             *TSolverConfig `protobuf:"bytes,1,opt,name=Task,proto3" json:"Task,omitempty"`
	Results          []*TResult     `protobuf:"bytes,2,rep,name=Results,proto3" json:"Results,omitempty"`
	RealSolution     *TMatrix       `protobuf:"bytes,3,opt,name=RealSolution,proto3,oneof" json:"RealSolution,omitempty"`
	RealSolutionName *string        `protobuf:"bytes,4,opt,name=RealSolutionName,proto3,oneof" json:"RealSolutionName,omitempty"`
}

func (x *TResults) Reset() {
	*x = TResults{}
	mi := &file_result_proto_msgTypes[1]
	ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
	ms.StoreMessageInfo(mi)
}

func (x *TResults) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*TResults) ProtoMessage() {}

func (x *TResults) ProtoReflect() protoreflect.Message {
	mi := &file_result_proto_msgTypes[1]
	if x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use TResults.ProtoReflect.Descriptor instead.
func (*TResults) Descriptor() ([]byte, []int) {
	return file_result_proto_rawDescGZIP(), []int{1}
}

func (x *TResults) GetTask() *TSolverConfig {
	if x != nil {
		return x.Task
	}
	return nil
}

func (x *TResults) GetResults() []*TResult {
	if x != nil {
		return x.Results
	}
	return nil
}

func (x *TResults) GetRealSolution() *TMatrix {
	if x != nil {
		return x.RealSolution
	}
	return nil
}

func (x *TResults) GetRealSolutionName() string {
	if x != nil && x.RealSolutionName != nil {
		return *x.RealSolutionName
	}
	return ""
}

var File_result_proto protoreflect.FileDescriptor

var file_result_proto_rawDesc = []byte{
	0x0a, 0x0c, 0x72, 0x65, 0x73, 0x75, 0x6c, 0x74, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x12, 0x0a,
	0x50, 0x46, 0x44, 0x45, 0x53, 0x6f, 0x6c, 0x76, 0x65, 0x72, 0x1a, 0x0c, 0x63, 0x6f, 0x6e, 0x66,
	0x69, 0x67, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x1a, 0x0c, 0x6d, 0x61, 0x74, 0x72, 0x69, 0x78,
	0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x22, 0x54, 0x0a, 0x07, 0x54, 0x52, 0x65, 0x73, 0x75, 0x6c,
	0x74, 0x12, 0x1e, 0x0a, 0x0a, 0x4d, 0x65, 0x74, 0x68, 0x6f, 0x64, 0x4e, 0x61, 0x6d, 0x65, 0x18,
	0x01, 0x20, 0x01, 0x28, 0x09, 0x52, 0x0a, 0x4d, 0x65, 0x74, 0x68, 0x6f, 0x64, 0x4e, 0x61, 0x6d,
	0x65, 0x12, 0x29, 0x0a, 0x05, 0x46, 0x69, 0x65, 0x6c, 0x64, 0x18, 0x02, 0x20, 0x01, 0x28, 0x0b,
	0x32, 0x13, 0x2e, 0x50, 0x46, 0x44, 0x45, 0x53, 0x6f, 0x6c, 0x76, 0x65, 0x72, 0x2e, 0x54, 0x4d,
	0x61, 0x74, 0x72, 0x69, 0x78, 0x52, 0x05, 0x46, 0x69, 0x65, 0x6c, 0x64, 0x22, 0xfd, 0x01, 0x0a,
	0x08, 0x54, 0x52, 0x65, 0x73, 0x75, 0x6c, 0x74, 0x73, 0x12, 0x2d, 0x0a, 0x04, 0x54, 0x61, 0x73,
	0x6b, 0x18, 0x01, 0x20, 0x01, 0x28, 0x0b, 0x32, 0x19, 0x2e, 0x50, 0x46, 0x44, 0x45, 0x53, 0x6f,
	0x6c, 0x76, 0x65, 0x72, 0x2e, 0x54, 0x53, 0x6f, 0x6c, 0x76, 0x65, 0x72, 0x43, 0x6f, 0x6e, 0x66,
	0x69, 0x67, 0x52, 0x04, 0x54, 0x61, 0x73, 0x6b, 0x12, 0x2d, 0x0a, 0x07, 0x52, 0x65, 0x73, 0x75,
	0x6c, 0x74, 0x73, 0x18, 0x02, 0x20, 0x03, 0x28, 0x0b, 0x32, 0x13, 0x2e, 0x50, 0x46, 0x44, 0x45,
	0x53, 0x6f, 0x6c, 0x76, 0x65, 0x72, 0x2e, 0x54, 0x52, 0x65, 0x73, 0x75, 0x6c, 0x74, 0x52, 0x07,
	0x52, 0x65, 0x73, 0x75, 0x6c, 0x74, 0x73, 0x12, 0x3c, 0x0a, 0x0c, 0x52, 0x65, 0x61, 0x6c, 0x53,
	0x6f, 0x6c, 0x75, 0x74, 0x69, 0x6f, 0x6e, 0x18, 0x03, 0x20, 0x01, 0x28, 0x0b, 0x32, 0x13, 0x2e,
	0x50, 0x46, 0x44, 0x45, 0x53, 0x6f, 0x6c, 0x76, 0x65, 0x72, 0x2e, 0x54, 0x4d, 0x61, 0x74, 0x72,
	0x69, 0x78, 0x48, 0x00, 0x52, 0x0c, 0x52, 0x65, 0x61, 0x6c, 0x53, 0x6f, 0x6c, 0x75, 0x74, 0x69,
	0x6f, 0x6e, 0x88, 0x01, 0x01, 0x12, 0x2f, 0x0a, 0x10, 0x52, 0x65, 0x61, 0x6c, 0x53, 0x6f, 0x6c,
	0x75, 0x74, 0x69, 0x6f, 0x6e, 0x4e, 0x61, 0x6d, 0x65, 0x18, 0x04, 0x20, 0x01, 0x28, 0x09, 0x48,
	0x01, 0x52, 0x10, 0x52, 0x65, 0x61, 0x6c, 0x53, 0x6f, 0x6c, 0x75, 0x74, 0x69, 0x6f, 0x6e, 0x4e,
	0x61, 0x6d, 0x65, 0x88, 0x01, 0x01, 0x42, 0x0f, 0x0a, 0x0d, 0x5f, 0x52, 0x65, 0x61, 0x6c, 0x53,
	0x6f, 0x6c, 0x75, 0x74, 0x69, 0x6f, 0x6e, 0x42, 0x13, 0x0a, 0x11, 0x5f, 0x52, 0x65, 0x61, 0x6c,
	0x53, 0x6f, 0x6c, 0x75, 0x74, 0x69, 0x6f, 0x6e, 0x4e, 0x61, 0x6d, 0x65, 0x42, 0x09, 0x5a, 0x07,
	0x2f, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x73, 0x62, 0x06, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x33,
}

var (
	file_result_proto_rawDescOnce sync.Once
	file_result_proto_rawDescData = file_result_proto_rawDesc
)

func file_result_proto_rawDescGZIP() []byte {
	file_result_proto_rawDescOnce.Do(func() {
		file_result_proto_rawDescData = protoimpl.X.CompressGZIP(file_result_proto_rawDescData)
	})
	return file_result_proto_rawDescData
}

var file_result_proto_msgTypes = make([]protoimpl.MessageInfo, 2)
var file_result_proto_goTypes = []any{
	(*TResult)(nil),       // 0: PFDESolver.TResult
	(*TResults)(nil),      // 1: PFDESolver.TResults
	(*TMatrix)(nil),       // 2: PFDESolver.TMatrix
	(*TSolverConfig)(nil), // 3: PFDESolver.TSolverConfig
}
var file_result_proto_depIdxs = []int32{
	2, // 0: PFDESolver.TResult.Field:type_name -> PFDESolver.TMatrix
	3, // 1: PFDESolver.TResults.Task:type_name -> PFDESolver.TSolverConfig
	0, // 2: PFDESolver.TResults.Results:type_name -> PFDESolver.TResult
	2, // 3: PFDESolver.TResults.RealSolution:type_name -> PFDESolver.TMatrix
	4, // [4:4] is the sub-list for method output_type
	4, // [4:4] is the sub-list for method input_type
	4, // [4:4] is the sub-list for extension type_name
	4, // [4:4] is the sub-list for extension extendee
	0, // [0:4] is the sub-list for field type_name
}

func init() { file_result_proto_init() }
func file_result_proto_init() {
	if File_result_proto != nil {
		return
	}
	file_config_proto_init()
	file_matrix_proto_init()
	file_result_proto_msgTypes[1].OneofWrappers = []any{}
	type x struct{}
	out := protoimpl.TypeBuilder{
		File: protoimpl.DescBuilder{
			GoPackagePath: reflect.TypeOf(x{}).PkgPath(),
			RawDescriptor: file_result_proto_rawDesc,
			NumEnums:      0,
			NumMessages:   2,
			NumExtensions: 0,
			NumServices:   0,
		},
		GoTypes:           file_result_proto_goTypes,
		DependencyIndexes: file_result_proto_depIdxs,
		MessageInfos:      file_result_proto_msgTypes,
	}.Build()
	File_result_proto = out.File
	file_result_proto_rawDesc = nil
	file_result_proto_goTypes = nil
	file_result_proto_depIdxs = nil
}
