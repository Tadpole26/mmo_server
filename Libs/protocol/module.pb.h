// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: module.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_module_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_module_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3019000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3019004 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_module_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_module_2eproto {
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_module_2eproto;
PROTOBUF_NAMESPACE_OPEN
PROTOBUF_NAMESPACE_CLOSE
namespace client {

enum enModule : int {
  enModule_Begin = 0,
  enModule_Mapping = 1,
  enModule_Login = 2,
  enModule_Hero = 3,
  enModule_Friend = 4,
  enModule_Mall = 5,
  enModule_Mail = 6,
  enModule_Mail_Faction = 7,
  enModule_Active = 8,
  enModule_Ranker = 9,
  enModule_Battle = 80,
  enModule_ServerCommon = 90,
  enModule_ServerInner = 91,
  enModule_ServerBattle = 92,
  enModule_End = 100,
  enModule_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  enModule_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool enModule_IsValid(int value);
constexpr enModule enModule_MIN = enModule_Begin;
constexpr enModule enModule_MAX = enModule_End;
constexpr int enModule_ARRAYSIZE = enModule_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* enModule_descriptor();
template<typename T>
inline const std::string& enModule_Name(T enum_t_value) {
  static_assert(::std::is_same<T, enModule>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function enModule_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    enModule_descriptor(), enum_t_value);
}
inline bool enModule_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, enModule* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<enModule>(
    enModule_descriptor(), name, value);
}
// ===================================================================


// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace client

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::client::enModule> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::client::enModule>() {
  return ::client::enModule_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_module_2eproto
