// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: server_common.proto

#include "server_common.pb.h"

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
PROTOBUF_CONSTEXPR Svr_Player_Info::Svr_Player_Info(
    ::_pbi::ConstantInitialized)
  : strplayername_(&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{})
  , strdevid_(&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{})
  , strchannel_(&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{})
  , strclientip_(&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{})
  , strcitycode_(&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{})
  , straccname_(&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{})
  , strappversion_(&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{})
  , llplayerid_(int64_t{0})
  , uilevel_(0u)
  , uigender_(0u)
  , lllogouttime_(int64_t{0})
  , eplayerstatus_(0)

  , uigameid_(0u)
  , lllogintime_(int64_t{0})
  , uigateid_(0u){}
struct Svr_Player_InfoDefaultTypeInternal {
  PROTOBUF_CONSTEXPR Svr_Player_InfoDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~Svr_Player_InfoDefaultTypeInternal() {}
  union {
    Svr_Player_Info _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 Svr_Player_InfoDefaultTypeInternal _Svr_Player_Info_default_instance_;
}  // namespace ProtoMsg
static ::_pb::Metadata file_level_metadata_server_5fcommon_2eproto[1];
static constexpr ::_pb::EnumDescriptor const** file_level_enum_descriptors_server_5fcommon_2eproto = nullptr;
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_server_5fcommon_2eproto = nullptr;

const uint32_t TableStruct_server_5fcommon_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::ProtoMsg::Svr_Player_Info, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::ProtoMsg::Svr_Player_Info, llplayerid_),
  PROTOBUF_FIELD_OFFSET(::ProtoMsg::Svr_Player_Info, strplayername_),
  PROTOBUF_FIELD_OFFSET(::ProtoMsg::Svr_Player_Info, uilevel_),
  PROTOBUF_FIELD_OFFSET(::ProtoMsg::Svr_Player_Info, uigender_),
  PROTOBUF_FIELD_OFFSET(::ProtoMsg::Svr_Player_Info, eplayerstatus_),
  PROTOBUF_FIELD_OFFSET(::ProtoMsg::Svr_Player_Info, lllogouttime_),
  PROTOBUF_FIELD_OFFSET(::ProtoMsg::Svr_Player_Info, uigameid_),
  PROTOBUF_FIELD_OFFSET(::ProtoMsg::Svr_Player_Info, uigateid_),
  PROTOBUF_FIELD_OFFSET(::ProtoMsg::Svr_Player_Info, strdevid_),
  PROTOBUF_FIELD_OFFSET(::ProtoMsg::Svr_Player_Info, strchannel_),
  PROTOBUF_FIELD_OFFSET(::ProtoMsg::Svr_Player_Info, strclientip_),
  PROTOBUF_FIELD_OFFSET(::ProtoMsg::Svr_Player_Info, strcitycode_),
  PROTOBUF_FIELD_OFFSET(::ProtoMsg::Svr_Player_Info, straccname_),
  PROTOBUF_FIELD_OFFSET(::ProtoMsg::Svr_Player_Info, strappversion_),
  PROTOBUF_FIELD_OFFSET(::ProtoMsg::Svr_Player_Info, lllogintime_),
};
static const ::_pbi::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, -1, sizeof(::ProtoMsg::Svr_Player_Info)},
};

static const ::_pb::Message* const file_default_instances[] = {
  &::ProtoMsg::_Svr_Player_Info_default_instance_._instance,
};

const char descriptor_table_protodef_server_5fcommon_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\023server_common.proto\022\010ProtoMsg\032\020msg_com"
  "mon.proto\"\331\002\n\017Svr_Player_Info\022\022\n\nllPlaye"
  "rId\030\001 \001(\003\022\025\n\rstrPlayerName\030\002 \001(\t\022\017\n\007uiLe"
  "vel\030\003 \001(\r\022\020\n\010uiGender\030\004 \001(\r\022.\n\rePlayerSt"
  "atus\030\005 \001(\0162\027.ProtoMsg.EPlayerStatus\022\024\n\014l"
  "lLogoutTime\030\006 \001(\003\022\020\n\010uiGameId\030\007 \001(\r\022\020\n\010u"
  "iGateId\030\010 \001(\r\022\020\n\010strDevId\030\t \001(\t\022\022\n\nstrCh"
  "annel\030\n \001(\t\022\023\n\013strClientIp\030\013 \001(\t\022\023\n\013strC"
  "ityCode\030\014 \001(\t\022\022\n\nstrAccName\030\r \001(\t\022\025\n\rstr"
  "AppVersion\030\016 \001(\t\022\023\n\013llLoginTime\030\017 \001(\003b\006p"
  "roto3"
  ;
