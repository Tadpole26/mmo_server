#pragma once
#include "util_load_base.h"
#include "msg_common.pb.h"
#include "singleton.h"


struct map_info_t
{
	uint32 uiUid = 0;
	std::string strMapName;							//��ͼ��
	uint32 uiMapLevel = 0;							//��ͼҪ��ȼ�
	bool bStatic = true;							//�Ƿ��Ǿ�̬��ͼ
	uint32 uiWidth = 0;								//��ͼ���X(8�ı���)
	uint32 uiHeight = 0;							//��ͼ�߶�X(8�ı���)
	zPos oBornPoint;								//��������
	uint32 uiBornDirect = 0;						//��������
	zPos oRebornPoint;								//��������
	uint32 uiRebornDirect = 0;						//��������
	uint32 uiNavmesh = 0;							//Ѱ·�ļ�
};


class CMapConfig : public CUtilLoadBase
{
public:
	bool LoadConfig(const std::string& strContent) override;
private:
	bool AddMapInfo();

private:
	std::map<uint32_t, map_info_t> m_mapMapInfoCfg;
};

#define gMapCfg Singleton<CMapConfig>::getInstance()
