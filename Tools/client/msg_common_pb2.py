# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: msg_common.proto
"""Generated protocol buffer code."""
from google.protobuf.internal import builder as _builder
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import symbol_database as _symbol_database
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\x10msg_common.proto\x12\x08ProtoMsg\":\n\x0bgame_item_t\x12\x0c\n\x04uiId\x18\x01 \x01(\r\x12\r\n\x05uiNum\x18\x02 \x01(\r\x12\x0e\n\x06llTime\x18\x03 \x01(\x03\"D\n\x12hero_common_info_t\x12\x0f\n\x07iHeroId\x18\x01 \x01(\x05\x12\x0e\n\x06iLevel\x18\x02 \x01(\x05\x12\r\n\x05iStar\x18\x03 \x01(\x05\"\xbb\x01\n\x12user_common_info_t\x12\r\n\x05llUid\x18\x01 \x01(\x03\x12\x0c\n\x04iPos\x18\x02 \x01(\x05\x12\x0f\n\x07strName\x18\x03 \x01(\t\x12\x0e\n\x06iLevel\x18\x04 \x01(\x05\x12.\n\x08vecHeros\x18\x05 \x03(\x0b\x32\x1c.ProtoMsg.hero_common_info_t\x12(\n\neRobotType\x18\x06 \x01(\x0e\x32\x14.ProtoMsg.ERobotType\x12\r\n\x05iSide\x18\x07 \x01(\x05\"\x90\x01\n\x12user_battle_info_t\x12\x18\n\x10strUserSessionId\x18\x01 \x01(\t\x12/\n\toUserInfo\x18\x02 \x01(\x0b\x32\x1c.ProtoMsg.user_common_info_t\x12\x11\n\tiMatchSec\x18\x03 \x01(\x05\x12\x0b\n\x03\x62Ob\x18\x04 \x01(\x08\x12\x0f\n\x07iConvId\x18\x05 \x01(\x05\"u\n\x14user_battle_result_t\x12\r\n\x05llUid\x18\x01 \x01(\x03\x12\x11\n\tiPosition\x18\x02 \x01(\x05\x12(\n\x07\x65Result\x18\x03 \x01(\x0e\x32\x17.ProtoMsg.EBattleResult\x12\x11\n\tllEndTime\x18\x04 \x01(\x03*N\n\rEPlayerStatus\x12\x13\n\x0f\x65Player_Offline\x10\x00\x12\x12\n\x0e\x65Player_Online\x10\x01\x12\x14\n\x10\x65Player_InBattle\x10\x02*h\n\tERoomType\x12\x12\n\x0e\x45RoomType_None\x10\x00\x12\x19\n\x15\x45RoomType_PVP_Leisure\x10\x01\x12\x15\n\x11\x45RoomType_PVP_1V1\x10\x02\x12\x15\n\x11\x45RoomType_PVP_2V2\x10\x03*\xed\x01\n\x0c\x45LeisureType\x12\x15\n\x11\x45LeisureType_None\x10\x00\x12\x16\n\x12\x45LeisureType_Melee\x10\x01\x12\x1b\n\x17\x45LeisureType_Stronghold\x10\x02\x12\x19\n\x15\x45LeisureType_RandBoss\x10\x03\x12\x1b\n\x17\x45LeisureType_RandGundam\x10\x04\x12\x1a\n\x16\x45LeisureType_BanGundam\x10\x05\x12\x1e\n\x1a\x45LeisureType_UnlimitedFire\x10\x06\x12\x1d\n\x19\x45LeisureType_SingleBattle\x10\x07*d\n\x0b\x45RoomStatus\x12\x14\n\x10\x45RoomStatus_None\x10\x00\x12\x15\n\x11\x45RoomStatus_Start\x10\x01\x12\x13\n\x0f\x45RoomStatus_Run\x10\x02\x12\x13\n\x0f\x45RoomStatus_End\x10\x03*\xc2\x01\n\nERobotType\x12\x13\n\x0f\x45RobotType_None\x10\x00\x12\x15\n\x11\x45RobotType_Level1\x10\x01\x12\x15\n\x11\x45RobotType_Level2\x10\x02\x12\x15\n\x11\x45RobotType_Level3\x10\x03\x12\x15\n\x11\x45RobotType_Level4\x10\x04\x12\x15\n\x11\x45RobotType_Level5\x10\x05\x12\x15\n\x11\x45RobotType_Level6\x10\x06\x12\x15\n\x11\x45RobotType_Level7\x10\x64*q\n\rEBattleResult\x12\x16\n\x12\x45\x42\x61ttleResult_None\x10\x00\x12\x15\n\x11\x45\x42\x61ttleResult_Win\x10\x01\x12\x16\n\x12\x45\x42\x61ttleResult_Lose\x10\x02\x12\x19\n\x15\x45\x42\x61ttleResult_DagFall\x10\x03*\x8e\x02\n\x07\x45\x42RCode\x12\x13\n\x0f\x45\x42RCode_Success\x10\x00\x12\x10\n\x0c\x45\x42RCode_Diff\x10\x01\x12\x14\n\x10\x45\x45\x42RCode_WinSame\x10\x02\x12\x13\n\x0f\x45\x42RCode_PvgDiff\x10\x03\x12\x11\n\rEBRCode_Empty\x10\x04\x12\x13\n\x0f\x45\x42RCode_Timeout\x10\x05\x12\x13\n\x0f\x45\x42RCode_NoStart\x10\x06\x12\x10\n\x0c\x45\x42RCode_Oper\x10\x07\x12\x16\n\x12\x45\x42RCode_ResTimeout\x10\x08\x12\x17\n\x13\x45\x42RCode_OfflineDiff\x10\t\x12\x1a\n\x16\x45\x42RCode_OfflineTimeout\x10\n\x12\x15\n\x11\x45\x42RCode_OneunSync\x10\x0b\x62\x06proto3')

_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, globals())
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'msg_common_pb2', globals())
if _descriptor._USE_C_DESCRIPTORS == False:

  DESCRIPTOR._options = None
  _EPLAYERSTATUS._serialized_start=616
  _EPLAYERSTATUS._serialized_end=694
  _EROOMTYPE._serialized_start=696
  _EROOMTYPE._serialized_end=800
  _ELEISURETYPE._serialized_start=803
  _ELEISURETYPE._serialized_end=1040
  _EROOMSTATUS._serialized_start=1042
  _EROOMSTATUS._serialized_end=1142
  _EROBOTTYPE._serialized_start=1145
  _EROBOTTYPE._serialized_end=1339
  _EBATTLERESULT._serialized_start=1341
  _EBATTLERESULT._serialized_end=1454
  _EBRCODE._serialized_start=1457
  _EBRCODE._serialized_end=1727
  _GAME_ITEM_T._serialized_start=30
  _GAME_ITEM_T._serialized_end=88
  _HERO_COMMON_INFO_T._serialized_start=90
  _HERO_COMMON_INFO_T._serialized_end=158
  _USER_COMMON_INFO_T._serialized_start=161
  _USER_COMMON_INFO_T._serialized_end=348
  _USER_BATTLE_INFO_T._serialized_start=351
  _USER_BATTLE_INFO_T._serialized_end=495
  _USER_BATTLE_RESULT_T._serialized_start=497
  _USER_BATTLE_RESULT_T._serialized_end=614
# @@protoc_insertion_point(module_scope)
