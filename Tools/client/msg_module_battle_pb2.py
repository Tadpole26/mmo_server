# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: msg_module_battle.proto
"""Generated protocol buffer code."""
from google.protobuf.internal import builder as _builder
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import symbol_database as _symbol_database
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()


import msg_common_pb2 as msg__common__pb2
import result_code_pb2 as result__code__pb2


DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\x17msg_module_battle.proto\x12\x08ProtoMsg\x1a\x10msg_common.proto\x1a\x11result_code.proto\"\xe9\x06\n\x10MsgModule_Battle\"\xd4\x06\n\x06MsgCmd\x12\x13\n\x0fMsg_Battle_None\x10\x00\x12 \n\x1cMsg_Battle_RoomHeartbeat_Rsp\x10\x01\x12\x1c\n\x18Msg_Battle_EnterRoom_Req\x10\x02\x12\x1c\n\x18Msg_Battle_EnterRoom_Rsp\x10\x03\x12\x1c\n\x18Msg_Battle_StartRoom_Req\x10\x04\x12\x1c\n\x18Msg_Battle_StartRoom_Rsp\x10\x05\x12\x1c\n\x18Msg_Battle_OrderRoom_Req\x10\x06\x12\x1c\n\x18Msg_Battle_OrderRoom_Rsp\x10\x07\x12\x1b\n\x17Msg_Battle_QuitRoom_Req\x10\x08\x12\x1b\n\x17Msg_Battle_QuitRoom_Rsp\x10\t\x12\x1c\n\x18Msg_Battle_PauseRoom_Req\x10\n\x12\x1c\n\x18Msg_Battle_PauseRoom_Rsp\x10\x0b\x12\x1e\n\x1aMsg_Battle_RecoverRoom_Req\x10\x0c\x12\x1e\n\x1aMsg_Battle_RecoverRoom_Rsp\x10\r\x12\x1c\n\x18Msg_Battle_Result_Notify\x10\x0e\x12\x1b\n\x17Msg_Battle_OverRoom_Req\x10\x0f\x12\x1b\n\x17Msg_Battle_OverRoom_Rsp\x10\x10\x12 \n\x1cMsg_Battle_GetRoomOrders_Req\x10\x11\x12 \n\x1cMsg_Battle_GetRoomOrders_Rsp\x10\x12\x12!\n\x1dMsg_Battle_SyncOrdersOver_Req\x10\x13\x12!\n\x1dMsg_Battle_SyncOrdersOver_Rsp\x10\x14\x12\x1b\n\x17Msg_Battle_Error_Notify\x10\x15\x12\x17\n\x13Msg_Battle_Beat_Req\x10\x16\x12\x17\n\x13Msg_Battle_Beat_Rsp\x10\x17\x12\x1f\n\x1bMsg_RoomLoadProgress_Submit\x10\x18\x12\x1f\n\x1bMsg_RoomLoadProgress_Notify\x10\x19\x12\x1c\n\x18Msg_RoomHeartbeat_Notify\x10\x1a\x12\x18\n\x14Msg_RoomStart_Notify\x10\x1b\"\x15\n\x13Msg_Battle_Beat_Req\"\x15\n\x13Msg_Battle_Beat_Rsp\"N\n\x18Msg_Battle_EnterRoom_Req\x12\x18\n\x10strRoomSessionId\x18\x01 \x01(\t\x12\x18\n\x10strUserSessionId\x18\x02 \x01(\t\"\xb4\x01\n\x18Msg_Battle_EnterRoom_Rsp\x12\"\n\x05\x65Type\x18\x01 \x01(\x0e\x32\x13.ProtoMsg.ERoomType\x12\x12\n\nstrMapName\x18\x02 \x01(\t\x12\x11\n\tiRandSeed\x18\x03 \x01(\x05\x12&\n\x07\x65Status\x18\x04 \x01(\x0e\x32\x15.ProtoMsg.ERoomStatus\x12\x11\n\tiRoomTime\x18\x05 \x01(\x05\x12\x12\n\niStartTime\x18\x06 \x01(\x05\"f\n\x17Msg_Battle_Error_Notify\x12\x12\n\nuiModuleId\x18\x01 \x01(\r\x12\r\n\x05uiCmd\x18\x02 \x01(\r\x12(\n\neErrorCode\x18\x03 \x01(\x0e\x32\x14.ProtoMsg.ResultCode\"a\n\x12\x62\x61ttle_room_info_t\x12\x12\n\nstrSession\x18\x01 \x01(\t\x12&\n\teRoomType\x18\x02 \x01(\x0e\x32\x13.ProtoMsg.ERoomType\x12\x0f\n\x07mapName\x18\x03 \x01(\t\"a\n\x0e\x62\x61ttle_order_t\x12\x11\n\tiExecTime\x18\x01 \x01(\x05\x12\x0f\n\x07iUserId\x18\x02 \x01(\x05\x12\r\n\x05iType\x18\x03 \x01(\x05\x12\r\n\x05iArg1\x18\x04 \x01(\x05\x12\r\n\x05iArg2\x18\x05 \x01(\x05\"L\n\x17Msg_Battle_OverRoom_Req\x12\x11\n\toUserData\x18\x01 \x01(\x0c\x12\x10\n\x08iKeyType\x18\x02 \x01(\x05\x12\x0c\n\x04iCrc\x18\x03 \x01(\x05\"\x19\n\x17Msg_Battle_OverRoom_Rsp\";\n\x14load_progress_info_t\x12\x10\n\x08llUserid\x18\x01 \x01(\x03\x12\x11\n\tiProgress\x18\x02 \x01(\x05\"N\n\x1bMsg_RoomLoadProgress_Notify\x12/\n\x07vecLoad\x18\x01 \x03(\x0b\x32\x1e.ProtoMsg.load_progress_info_t\"o\n\x18Msg_RoomHeartbeat_Notify\x12\x13\n\x0biServerTime\x18\x01 \x01(\x05\x12\x11\n\tiRoomTime\x18\x02 \x01(\x05\x12+\n\tvecOrders\x18\x03 \x03(\x0b\x32\x18.ProtoMsg.battle_order_t\">\n\x14Msg_RoomStart_Notify\x12\x13\n\x0biServerTime\x18\x01 \x01(\x05\x12\x11\n\tiRoomTime\x18\x02 \x01(\x05\"s\n\x18Msg_Battle_Result_Notify\x12&\n\x0b\x65ReusltCode\x18\x01 \x01(\x0e\x32\x11.ProtoMsg.EBRCode\x12/\n\x07vecData\x18\x02 \x03(\x0b\x32\x1e.ProtoMsg.user_battle_result_tb\x06proto3')

