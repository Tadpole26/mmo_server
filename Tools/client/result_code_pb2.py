# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: result_code.proto
"""Generated protocol buffer code."""
from google.protobuf.internal import builder as _builder
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import symbol_database as _symbol_database
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\x11result_code.proto\x12\x08ProtoMsg*\xb3\x03\n\nResultCode\x12\x17\n\x13\x43ode_Common_Success\x10\x00\x12\x17\n\x13\x43ode_Common_Failure\x10\x01\x12!\n\x1c\x43ode_Login_RoleNameCollision\x10\xe9\x07\x12\x16\n\x11\x43ode_Login_NoRole\x10\xea\x07\x12\x1a\n\x15\x43ode_Login_RoleInGame\x10\xeb\x07\x12\x1a\n\x15\x43ode_Login_CheckOften\x10\xec\x07\x12\x18\n\x13\x43ode_Login_NoInGame\x10\xed\x07\x12\x1b\n\x16\x43ode_Login_NameTooLong\x10\xee\x07\x12\x1c\n\x17\x43ode_Login_NameNotRight\x10\xef\x07\x12\x1d\n\x18\x43ode_Login_DeviceNotSame\x10\xf0\x07\x12\x1e\n\x19\x43ode_Login_ReloginTimeout\x10\xf1\x07\x12\x17\n\x12\x43ode_Login_Failure\x10\xf2\x07\x12\x1b\n\x16\x43ode_Match_RoomRelease\x10\xd1\x0f\x12\x1c\n\x17\x43ode_Match_NotEnterRoom\x10\xd2\x0f\x12\x18\n\x13\x43ode_Match_FullRoom\x10\xd3\x0f\x62\x06proto3')

_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, globals())
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'result_code_pb2', globals())
if _descriptor._USE_C_DESCRIPTORS == False:

  DESCRIPTOR._options = None
  _RESULTCODE._serialized_start=32
  _RESULTCODE._serialized_end=467
# @@protoc_insertion_point(module_scope)
