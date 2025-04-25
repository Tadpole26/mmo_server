#pragma once

#include "sdk.h"

using zUnitIdType = uint64;							//匹配单元id
using zRoomIdType = uint64;							//匹配成功后房间id

namespace cMatch
{
	constexpr uint32 szMaxKeepSeason = 2;			//保留历史赛季个数
	enum
	{
		ePunishEscapre = 1,								//逃跑
		ePunishRefuse,									//拒绝确认
		ePunishReset,									//惩罚重置
	};
	enum
	{
		eConfirmNone,									//默认
		eConfirmOk,										//确认ok
		eConfirmRefuse,									//确认拒绝
		eConfirmTimeout,								//确认超时
		eConfirmForceCancel,							//确认强制取消
	};
	enum
	{
		eLoadingNone,									//默认
		eLoadingOk,										//载入ok
		eLoadingTimeout,								//载入吵时
	};
	enum
	{
		eRoomGroupIdNone,								//默认
		eRoomGroupIdRed,								//房间红色
		eRoomGroupIdBlue,								//房间蓝色
		eRoomGroupIdMax,
	};
	enum
	{
		eRestartMatchFromRoom = 1,						//从房间中重新匹配
		eRestartMatchFromQueue,							//从队列中重新匹配
	};
	enum
	{
		eMatchEvilNormal = 1,
		eMatchEvilChallege = 2,
	};
	enum
	{
		eMatchNone = 0,
		eMatchF33 = 1,
		eMatchF55 = 2,
	};
	inline const char* eMatch2Str(uint32 mtype)
	{
		switch (mtype)
		{
		case eMatchF33: return "比武3v3"; break;
		case eMatchF55: return "比武5v5"; break;
		default: assert(false);			  break;
		}
		return "UnknowMatchType";
	}
	inline uint32 eMatch2MatchGroupSize(uint32 mtype)
	{
		switch (mtype)
		{
		case eMatchF33: return 3; break;
		case eMatchF55: return 5; break;
		default: assert(false);	break;
		}
		return std::numeric_limits<uint32>::max();
	}
	inline bool validRoomGroupId(uint32 groupId)
	{
		return groupId > eRoomGroupIdNone && groupId < eRoomGroupIdMax;
	}
	inline bool foreachRoomGroup(std::function<bool(uint32)> func)
	{
		for (auto groupId = eRoomGroupIdNone + 1; groupId < eRoomGroupIdMax; ++groupId)
		{
			if (!func(groupId)) return false;
		}
		return true;
	}
	inline uint64 unionMgrId(uint32 eproc, uint32 eparam, uint32 mtype, uint32 matchId, uint32 subtype = 0)
	{
		return (uint64)eproc * 10000000000000000ul + (uint64)eparam * 10000000000ul + (uint64)mtype * 100000000ul + (uint64)subtype * 1000000ul + (uint64)matchId;
	}
	inline uint32 splitEprocMgrId(uint64 mgrId)
	{
		return uint32(mgrId / 10000000000000000ul);
	}
	inline uint32 splitEparamMgrId(uint64 mgrId)
	{
		return uint32(mgrId % 10000000000000000ul / 10000000000ul);
	}
	inline uint32 splitMtypeMgrId(uint64 mgrId)
	{
		return uint32(mgrId % 10000000000ul / 100000000ul);
	}
	inline uint32 splitSubtypeMgrId(uint64 mgrId)
	{
		return uint32(mgrId % 100000000ul / 1000000ul);
	}
	inline uint32 splitMatchIdMgrId(uint64 mgrId)
	{
		return uint32(mgrId % 1000000ul);
	}
	inline zUnitIdType unionUnitId(uint32 eproc, uint32 timestamp, uint32 autoId)
	{
		return ((uint64)eproc << 61) + ((uint64)timestamp << 29) + (uint64)autoId;
	}
	inline uint32 splitEprocUnitId(zUnitIdType unitId)
	{
		return unitId >> 61;
	}
	inline zRoomIdType unionRoomId(uint32 eproc, uint32 timestamp, uint32 autoId)
	{
		return ((uint64)eproc << 61) + ((uint64)timestamp << 29) + (uint64)autoId;
	}
	inline uint32 splitEprocRoomId(zRoomIdType roomId)
	{
		return roomId >> 61;
	}
}

