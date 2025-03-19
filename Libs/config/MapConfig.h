#pragma once

#include "cLoadBase.h"
#include "singleton.h"

namespace config
{
	struct map_info_t
	{
		uint32 uiUid = 0;
		std::string strMapName;							//��ͼ��
		uint32 uiMapLevel = 0;							//��ͼҪ��ȼ�
		bool bStatic = true;							//�Ƿ��Ǿ�̬��ͼ
		uint32 uiTiny = 0;								//�Ƿ���΢�͵�ͼ
		uint32 uiWidth = 0;								//��ͼ���X(8�ı���)
		uint32 uiHeight = 0;							//��ͼ�߶�X(8�ı���)
		zPos oBornPoint;								//��������
		uint32 uiBornDirect = 0;						//��������
		zPos oRebornPoint;								//��������
		uint32 uiRebornDirect = 0;						//��������
		uint32 uiNavmesh = 0;							//Ѱ·�ļ�
		uint32 maxLine = 64;							//�õ�ͼ�������������
	};


	class MapConfig : public cLoadBase
	{
	public:
		bool LoadConfig(const std::string& strContent) override;

		map_info_t* GetMapConfig(const uint32 tid);
	private:
		bool AddMapInfo();

	private:
		std::map<uint32_t, map_info_t> _mapInfoCfg;
	};
}
#define gMapCfg Singleton<config::MapConfig>::getInstance()