_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, globals())
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'msg_module_battle_pb2', globals())
if _descriptor._USE_C_DESCRIPTORS == False:

  DESCRIPTOR._options = None
  _MSGMODULE_BATTLE._serialized_start=75
  _MSGMODULE_BATTLE._serialized_end=948
  _MSGMODULE_BATTLE_MSGCMD._serialized_start=96
  _MSGMODULE_BATTLE_MSGCMD._serialized_end=948
  _MSG_BATTLE_BEAT_REQ._serialized_start=778
  _MSG_BATTLE_BEAT_REQ._serialized_end=799
  _MSG_BATTLE_BEAT_RSP._serialized_start=803
  _MSG_BATTLE_BEAT_RSP._serialized_end=824
  _MSG_BATTLE_ENTERROOM_REQ._serialized_start=996
  _MSG_BATTLE_ENTERROOM_REQ._serialized_end=1074
  _MSG_BATTLE_ENTERROOM_RSP._serialized_start=1077
  _MSG_BATTLE_ENTERROOM_RSP._serialized_end=1257
  _MSG_BATTLE_ERROR_NOTIFY._serialized_start=1259
  _MSG_BATTLE_ERROR_NOTIFY._serialized_end=1361
  _BATTLE_ROOM_INFO_T._serialized_start=1363
  _BATTLE_ROOM_INFO_T._serialized_end=1460
  _BATTLE_ORDER_T._serialized_start=1462
  _BATTLE_ORDER_T._serialized_end=1559
  _MSG_BATTLE_OVERROOM_REQ._serialized_start=1561
  _MSG_BATTLE_OVERROOM_REQ._serialized_end=1637
  _MSG_BATTLE_OVERROOM_RSP._serialized_start=582
  _MSG_BATTLE_OVERROOM_RSP._serialized_end=607
  _LOAD_PROGRESS_INFO_T._serialized_start=1666
  _LOAD_PROGRESS_INFO_T._serialized_end=1725
  _MSG_ROOMLOADPROGRESS_NOTIFY._serialized_start=1727
  _MSG_ROOMLOADPROGRESS_NOTIFY._serialized_end=1805
  _MSG_ROOMHEARTBEAT_NOTIFY._serialized_start=1807
  _MSG_ROOMHEARTBEAT_NOTIFY._serialized_end=1918
  _MSG_ROOMSTART_NOTIFY._serialized_start=1920
  _MSG_ROOMSTART_NOTIFY._serialized_end=1982
  _MSG_BATTLE_RESULT_NOTIFY._serialized_start=1984
  _MSG_BATTLE_RESULT_NOTIFY._serialized_end=2099
# @@protoc_insertion_point(module_scope)
