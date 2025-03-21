#pragma once
#include "zType.h"
#include "sdk.h"

using zRoleIdType = uint64;
using zAccIdType = uint64;

template<typename T>
class zRoleMgr
{
private:
	using zRoleListT = std::list<T*>;
	using zRoleMapT = std::unordered_map<zRoleIdType, T*>;
	using zAccMapT = std::unordered_map<zAccIdType, zRoleListT>;
	zRoleMapT _rolemap;
	zAccMapT _accmap;
public:
	zRoleMgr() {}
	~zRoleMgr() {}
public:
	bool addRole(T* pRole)
	{
		auto iter = _rolemap.find(pRole->getRoleId());
		if (iter != _rolemap.end())
		{
			return false;
		}
		_rolemap[pRole->getRoleId()] = pRole;
		_accmap[pRole->getAccId()].push_back(pRole);
		return true;
	}
	T* getRole(const zRoleIdType roleId)
	{
		T* pRole = nullptr;
		auto iter = _rolemap.find(roleId);
		if (iter != _rolemap.end())
		{
			pRole = iter->second;
		}
		return pRole;
	}
	void removeRole(T* pRole)
	{
		auto iter = _rolemap.find(pRole->getRoleId());
		if (iter != _rolemap.end())
		{
			_rolemap.erase(iter);
		}
		auto itf = _accmap.find(pRole->getAccId());
		if (itf != _accmap.end())
		{
			zMisc::listErase(itf->second, pRole);
			if (itf->second.empty())
				_accmap.erase(itf);
		}
	}
	bool execEveryRole(std::function<bool(T*)> func)
	{
		for (auto& pair : _rolemap)
		{
			if (!func(pair.second))
				return false;
		}
		return true;
	}
	bool getByAccId(zAccIdType accId, std::function<bool(T*)> func)
	{
		auto iter = _accmap.find(accId);
		if (iter == _accmap.end())
		{
			for (auto* pRole : iter->second)
			{
				if (!func(pRole)) break;
			}
		}
		return true;
	}
	uint32 sizeRole() { return _rolemap.size(); }
	bool emptyRole() { return _rolemap.empty(); }
};
