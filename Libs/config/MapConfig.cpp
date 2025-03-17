#include "MapConfig.h"

namespace config
{
	bool MapConfig::LoadConfig(const std::string& strContent)
	{
		InitSheet("map_info", std::bind(&MapConfig::AddMapInfo, this));

		return LoadCfgFromStr(strContent);

	}

	bool MapConfig::AddMapInfo()
	{
		uint32 uiUid = GetDataUInt("id");
		map_info_t& stElem = _MapInfoCfg[uiUid];
		stElem.uiUid = GetDataUInt("id");
		stElem.strMapName = GetDataChr("comment");							//��ͼ��
		stElem.uiMapLevel = GetDataUInt("level");							//��ͼҪ��ȼ�
		stElem.bStatic = GetDataBool("static");								//�Ƿ��Ǿ�̬��ͼ
		stElem.uiWidth = GetDataUInt("width");								//��ͼ���X(8�ı���)
		stElem.uiHeight = GetDataUInt("height");							//��ͼ�߶�X(8�ı���)
		GetDataPos("born_point", stElem.oBornPoint);							//��������
		stElem.uiBornDirect = GetDataUInt("direct");						//��������
		GetDataPos("reborn_point", stElem.oRebornPoint);
		stElem.uiRebornDirect = GetDataUInt("reborn_direct");						//��������
		stElem.uiNavmesh = GetDataUInt("height");							//Ѱ·�ļ�
		return true;
	}

	map_info_t* MapConfig::GetMapConfig(const uint32 tid)
	{
		auto iter = _MapInfoCfg.find(tid);
		if (iter == _MapInfoCfg.end())
			return nullptr;
		return &(iter->second);
	}
}
