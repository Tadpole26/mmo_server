#include "MapConfig.h"

namespace config
{
	bool MapConfig::LoadConfig(const std::string& strContent)
	{
		InitSheet("map_info", std::bind(&MapConfig::AddMapInfo, this));

		return LoadCfgFromStr(strContent);
	}

	bool MapConfig::LoadConfigByFile(const std::string& fileName)
	{
		InitSheet("map_info", std::bind(&MapConfig::AddMapInfo, this));

		return LoadCfgFromXml(fileName.c_str());
	}

	bool MapConfig::AddMapInfo()
	{
		auto* pMapCfg = new map_info_t;
		assert(pMapCfg != nullptr);
		pMapCfg->tid = GetDataUInt("id");
		pMapCfg->strMapName = GetDataChr("comment");							//��ͼ��
		pMapCfg->uiMapLevel = GetDataUInt("level");							//��ͼҪ��ȼ�
		pMapCfg->bStatic = GetDataBool("static");								//�Ƿ��Ǿ�̬��ͼ
		pMapCfg->uiWidth = GetDataUInt("width");								//��ͼ���X(8�ı���)
		pMapCfg->uiHeight = GetDataUInt("height");							//��ͼ�߶�X(8�ı���)
		GetDataPos("born_point", pMapCfg->oBornPoint);							//��������
		pMapCfg->uiBornDirect = GetDataUInt("direct");						//��������
		GetDataPos("reborn_point", pMapCfg->oRebornPoint);
		pMapCfg->uiRebornDirect = GetDataUInt("reborn_direct");						//��������
		pMapCfg->navmesh = GetDataUInt("height");							//Ѱ·�ļ�
		_allMapCfg[pMapCfg->tid] = pMapCfg;
		return true;
	}

	const map_info_t* MapConfig::getMapConfig(const uint32 tid) const
	{
		auto iter = _allMapCfg.find(tid);
		if (iter == _allMapCfg.end())
			return nullptr;
		return iter->second;
	}

	bool MapConfig::foreach(std::function<bool(map_info_t* pConfig)> func) const
	{
		for (auto& pair : _allMapCfg)
		{
			auto* pMapCfg = pair.second;
			if (!func(pMapCfg)) return false;
		}
		return true;
	}
}