static const ::_pbi::DescriptorTable* const descriptor_table_server_5fcommon_2eproto_deps[1] = {
  &::descriptor_table_msg_5fcommon_2eproto,
};
static ::_pbi::once_flag descriptor_table_server_5fcommon_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_server_5fcommon_2eproto = {
    false, false, 405, descriptor_table_protodef_server_5fcommon_2eproto,
    "server_common.proto",
    &descriptor_table_server_5fcommon_2eproto_once, descriptor_table_server_5fcommon_2eproto_deps, 1, 1,
    schemas, file_default_instances, TableStruct_server_5fcommon_2eproto::offsets,
    file_level_metadata_server_5fcommon_2eproto, file_level_enum_descriptors_server_5fcommon_2eproto,
    file_level_service_descriptors_server_5fcommon_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_server_5fcommon_2eproto_getter() {
  return &descriptor_table_server_5fcommon_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_server_5fcommon_2eproto(&descriptor_table_server_5fcommon_2eproto);
namespace ProtoMsg {

// ===================================================================

class Svr_Player_Info::_Internal {
 public:
};

Svr_Player_Info::Svr_Player_Info(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor();
  // @@protoc_insertion_point(arena_constructor:ProtoMsg.Svr_Player_Info)
}
Svr_Player_Info::Svr_Player_Info(const Svr_Player_Info& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  strplayername_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    strplayername_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_strplayername().empty()) {
    strplayername_.Set(from._internal_strplayername(), 
      GetArenaForAllocation());
  }
  strdevid_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    strdevid_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_strdevid().empty()) {
    strdevid_.Set(from._internal_strdevid(), 
      GetArenaForAllocation());
  }
  strchannel_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    strchannel_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_strchannel().empty()) {
    strchannel_.Set(from._internal_strchannel(), 
      GetArenaForAllocation());
  }
  strclientip_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    strclientip_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_strclientip().empty()) {
    strclientip_.Set(from._internal_strclientip(), 
      GetArenaForAllocation());
  }
  strcitycode_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    strcitycode_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_strcitycode().empty()) {
    strcitycode_.Set(from._internal_strcitycode(), 
      GetArenaForAllocation());
  }
  straccname_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    straccname_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_straccname().empty()) {
    straccname_.Set(from._internal_straccname(), 
      GetArenaForAllocation());
  }
  strappversion_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    strappversion_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_strappversion().empty()) {
    strappversion_.Set(from._internal_strappversion(), 
      GetArenaForAllocation());
  }
  ::memcpy(&llplayerid_, &from.llplayerid_,
    static_cast<size_t>(reinterpret_cast<char*>(&uigateid_) -
    reinterpret_cast<char*>(&llplayerid_)) + sizeof(uigateid_));
  // @@protoc_insertion_point(copy_constructor:ProtoMsg.Svr_Player_Info)
}

inline void Svr_Player_Info::SharedCtor() {
strplayername_.InitDefault();
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  strplayername_.Set("", GetArenaForAllocation());
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
strdevid_.InitDefault();
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  strdevid_.Set("", GetArenaForAllocation());
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
strchannel_.InitDefault();
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  strchannel_.Set("", GetArenaForAllocation());
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
strclientip_.InitDefault();
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  strclientip_.Set("", GetArenaForAllocation());
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
strcitycode_.InitDefault();
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  strcitycode_.Set("", GetArenaForAllocation());
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
straccname_.InitDefault();
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  straccname_.Set("", GetArenaForAllocation());
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
strappversion_.InitDefault();
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  strappversion_.Set("", GetArenaForAllocation());
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
::memset(reinterpret_cast<char*>(this) + static_cast<size_t>(
    reinterpret_cast<char*>(&llplayerid_) - reinterpret_cast<char*>(this)),
    0, static_cast<size_t>(reinterpret_cast<char*>(&uigateid_) -
    reinterpret_cast<char*>(&llplayerid_)) + sizeof(uigateid_));
}

Svr_Player_Info::~Svr_Player_Info() {
  // @@protoc_insertion_point(destructor:ProtoMsg.Svr_Player_Info)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void Svr_Player_Info::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  strplayername_.Destroy();
  strdevid_.Destroy();
  strchannel_.Destroy();
  strclientip_.Destroy();
  strcitycode_.Destroy();
  straccname_.Destroy();
  strappversion_.Destroy();
}

