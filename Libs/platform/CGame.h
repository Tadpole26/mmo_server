#pragma once
#include "sdk.h"

namespace cGame
{
	extern uint32 uiDBNum;								//�˺ſ����
	extern uint32 uiDBTableNum;							//ÿ��������
	extern uint32 uiCreateRoleMax;						//һ��accId��һ�����Ľ�ɫ����
	
	static const uint32	accIdBitMax = 35;				//�˺����λ��	
	static const uint64 accIdValMin = 1;				//�˺���Сֵ��
	static const uint64 accIdValMax = 0x7FFFFFFFF;	//�˺����ֵ��(35λ��)
	static const uint32 tblIdBitMax = 8;
	static const uint32 tblIdValMin = 0;
	static const uint32 tblIdValMax = 0xFF;
	static const uint32 autoIdValMin = 1;
	static const uint32 autoIdValMax = 0x7FFFFFF;
	static const uint32 zoneIdBitMax = 13;
	static const uint32 zoneIdValMin = 1;
	static const uint32 zoneIdValMax = 0xFFFF;
	static const uint32 roleNumIdBitMax = 3;
	static const uint32 roleNumIdValMin = 0;
	static const uint32 roleNumIdValMax = 0x7;
	static const uint32 autoIdBitMax = accIdBitMax - tblIdBitMax;

	//dbid�Ƿ���Ч
	inline bool DBIdValid(const uint32 uiDBId)
	{
		return uiDBId < uiDBNum;
	}

	//tableid�Ƿ���Ч
	inline bool TableIdValid(const uint32 uiTblId)
	{
		return uiTblId >= autoIdValMin && uiTblId <= tblIdValMax;
	}

	//autoid�Ƿ���Ч
	inline bool AutoIdValid(const uint32 uiAutoId)
	{
		return uiAutoId >= autoIdValMin && uiAutoId <= autoIdValMax;
	}

	//zoneId�Ƿ���Ч
	inline bool ZoneIdValid(const uint32 uiZoneId)
	{
		return uiZoneId <= zoneIdValMax;
	}

	//zoneId�Ƿ�Ϊ0
	inline bool ZoneIdZero(uint32 uiZoneId)
	{
		return uiZoneId == 0;
	}

	//roleNumId�Ƿ���Ч
	inline bool RoleNumIdValid(const uint32 uiRoleNumId)
	{
		return uiRoleNumId >= roleNumIdValMin && uiRoleNumId <= roleNumIdValMax;
	}

	//accid�Ƿ���Ч(���ܳ���35λ)
	inline bool AccIdValid(uint64 ullAccId)
	{
		return ullAccId >= accIdValMin && ullAccId < accIdValMax;
	}

	//accidתhanshid
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

	//tableIdתdbId
	inline uint32 TableId2DBId(const uint32 uiTblId)
	{
		return uiTblId / uiDBTableNum;
	}
	//hashIdתtableId
	inline uint32 HashId2TableId(const uint32 uiHashId)
	{
		return uiHashId % (uiDBTableNum * uiDBNum);
	}
	//hashIdתDBId
	inline uint32 HashId2DBId(const uint32 uiHashId)
	{
		return TableId2DBId(HashId2TableId(uiHashId));
	}
	//accIdתtableId
	inline uint32 Acc2TableId(const char* pAccount)
	{
		return HashId2TableId(Acc2HashId(pAccount));
	}
	//acciתDBId
	inline uint32 Acc2DBId(const char* pAccount)
	{
		return HashId2DBId(Acc2HashId(pAccount));
	}
	//�ϳ�accid(�˺�id�ĸ�8λ��tableid,��27λ����id)
	inline uint64 UnionAccId(const uint32 uiTblId, const uint32 uiAutoId)
	{
		assert(TableIdValid(uiTblId));
		assert(AutoIdValid(uiAutoId));
		return (((uint64)uiTblId) << autoIdBitMax) + ((uint64)uiAutoId);
	}
	//accIdתtableId(ȡ��8λ)
	inline uint32 AccId2TableId(const uint64 ullAccId)
	{
		assert(AccIdValid(ullAccId));
		return uint32(ullAccId >> autoIdBitMax);
	}
	//accIdתDBId
	inline uint32 AccId2DBId(const uint64 ullAccId)
	{
		assert(AccIdValid(ullAccId));
		return TableId2DBId(AccId2TableId(ullAccId));
	}
	//accIdתautoId
	inline uint32 AccId2AutoId(const uint64 ullAccId)
	{
		assert(AccIdValid(ullAccId));
		return (ullAccId << (64 - autoIdBitMax)) >> (64 - autoIdBitMax);
	}
	//�ϳ�robot��roleId
	inline uint64 UnionRobotId(const uint32 uiZoneId, const uint32 uiAutoId)
	{
		assert(ZoneIdValid(uiZoneId));
		return (((uint64)uiZoneId) << (accIdBitMax + roleNumIdBitMax)) + uiAutoId;

	}
	//�ϳ�roleId
	inline uint64 UnionRoleId(const uint32 uiZoneId, const uint64 ullAccId, const uint32 uiRoleNumId)
	{
		assert(ZoneIdValid(uiZoneId));
		assert(AccIdValid(ullAccId));
		assert(RoleNumIdValid(uiRoleNumId));
		return (((uint64)uiZoneId) << (accIdBitMax + roleNumIdBitMax)) + (uint64)(ullAccId << roleNumIdBitMax) + (uint64)uiRoleNumId;
	}
	//roleIdתzoneId
	inline uint32 RoleId2ZoneId(const uint64 ullRoleId)
	{
		return ullRoleId >> (accIdBitMax + roleNumIdBitMax);
	}
	//roleIdתaccId
	inline uint64 RoleId2AccId(const uint64 ullRoleId)
	{
		return (uint64)(ullRoleId << (64 - accIdBitMax - roleNumIdBitMax)) >> (64 - accIdBitMax);
	}
	//roleIdתroleNumId
	inline uint32 RoleId2RoleNumId(const uint64 ullRoleId)
	{
		return (ullRoleId << (64 - roleNumIdBitMax)) >> (64 - roleNumIdBitMax);
	}
	//roleIdתautoId
	inline uint32 RoleId2AutoId(const uint64 ullRoleId)
	{
		return AccId2AutoId(RoleId2AccId(ullRoleId));
	}
	//roleIdתtableId
	inline uint32 RoleId2TableId(const uint64 ullRoleId)
	{
		return AccId2TableId(RoleId2AccId(ullRoleId));
	}
	//roleIdתDBId
	inline uint32 RoleId2DBId(const uint64 ullRoleId)
	{
		return AccId2DBId(RoleId2AccId(ullRoleId));
	}
	//�ϳ�Robot��roleId
	inline zRoleIdType unionRobotId(const uint32 zoneId, const uint32 autoId)
	{
		return (((zRoleIdType)zoneId) << (accIdBitMax + roleNumIdBitMax)) + autoId;
	}
}
