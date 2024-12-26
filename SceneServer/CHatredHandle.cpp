#include "CHatredHandle.h"
#include "util_time.h"

CHatredHandleArr::CHatredHandleArr() : CHatredHandle()
{
}

CHatredHandleArr::~CHatredHandleArr()
{
}

void CHatredHandleArr::Init()
{
	for (uint32 i = 0; i < m_arrHatred.size(); ++i)
		m_arrHatred[i].index = i;
}

void CHatredHandleArr::Final()
{
}

bool CHatredHandleArr::Empty() const
{
	for (uint32 i = 0; i < m_arrHatred.size(); ++i)
	{
		const HatredEntity& oHatred = m_arrHatred[i];
		if (!oHatred.entity.empty())
			return false;
	}
	return true;
}

void CHatredHandleArr::Clear()
{
	for (uint32 i = 0; i < m_arrHatred.size(); ++i)
	{
		HatredEntity& oHatred = m_arrHatred[i];
		if (!oHatred.entity.empty())
			continue;
		oHatred.clear();
	}
	m_oLastHatred.clear();
}

bool CHatredHandleArr::ForeachByOrder(std::function<bool(uint32, uint64, const Entity&)> func)
{
	assert(func);

	std::list<HatredEntity*> lstFound;
	for (uint32 i = 0; i < m_arrHatred.size(); ++i)
	{
		HatredEntity& oHatred = m_arrHatred[i];
		if (oHatred.entity.empty())
			continue;
		lstFound.push_back(&oHatred);
	}
	if (lstFound.empty())
		return true;
	//按仇恨值排序
	lstFound.sort([](HatredEntity* lVal, HatredEntity* rVal) { return lVal->value > rVal->value; });
	uint32 i = 0;
	for (auto* pHatred : lstFound)
	{
		if (!func(++i, pHatred->value, pHatred->entity))
			return false;
	}
	return true;
}

CHatredHandle::Entity CHatredHandleArr::SelectHatred(std::function<bool(const Entity&)> func)
{
	assert(func);

	HatredEntity* pFound = nullptr;
	[&]
		{
			std::list<HatredEntity*> lstFound;
			for (uint32 i = 0; i < m_arrHatred.size(); ++i)
			{
				HatredEntity& oHatred = m_arrHatred[i];
				if (oHatred.entity.empty())
					continue;
				lstFound.push_back(&oHatred);
				if (!pFound || pFound->value < oHatred.value)
					pFound = &oHatred;
			}
			if (!pFound) return;
			//满足取最大仇恨对象,最大仇恨对象不满足直接清除
			if (!func(pFound->entity)) return;
			pFound->clear();
			zMisc::listErase(lstFound, pFound);
			pFound = nullptr;
			lstFound.sort([](HatredEntity* lVal, HatredEntity* rVal) { return lVal->value > rVal->value; });
			if (lstFound.empty())
				return;
			for (HatredEntity* pEntity : lstFound)
			{
				if (!func(pEntity->entity))
				{
					pFound = pEntity;
					break;
				}
				pEntity->clear();
			}
		}();
	
	return pFound ? pFound->entity : Entity();
}

void CHatredHandleArr::ClearHatred(const Entity& oEntity)
{
	for (uint32 i = 0; i < m_arrHatred.size(); ++i)
	{
		HatredEntity& oHatred = m_arrHatred[i];
		if (oHatred.entity.equal(oEntity))
		{
			oHatred.clear();
			break;
		}
	}
}

void  CHatredHandleArr::AddHatred(const Entity& oEntity, uint32 uiVal)
{
	uint32 uiKnockOut = kInvalidHatredIndex;
	uint64 uiCurTime = GetCurrTime();
	for (uint32 i = 0; i < m_arrHatred.size(); ++i)
	{
		HatredEntity& oHatred = m_arrHatred[i];
		if (oHatred.entity.equal(oEntity))
		{
			oHatred.value = oHatred.value + uiVal;
			oHatred.updateTime = uiCurTime;
			return;
		}
		if (oHatred.entity.empty())
		{
			oHatred.entity = oEntity;
			oHatred.updateTime = uiCurTime;
			oHatred.value = std::min(uiVal, GetHatredLimit());
		}
		//超过时间仇恨值未更新,踢出仇恨列表
		if (oHatred.updateTime + kLoseHatredInterval < uiCurTime && uiKnockOut == kInvalidHatredIndex)
			uiKnockOut = i;
	}
	if (uiKnockOut != kInvalidHatredIndex)
	{
		HatredEntity& oHatred = m_arrHatred[uiKnockOut];
		oHatred.entity = oEntity;
		oHatred.updateTime = uiCurTime;
		oHatred.value = std::min(uiVal, GetHatredLimit());
	}
}