void Svr_Player_Info::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void Svr_Player_Info::Clear() {
// @@protoc_insertion_point(message_clear_start:ProtoMsg.Svr_Player_Info)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  strplayername_.ClearToEmpty();
  strdevid_.ClearToEmpty();
  strchannel_.ClearToEmpty();
  strclientip_.ClearToEmpty();
  strcitycode_.ClearToEmpty();
  straccname_.ClearToEmpty();
  strappversion_.ClearToEmpty();
  ::memset(&llplayerid_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&uigateid_) -
      reinterpret_cast<char*>(&llplayerid_)) + sizeof(uigateid_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* Svr_Player_Info::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // int64 llPlayerId = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 8)) {
          llplayerid_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // string strPlayerName = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 18)) {
          auto str = _internal_mutable_strplayername();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
          CHK_(::_pbi::VerifyUTF8(str, "ProtoMsg.Svr_Player_Info.strPlayerName"));
        } else
          goto handle_unusual;
        continue;
      // uint32 uiLevel = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 24)) {
          uilevel_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // uint32 uiGender = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 32)) {
          uigender_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // .ProtoMsg.EPlayerStatus ePlayerStatus = 5;
      case 5:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 40)) {
          uint64_t val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
          _internal_set_eplayerstatus(static_cast<::ProtoMsg::EPlayerStatus>(val));
        } else
          goto handle_unusual;
        continue;
      // int64 llLogoutTime = 6;
      case 6:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 48)) {
          lllogouttime_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // uint32 uiGameId = 7;
      case 7:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 56)) {
          uigameid_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // uint32 uiGateId = 8;
      case 8:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 64)) {
          uigateid_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // string strDevId = 9;
      case 9:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 74)) {
          auto str = _internal_mutable_strdevid();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
          CHK_(::_pbi::VerifyUTF8(str, "ProtoMsg.Svr_Player_Info.strDevId"));
        } else
          goto handle_unusual;
        continue;
      // string strChannel = 10;
      case 10:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 82)) {
          auto str = _internal_mutable_strchannel();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
          CHK_(::_pbi::VerifyUTF8(str, "ProtoMsg.Svr_Player_Info.strChannel"));
        } else
          goto handle_unusual;
        continue;
      // string strClientIp = 11;
      case 11:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 90)) {
          auto str = _internal_mutable_strclientip();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
          CHK_(::_pbi::VerifyUTF8(str, "ProtoMsg.Svr_Player_Info.strClientIp"));
        } else
          goto handle_unusual;
        continue;
      // string strCityCode = 12;
      case 12:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 98)) {
          auto str = _internal_mutable_strcitycode();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
          CHK_(::_pbi::VerifyUTF8(str, "ProtoMsg.Svr_Player_Info.strCityCode"));
        } else
          goto handle_unusual;
        continue;
      // string strAccName = 13;
      case 13:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 106)) {
          auto str = _internal_mutable_straccname();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
          CHK_(::_pbi::VerifyUTF8(str, "ProtoMsg.Svr_Player_Info.strAccName"));
        } else
          goto handle_unusual;
        continue;
      // string strAppVersion = 14;
      case 14:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 114)) {
          auto str = _internal_mutable_strappversion();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
          CHK_(::_pbi::VerifyUTF8(str, "ProtoMsg.Svr_Player_Info.strAppVersion"));
        } else
          goto handle_unusual;
        continue;
      // int64 llLoginTime = 15;
      case 15:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 120)) {
          lllogintime_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* Svr_Player_Info::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:ProtoMsg.Svr_Player_Info)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // int64 llPlayerId = 1;
  if (this->_internal_llplayerid() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt64ToArray(1, this->_internal_llplayerid(), target);
  }

  // string strPlayerName = 2;
  if (!this->_internal_strplayername().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_strplayername().data(), static_cast<int>(this->_internal_strplayername().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "ProtoMsg.Svr_Player_Info.strPlayerName");
    target = stream->WriteStringMaybeAliased(
        2, this->_internal_strplayername(), target);
  }

  // uint32 uiLevel = 3;
  if (this->_internal_uilevel() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteUInt32ToArray(3, this->_internal_uilevel(), target);
  }

  // uint32 uiGender = 4;
  if (this->_internal_uigender() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteUInt32ToArray(4, this->_internal_uigender(), target);
  }

  // .ProtoMsg.EPlayerStatus ePlayerStatus = 5;
  if (this->_internal_eplayerstatus() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteEnumToArray(
      5, this->_internal_eplayerstatus(), target);
  }

  // int64 llLogoutTime = 6;
  if (this->_internal_lllogouttime() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt64ToArray(6, this->_internal_lllogouttime(), target);
  }

  // uint32 uiGameId = 7;
  if (this->_internal_uigameid() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteUInt32ToArray(7, this->_internal_uigameid(), target);
  }

  // uint32 uiGateId = 8;
  if (this->_internal_uigateid() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteUInt32ToArray(8, this->_internal_uigateid(), target);
  }

  // string strDevId = 9;
  if (!this->_internal_strdevid().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_strdevid().data(), static_cast<int>(this->_internal_strdevid().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "ProtoMsg.Svr_Player_Info.strDevId");
    target = stream->WriteStringMaybeAliased(
        9, this->_internal_strdevid(), target);
  }

  // string strChannel = 10;
  if (!this->_internal_strchannel().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_strchannel().data(), static_cast<int>(this->_internal_strchannel().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "ProtoMsg.Svr_Player_Info.strChannel");
    target = stream->WriteStringMaybeAliased(
        10, this->_internal_strchannel(), target);
  }

  // string strClientIp = 11;
  if (!this->_internal_strclientip().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_strclientip().data(), static_cast<int>(this->_internal_strclientip().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "ProtoMsg.Svr_Player_Info.strClientIp");
    target = stream->WriteStringMaybeAliased(
        11, this->_internal_strclientip(), target);
  }

  // string strCityCode = 12;
  if (!this->_internal_strcitycode().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_strcitycode().data(), static_cast<int>(this->_internal_strcitycode().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "ProtoMsg.Svr_Player_Info.strCityCode");
    target = stream->WriteStringMaybeAliased(
        12, this->_internal_strcitycode(), target);
  }

  // string strAccName = 13;
  if (!this->_internal_straccname().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_straccname().data(), static_cast<int>(this->_internal_straccname().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "ProtoMsg.Svr_Player_Info.strAccName");
    target = stream->WriteStringMaybeAliased(
        13, this->_internal_straccname(), target);
  }

  // string strAppVersion = 14;
  if (!this->_internal_strappversion().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_strappversion().data(), static_cast<int>(this->_internal_strappversion().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "ProtoMsg.Svr_Player_Info.strAppVersion");
    target = stream->WriteStringMaybeAliased(
        14, this->_internal_strappversion(), target);
  }

  // int64 llLoginTime = 15;
  if (this->_internal_lllogintime() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt64ToArray(15, this->_internal_lllogintime(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:ProtoMsg.Svr_Player_Info)
  return target;
}

size_t Svr_Player_Info::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:ProtoMsg.Svr_Player_Info)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string strPlayerName = 2;
  if (!this->_internal_strplayername().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_strplayername());
  }

  // string strDevId = 9;
  if (!this->_internal_strdevid().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_strdevid());
  }

  // string strChannel = 10;
  if (!this->_internal_strchannel().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_strchannel());
  }

  // string strClientIp = 11;
  if (!this->_internal_strclientip().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_strclientip());
  }

  // string strCityCode = 12;
  if (!this->_internal_strcitycode().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_strcitycode());
  }

  // string strAccName = 13;
  if (!this->_internal_straccname().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_straccname());
  }

  // string strAppVersion = 14;
  if (!this->_internal_strappversion().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_strappversion());
  }

  // int64 llPlayerId = 1;
  if (this->_internal_llplayerid() != 0) {
    total_size += ::_pbi::WireFormatLite::Int64SizePlusOne(this->_internal_llplayerid());
  }

  // uint32 uiLevel = 3;
  if (this->_internal_uilevel() != 0) {
    total_size += ::_pbi::WireFormatLite::UInt32SizePlusOne(this->_internal_uilevel());
  }

  // uint32 uiGender = 4;
  if (this->_internal_uigender() != 0) {
    total_size += ::_pbi::WireFormatLite::UInt32SizePlusOne(this->_internal_uigender());
  }

  // int64 llLogoutTime = 6;
  if (this->_internal_lllogouttime() != 0) {
    total_size += ::_pbi::WireFormatLite::Int64SizePlusOne(this->_internal_lllogouttime());
  }

  // .ProtoMsg.EPlayerStatus ePlayerStatus = 5;
  if (this->_internal_eplayerstatus() != 0) {
    total_size += 1 +
      ::_pbi::WireFormatLite::EnumSize(this->_internal_eplayerstatus());
  }

  // uint32 uiGameId = 7;
  if (this->_internal_uigameid() != 0) {
    total_size += ::_pbi::WireFormatLite::UInt32SizePlusOne(this->_internal_uigameid());
  }

  // int64 llLoginTime = 15;
  if (this->_internal_lllogintime() != 0) {
    total_size += ::_pbi::WireFormatLite::Int64SizePlusOne(this->_internal_lllogintime());
  }

  // uint32 uiGateId = 8;
  if (this->_internal_uigateid() != 0) {
    total_size += ::_pbi::WireFormatLite::UInt32SizePlusOne(this->_internal_uigateid());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData Svr_Player_Info::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSizeCheck,
    Svr_Player_Info::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*Svr_Player_Info::GetClassData() const { return &_class_data_; }

void Svr_Player_Info::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to,
                      const ::PROTOBUF_NAMESPACE_ID::Message& from) {
  static_cast<Svr_Player_Info *>(to)->MergeFrom(
      static_cast<const Svr_Player_Info &>(from));
}


