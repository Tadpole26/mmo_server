#pragma once
#include "sdk.h"

namespace CGame
{
	extern uint32 uiDBNum;								//账号库个数
	extern uint32 uiDBTableNum;							//每个库表个数
	extern uint32 uiCreateRoleMax;						//一个accId在一个区的角色上限
	
	static const uint32 uiAccIdBitMax = 35;				//账号最大位数	
	static const uint64 ullAccIdValMin = 1;				//账号最小值数
	static const uint64 ullAccIdValMax = 0x7FFFFFFFF;	//账号最大值数(35位数)
	static const uint32 uiTblIdBitMax = 8;
	static const uint32 uiTblIdValMin = 0;
	static const uint32 uiTblIdValMax = 0xFF;
	static const uint32 uiAutoIdValMin = 1;
	static const uint32 uiAutoIdValMax = 0x7FFFFFF;
	static const uint32 uiZoneIdBitMax = 13;
	static const uint32 uiZoneIdValMin = 1;
	static const uint32 uiZoneIdValMax = 0xFFFF;
	static const uint32 uiRoleNumIdBitMax = 3;
	static const uint32 uiRoleNumIdValMin = 0;
	static const uint32 uiRoleNumIdValMax = 0x7;
	static const uint32 uiAutoIdBitMax = uiAccIdBitMax - uiTblIdBitMax;

	//dbid是否有效
	inline bool DBIdValid(const uint32 uiDBId)
	{
		return uiDBId < uiDBNum;
	}

	//tableid是否有效
	inline bool TableIdValid(const uint32 uiTblId)
	{
		return uiTblId >= uiAutoIdValMin && uiTblId <= uiTblIdValMax;
	}

	//autoid是否有效
	inline bool AutoIdValid(const uint32 uiAutoId)
	{
		return uiAutoId >= uiAutoIdValMin && uiAutoId <= uiAutoIdValMax;
	}

	//zoneId是否有效
	inline bool ZoneIdValid(const uint32 uiZoneId)
	{
		return uiZoneId <= uiZoneIdValMax;
	}

	//zoneId是否为0
	inline bool ZoneIdZero(uint32 uiZoneId)
	{
		return uiZoneId == 0;
	}

	//roleNumId是否有效
	inline bool RoleNumIdValid(const uint32 uiRoleNumId)
	{
		return uiRoleNumId >= uiRoleNumIdValMin && uiRoleNumId <= uiRoleNumIdValMax;
	}

	//accid是否有效(不能超过35位)
	inline bool AccIdValid(uint64 ullAccId)
	{
		return ullAccId >= ullAccIdValMin && ullAccId < ullAccIdValMax;
	}

	//accid转hanshid
	inline uint32 Acc2HashId(const char* str)
	{
		uint32 uiSeed = 131; //31 131 1313 13131 131313
		uint32 uiHashVal = 0;
		while (*str)
		{
			uiHashVal = uiHashVal * uiSeed + *str++;
		}
		return uiHashVal & 0xFFFFFFFF;
	}

	//tableId转dbId
	inline uint32 TableId2DBId(const uint32 uiTblId)
	{
		return uiTblId / uiDBTableNum;
	}

	//hashId转tableId
	inline uint32 HashId2TableId(const uint32 uiHashId)
	{
		return uiHashId % (uiDBTableNum * uiDBNum);
	}

	//hashId转DBId
	inline uint32 HashId2DBId(const uint32 uiHashId)
	{
		return TableId2DBId(HashId2TableId(uiHashId));
	}

	//accId转tableId
	inline uint32 Acc2TableId(const char* pAccount)
	{
		return HashId2TableId(Acc2HashId(pAccount));
	}

	//acci转DBId
	inline uint32 Acc2DBId(const char* pAccount)
	{
		return HashId2DBId(Acc2HashId(pAccount));
	}

	//合成accid(账号id的高8位是tableid,低27位自增id)
	inline uint64 UnionAccId(const uint32 uiTblId, const uint32 uiAutoId)
	{
		assert(TableIdValid(uiTblId));
		assert(AutoIdValid(uiAutoId));
		return (((uint64)uiTblId) << uiAutoIdBitMax) + ((uint64)uiAutoId);
	}

	//accId转tableId(取高8位)
	inline uint32 AccId2TableId(const uint64 ullAccId)
	{
		assert(AccIdValid(ullAccId));
		return uint32(ullAccId >> uiAutoIdBitMax);
	}

	//accId转DBId
	inline uint32 AccId2DBId(const uint64 ullAccId)
	{
		assert(AccIdValid(ullAccId));
		return TableId2DBId(AccId2TableId(ullAccId));
	}

	//accId转autoId
	inline uint32 AccId2AutoId(const uint64 ullAccId)
	{
		assert(AccIdValid(ullAccId));
		return (ullAccId << (64 - uiAutoIdBitMax)) >> (64 - uiAutoIdBitMax);
	}

	//合成robot的roleId
	inline uint64 UnionRobotId(const uint32 uiZoneId, const uint32 uiAutoId)
	{
		assert(ZoneIdValid(uiZoneId));
		return (((uint64)uiZoneId) << (uiAccIdBitMax + uiRoleNumIdBitMax)) + uiAutoId;

	}

	//合成roleId
	inline uint64 UnionRoleId(const uint32 uiZoneId, const uint64 ullAccId, const uint32 uiRoleNumId)
	{
		assert(ZoneIdValid(uiZoneId));
		assert(AccIdValid(ullAccId));
		assert(RoleNumIdValid(uiRoleNumId));
		return (((uint64)uiZoneId) << (uiAccIdBitMax + uiRoleNumIdBitMax)) + (uint64)(ullAccId << uiRoleNumIdBitMax) + (uint64)uiRoleNumId;
	}

	//roleId转zoneId
	inline uint32 RoleId2ZoneId(const uint64 ullRoleId)
	{
		return ullRoleId >> (uiAccIdBitMax + uiRoleNumIdBitMax);
	}

	//roleId转accId
	inline uint64 RoleId2AccId(const uint64 ullRoleId)
	{
		return (uint64)(ullRoleId << (64 - uiAccIdBitMax - uiRoleNumIdBitMax)) >> (64 - uiAccIdBitMax);
	}

	//roleId转roleNumId
	inline uint32 RoleId2RoleNumId(const uint64 ullRoleId)
	{
		return (ullRoleId << (64 - uiRoleNumIdBitMax)) >> (64 - uiRoleNumIdBitMax);
	}

	//roleId转autoId
	inline uint32 RoleId2AutoId(const uint64 ullRoleId)
	{
		return AccId2AutoId(RoleId2AccId(ullRoleId));
	}

	//roleId转tableId
	inline uint32 RoleId2TableId(const uint64 ullRoleId)
	{
		return AccId2TableId(RoleId2AccId(ullRoleId));
	}

	//roleId转DBId
	inline uint32 RoleId2DBId(const uint64 ullRoleId)
	{
		return AccId2DBId(RoleId2AccId(ullRoleId));
	}
}
