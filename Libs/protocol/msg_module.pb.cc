// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: msg_module.proto

#include "msg_module.pb.h"

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

namespace ProtoMsg {
}  // namespace ProtoMsg
static const ::_pb::EnumDescriptor* file_level_enum_descriptors_msg_5fmodule_2eproto[1];
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_msg_5fmodule_2eproto = nullptr;
const uint32_t TableStruct_msg_5fmodule_2eproto::offsets[1] = {};
static constexpr ::_pbi::MigrationSchema* schemas = nullptr;
static constexpr ::_pb::Message* const* file_default_instances = nullptr;

const char descriptor_table_protodef_msg_5fmodule_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\020msg_module.proto\022\010ProtoMsg*\307\001\n\tMsgModu"
  "le\022\t\n\005Begin\020\000\022\013\n\007Mapping\020\001\022\t\n\005Login\020\002\022\010\n"
  "\004Hero\020\003\022\n\n\006Friend\020\004\022\010\n\004Mall\020\005\022\010\n\004Mail\020\006\022"
  "\013\n\007Faction\020\007\022\n\n\006Active\020\010\022\n\n\006Ranker\020\t\022\n\n\006"
  "Battle\020P\022\020\n\014ServerCommon\020Z\022\017\n\013ServerInne"
  "r\020[\022\020\n\014ServerBattle\020\\\022\007\n\003End\020db\006proto3"
  ;
static ::_pbi::once_flag descriptor_table_msg_5fmodule_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_msg_5fmodule_2eproto = {
    false, false, 238, descriptor_table_protodef_msg_5fmodule_2eproto,
    "msg_module.proto",
    &descriptor_table_msg_5fmodule_2eproto_once, nullptr, 0, 0,
    schemas, file_default_instances, TableStruct_msg_5fmodule_2eproto::offsets,
    nullptr, file_level_enum_descriptors_msg_5fmodule_2eproto,
    file_level_service_descriptors_msg_5fmodule_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_msg_5fmodule_2eproto_getter() {
  return &descriptor_table_msg_5fmodule_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_msg_5fmodule_2eproto(&descriptor_table_msg_5fmodule_2eproto);
namespace ProtoMsg {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* MsgModule_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_msg_5fmodule_2eproto);
  return file_level_enum_descriptors_msg_5fmodule_2eproto[0];
}
bool MsgModule_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 80:
    case 90:
    case 91:
    case 92:
    case 100:
      return true;
    default:
      return false;
  }
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace ProtoMsg
PROTOBUF_NAMESPACE_OPEN
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