void  CHatredHandleArr::SubHatred(const Entity& oEntity, uint32 uiVal)
{
	uint64 uiCurTime = GetCurrTime();
	for (uint32 i = 0; i < m_arrHatred.size(); ++i)
	{
		HatredEntity& oHatred = m_arrHatred[i];
		if (!oHatred.entity.equal(oEntity))
			continue;
		if (oHatred.value > uiVal)
		{
			oHatred.value -= uiVal;
			oHatred.updateTime = uiCurTime;
		}
		else
		{
			oHatred.clear();

		}
		break;
	}
}

uint64  CHatredHandleArr::GetHatred(const Entity& oEntity)
{
	for (uint32 i = 0; i < m_arrHatred.size(); ++i)
	{
		const HatredEntity& oHatred = m_arrHatred[i];
		if (oHatred.entity.equal(oEntity))
			return oHatred.value;
	}
	return 0;
}

bool CHatredHandleArr::HasHatred(const Entity& oEntity)
{
	for (uint32 i = 0; i < m_arrHatred.size(); ++i)
	{
		const HatredEntity& oHatred = m_arrHatred[i];
		if (oHatred.entity.equal(oEntity))
			return true;
	}
	return false;
}

CHatredHandle::Entity CHatredHandleArr::GetLastHatred() const
{
	return m_oLastHatred;
}

/////////////////////////////////////////////////////////////////////////////////////
CHatredHandleMap::CHatredHandleMap() : 
	CHatredHandle()
{
}

CHatredHandleMap::~CHatredHandleMap()
{
}

void CHatredHandleMap::Init()
{
}

void CHatredHandleMap::Final()
{
}

bool CHatredHandleMap::Empty() const
{
	return m_zsetHatred.Size() == 0;
}

void CHatredHandleMap::Clear()
{
	for (auto iter = m_zsetHatred.Begin(); iter != m_zsetHatred.End();)
		iter = m_zsetHatred.Erase(iter);

	m_mapUpdateFlag.clear();
	m_uiResetFlag = 1;
}

bool CHatredHandleMap::ForeachByOrder(std::function<bool(uint32, uint64, const Entity&)> func)
{
	uint32 uiRank = 0;
	for (auto iter = m_zsetHatred.Begin(); iter != m_zsetHatred.End(); iter++)
	{
		++uiRank;
		if (!func(uiRank, iter->second, iter->first.second)) return false;
		return true;
	}
	return true;
}

CHatredHandle::Entity CHatredHandleMap::SelectHatred(std::function<bool(const Entity&)> func)
{
	assert(func);
	//上一个仇恨对象是否参与判断
	bool bLastHatred = true;
	if (func(m_oLastHatred))
		bLastHatred = false;
	auto iterLast = m_zsetHatred.Find(m_oLastHatred.hashValue());
	if (iterLast == m_zsetHatred.End())
	{
		bLastHatred = false;
		m_oLastHatred.clear();
	}

	//先判断仇恨列表
	for (auto iter = m_zsetHatred.Begin(); iter != m_zsetHatred.End(); iter++)
	{
		if (bLastHatred && iterLast->second < iter->second)
			return m_oLastHatred;

		if (func(iter->first.second))
			return Entity{};

		return iter->first.second;
	}
	return Entity{};
}

void CHatredHandleMap::ClearHatred(const Entity& oEntity)
{
	uint64 ullEntityHash = oEntity.hashValue();
	auto iter = m_zsetHatred.Find(ullEntityHash);
	if (iter != m_zsetHatred.End())
		m_zsetHatred.Erase(iter);
	m_mapUpdateFlag[ullEntityHash] = 0;
}

void CHatredHandleMap::AddHatred(const Entity& oEntity, uint32 uiVal)
{
	if (!uiVal) return;
	uint64 ullEntityHash = oEntity.hashValue();
	m_zsetHatred.UpdateOrInsert({ ullEntityHash, oEntity }, uiVal);
	m_mapUpdateFlag[ullEntityHash] = 1;
	m_uiResetFlag = 0;
}

void CHatredHandleMap::SubHatred(const Entity& oEntity, uint32 uiVal)
{
	if (!uiVal) return;
	uint64 ullEntityHash = oEntity.hashValue();
	auto iter = m_zsetHatred.Find(ullEntityHash);
	if (iter == m_zsetHatred.End())
		return;
	uint64 ullScore = iter->second > uiVal ? iter->second - uiVal : 0;
	m_zsetHatred.UpdateOrInsert({ ullEntityHash , oEntity}, ullScore);
	m_mapUpdateFlag[ullEntityHash] = 1;
	m_uiResetFlag = 0;
}

uint64 CHatredHandleMap::GetHatred(const Entity& oEntity)
{
	uint64 ullEntityHash = oEntity.hashValue();
	auto iter = m_zsetHatred.Find(ullEntityHash);
	if (iter == m_zsetHatred.End())
		return 0;
	return (uint32)iter->second;
}

bool CHatredHandleMap::HasHatred(const Entity& oEntity) 
{
	uint64 ullEntityHash = oEntity.hashValue();
	auto iter = m_zsetHatred.Find(ullEntityHash);
	if (iter == m_zsetHatred.End())
		return false;
	return true;
}
