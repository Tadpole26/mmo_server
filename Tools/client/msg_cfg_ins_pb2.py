# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: msg_cfg_ins.proto
"""Generated protocol buffer code."""
from google.protobuf.internal import builder as _builder
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import symbol_database as _symbol_database
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\x11msg_cfg_ins.proto\x12\x06packet\"(\n\nGrid_Point\x12\x0c\n\x04PosX\x18\x01 \x01(\r\x12\x0c\n\x04PosY\x18\x02 \x01(\r\"<\n\rGrid_Rotation\x12\r\n\x05RotaX\x18\x01 \x01(\r\x12\r\n\x05RotaY\x18\x02 \x01(\r\x12\r\n\x05RotaZ\x18\x03 \x01(\r\"S\n\tGrid_Rect\x12\"\n\x06minPos\x18\x01 \x01(\x0b\x32\x12.packet.Grid_Point\x12\"\n\x06maxPos\x18\x02 \x01(\x0b\x32\x12.packet.Grid_Point\"\xb8\x01\n\x0bGrid_RigNPC\x12\r\n\x05npcId\x18\x01 \x01(\r\x12&\n\x0btriggerRect\x18\x02 \x01(\x0b\x32\x11.packet.Grid_Rect\x12#\n\x04rota\x18\x03 \x01(\x0b\x32\x15.packet.Grid_Rotation\x12\x1f\n\x03pos\x18\x04 \x01(\x0b\x32\x12.packet.Grid_Point\x12,\n\x11triggerRectSecond\x18\x05 \x01(\x0b\x32\x11.packet.Grid_Rect\"3\n\nMap_RigNPC\x12%\n\x08vecGrids\x18\x01 \x03(\x0b\x32\x13.packet.Grid_RigNPC\"\xb4\x01\n\x0fGrid_Teleporter\x12\x0f\n\x07telType\x18\x01 \x01(\r\x12\x11\n\ttargetMap\x18\x02 \x01(\r\x12\x0f\n\x07telAnim\x18\x03 \x01(\r\x12&\n\x0btriggerRect\x18\x04 \x01(\x0b\x32\x11.packet.Grid_Rect\x12#\n\x04rota\x18\x05 \x01(\x0b\x32\x15.packet.Grid_Rotation\x12\x1f\n\x03pos\x18\x06 \x01(\x0b\x32\x12.packet.Grid_Point\";\n\x0eMap_Teleporter\x12)\n\x08vecGrids\x18\x01 \x03(\x0b\x32\x17.packet.Grid_Teleporter\"_\n\x0eGrid_BlockArea\x12\x13\n\x0b\x62lockAreaId\x18\x01 \x01(\r\x12$\n\tblockRect\x18\x02 \x01(\x0b\x32\x11.packet.Grid_Rect\x12\x12\n\ninstanceId\x18\x03 \x01(\x04\"9\n\rMap_BlockArea\x12(\n\x08vecGrids\x18\x01 \x03(\x0b\x32\x16.packet.Grid_BlockArea\"\xf6\x01\n\x11Grid_MonsterGroup\x12\x42\n\tvecGroups\x18\x01 \x03(\x0b\x32/.packet.Grid_MonsterGroup.Grid_MonsterGroupUnit\x1a\x9c\x01\n\x15Grid_MonsterGroupUnit\x12\x16\n\x0emonsterGroupId\x18\x01 \x01(\r\x12%\n\ncreateRect\x18\x02 \x01(\x0b\x32\x11.packet.Grid_Rect\x12#\n\x04rota\x18\x03 \x01(\x0b\x32\x15.packet.Grid_Rotation\x12\x1f\n\x03pos\x18\x04 \x01(\x0b\x32\x12.packet.Grid_Point\"?\n\x10Map_MonsterGroup\x12+\n\x08vecGrids\x18\x01 \x03(\x0b\x32\x19.packet.Grid_MonsterGroup\")\n\x08Map_Info\x12\r\n\x05width\x18\x01 \x01(\r\x12\x0e\n\x06height\x18\x02 \x01(\r\"\xda\x01\n\x0cMap_AllTypes\x12\x1f\n\x04type\x18\x01 \x01(\x0e\x32\x11.packet.BrushType\x12%\n\tmapRigNPC\x18\x02 \x01(\x0b\x32\x12.packet.Map_RigNPC\x12&\n\x06mapTel\x18\x03 \x01(\x0b\x32\x16.packet.Map_Teleporter\x12\'\n\x08mapBlock\x18\x04 \x01(\x0b\x32\x15.packet.Map_BlockArea\x12\x31\n\x0fmapMonsterGroup\x18\x05 \x01(\x0b\x32\x18.packet.Map_MonsterGroup*\x84\x01\n\tBrushType\x12\x12\n\x0e\x42rushType_None\x10\x00\x12\x14\n\x10\x42rushType_RigNPC\x10\x01\x12\x18\n\x14\x42rushType_Teleporter\x10\x02\x12\x17\n\x13\x42rushType_BlockArea\x10\x03\x12\x1a\n\x16\x42rushType_MonsterGroup\x10\x04\x62\x06proto3')

_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, globals())
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'msg_cfg_ins_pb2', globals())
if _descriptor._USE_C_DESCRIPTORS == False:

  DESCRIPTOR._options = None
  _BRUSHTYPE._serialized_start=1437
  _BRUSHTYPE._serialized_end=1569
  _GRID_POINT._serialized_start=29
  _GRID_POINT._serialized_end=69
  _GRID_ROTATION._serialized_start=71
  _GRID_ROTATION._serialized_end=131
  _GRID_RECT._serialized_start=133
  _GRID_RECT._serialized_end=216
  _GRID_RIGNPC._serialized_start=219
  _GRID_RIGNPC._serialized_end=403
  _MAP_RIGNPC._serialized_start=405
  _MAP_RIGNPC._serialized_end=456
  _GRID_TELEPORTER._serialized_start=459
  _GRID_TELEPORTER._serialized_end=639
  _MAP_TELEPORTER._serialized_start=641
  _MAP_TELEPORTER._serialized_end=700
  _GRID_BLOCKAREA._serialized_start=702
  _GRID_BLOCKAREA._serialized_end=797
  _MAP_BLOCKAREA._serialized_start=799
  _MAP_BLOCKAREA._serialized_end=856
  _GRID_MONSTERGROUP._serialized_start=859
  _GRID_MONSTERGROUP._serialized_end=1105
  _GRID_MONSTERGROUP_GRID_MONSTERGROUPUNIT._serialized_start=949
  _GRID_MONSTERGROUP_GRID_MONSTERGROUPUNIT._serialized_end=1105
  _MAP_MONSTERGROUP._serialized_start=1107
  _MAP_MONSTERGROUP._serialized_end=1170
  _MAP_INFO._serialized_start=1172
  _MAP_INFO._serialized_end=1213
  _MAP_ALLTYPES._serialized_start=1216
  _MAP_ALLTYPES._serialized_end=1434
# @@protoc_insertion_point(module_scope)