void Svr_Player_Info::MergeFrom(const Svr_Player_Info& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:ProtoMsg.Svr_Player_Info)
  GOOGLE_DCHECK_NE(&from, this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_strplayername().empty()) {
    _internal_set_strplayername(from._internal_strplayername());
  }
  if (!from._internal_strdevid().empty()) {
    _internal_set_strdevid(from._internal_strdevid());
  }
  if (!from._internal_strchannel().empty()) {
    _internal_set_strchannel(from._internal_strchannel());
  }
  if (!from._internal_strclientip().empty()) {
    _internal_set_strclientip(from._internal_strclientip());
  }
  if (!from._internal_strcitycode().empty()) {
    _internal_set_strcitycode(from._internal_strcitycode());
  }
  if (!from._internal_straccname().empty()) {
    _internal_set_straccname(from._internal_straccname());
  }
  if (!from._internal_strappversion().empty()) {
    _internal_set_strappversion(from._internal_strappversion());
  }
  if (from._internal_llplayerid() != 0) {
    _internal_set_llplayerid(from._internal_llplayerid());
  }
  if (from._internal_uilevel() != 0) {
    _internal_set_uilevel(from._internal_uilevel());
  }
  if (from._internal_uigender() != 0) {
    _internal_set_uigender(from._internal_uigender());
  }
  if (from._internal_lllogouttime() != 0) {
    _internal_set_lllogouttime(from._internal_lllogouttime());
  }
  if (from._internal_eplayerstatus() != 0) {
    _internal_set_eplayerstatus(from._internal_eplayerstatus());
  }
  if (from._internal_uigameid() != 0) {
    _internal_set_uigameid(from._internal_uigameid());
  }
  if (from._internal_lllogintime() != 0) {
    _internal_set_lllogintime(from._internal_lllogintime());
  }
  if (from._internal_uigateid() != 0) {
    _internal_set_uigateid(from._internal_uigateid());
  }
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void Svr_Player_Info::CopyFrom(const Svr_Player_Info& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:ProtoMsg.Svr_Player_Info)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Svr_Player_Info::IsInitialized() const {
  return true;
}

