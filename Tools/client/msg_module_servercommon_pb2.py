# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: msg_module_servercommon.proto
"""Generated protocol buffer code."""
from google.protobuf.internal import builder as _builder
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import symbol_database as _symbol_database
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\x1dmsg_module_servercommon.proto\x12\x08ProtoMsg\"\xe9\x01\n\x16MsgModule_ServerCommon\"\xce\x01\n\x06MsgCmd\x12\x19\n\x15Msg_ServerCommon_None\x10\x00\x12!\n\x1dMsg_ServerCommon_Register_Req\x10\x01\x12!\n\x1dMsg_ServerCommon_Register_Rsp\x10\x02\x12!\n\x1dMsg_ServerCommon_BeatHart_Req\x10\x03\x12!\n\x1dMsg_ServerCommon_BeatHart_Rsp\x10\x04\x12\x1d\n\x19Msg_ServerCommon_Transmit\x10\x05\"\x90\x01\n\x1dMsg_ServerCommon_Register_Req\x12\x14\n\x0cuiServerKind\x18\x01 \x01(\r\x12\x12\n\nuiServerId\x18\x02 \x01(\r\x12\x10\n\x08uiPlatId\x18\x03 \x01(\r\x12\x0f\n\x07uiIndex\x18\x04 \x01(\r\x12\x11\n\tuiGroupId\x18\x05 \x01(\r\x12\x0f\n\x07strExIp\x18\x06 \x01(\t\"p\n\x1dMsg_ServerCommon_Register_Rsp\x12\x14\n\x0cuiServerKind\x18\x01 \x01(\r\x12\x12\n\nuiServerId\x18\x02 \x01(\r\x12\x11\n\tbFirstCon\x18\x03 \x01(\x08\x12\x12\n\nstrComment\x18\x04 \x01(\t\"\x1f\n\x1dMsg_ServerCommon_BeatHart_Req\"\x1f\n\x1dMsg_ServerCommon_BeatHart_Rsp\"\x98\x01\n\x19Msg_ServerCommon_Transmit\x12\r\n\x05llUid\x18\x01 \x01(\x03\x12\x0f\n\x07vecUIds\x18\x02 \x03(\x03\x12\x12\n\nuiModuleId\x18\x03 \x01(\r\x12\r\n\x05uiCmd\x18\x04 \x01(\r\x12\x12\n\nuiSequence\x18\x05 \x01(\r\x12\x11\n\tuiRetCode\x18\x06 \x01(\r\x12\x11\n\tstrCmdMsg\x18\x07 \x01(\x0c\x62\x06proto3')

_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, globals())
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'msg_module_servercommon_pb2', globals())
if _descriptor._USE_C_DESCRIPTORS == False:

  DESCRIPTOR._options = None
  _MSGMODULE_SERVERCOMMON._serialized_start=44
  _MSGMODULE_SERVERCOMMON._serialized_end=277
  _MSGMODULE_SERVERCOMMON_MSGCMD._serialized_start=71
  _MSGMODULE_SERVERCOMMON_MSGCMD._serialized_end=277
  _MSG_SERVERCOMMON_REGISTER_REQ._serialized_start=280
  _MSG_SERVERCOMMON_REGISTER_REQ._serialized_end=424
  _MSG_SERVERCOMMON_REGISTER_RSP._serialized_start=426
  _MSG_SERVERCOMMON_REGISTER_RSP._serialized_end=538
  _MSG_SERVERCOMMON_BEATHART_REQ._serialized_start=178
  _MSG_SERVERCOMMON_BEATHART_REQ._serialized_end=209
  _MSG_SERVERCOMMON_BEATHART_RSP._serialized_start=213
  _MSG_SERVERCOMMON_BEATHART_RSP._serialized_end=244
  _MSG_SERVERCOMMON_TRANSMIT._serialized_start=607
  _MSG_SERVERCOMMON_TRANSMIT._serialized_end=759
# @@protoc_insertion_point(module_scope)