void Svr_Player_Info::InternalSwap(Svr_Player_Info* other) {
  using std::swap;
  auto* lhs_arena = GetArenaForAllocation();
  auto* rhs_arena = other->GetArenaForAllocation();
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &strplayername_, lhs_arena,
      &other->strplayername_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &strdevid_, lhs_arena,
      &other->strdevid_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &strchannel_, lhs_arena,
      &other->strchannel_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &strclientip_, lhs_arena,
      &other->strclientip_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &strcitycode_, lhs_arena,
      &other->strcitycode_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &straccname_, lhs_arena,
      &other->straccname_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &strappversion_, lhs_arena,
      &other->strappversion_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(Svr_Player_Info, uigateid_)
      + sizeof(Svr_Player_Info::uigateid_)
      - PROTOBUF_FIELD_OFFSET(Svr_Player_Info, llplayerid_)>(
          reinterpret_cast<char*>(&llplayerid_),
          reinterpret_cast<char*>(&other->llplayerid_));
}

::PROTOBUF_NAMESPACE_ID::Metadata Svr_Player_Info::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_server_5fcommon_2eproto_getter, &descriptor_table_server_5fcommon_2eproto_once,
      file_level_metadata_server_5fcommon_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace ProtoMsg
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::ProtoMsg::Svr_Player_Info*
Arena::CreateMaybeMessage< ::ProtoMsg::Svr_Player_Info >(Arena* arena) {
  return Arena::CreateMessageInternal< ::ProtoMsg::Svr_Player_Info >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
