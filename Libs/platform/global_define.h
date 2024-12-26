#pragma once
#include "ztype.h"

#define ACCEPT_BUF_SIZE		131072							//128*1024
#define SERVER_BUF_SIZE		1048576							//1024*1024e

#define SERVER_INIT_NUM		10
#define PLAYER_INIT_MAX		100

#define SERVER_CON_SEC		1

#define INDEX_MOD_NUM 1000000									//�������ֱ�ǼӺ�׺��λ
#define MAKE_MOD_ID(id, group) ((id) * INDEX_MOD_NUM + (group)) //���ڷ�����ǼӺ�׺��λ
#define SPLIT_MOD_ID(id, group) (((id) - (group)) / INDEX_MOD_NUM)
#define SPLIT_MOD_GAME(id) ((id) % (INDEX_MOD_NUM / 10))
#define SPLIT_MOD_PLAT(id) ((((id) % INDEX_MOD_NUM) * 10) / INDEX_MOD_NUM)
#define SPLIT_MOD_ACCID(id) ((id) / INDEX_MOD_NUM)

#define DB_THREAD_NUM 1	//��Ϸ���������ݿ⴦���߳���


#define CONST_CONFIG		"../Config/const_config.xml"		//ȫ����������(�����������ݿ⡢zk��������Ϣ)

#define MAX_PALYER_NAME_LEN			21	//����ɫ���Ƴ�
#define MAX_CHANNEL_LEN				13	//��������ֳ�

#define GLOBAL_MAIL_LIST	"g_mail_list"							//ȫ���ʼ���,����game��ȡ

#define MAX_CON_SERVER 50

#define MAX_UDP_BUFF 1200
#define NET_FRAME_TIME  50						//����֡ʱ��
#define MAX_HEART_ROOM 50						//��󷿼���
#define WAIT_DELETE_MILLSEC 2000
#define MAX_WAIT_CREATEROOM 30000				//30��

#define ROOM_FILTER_NUM 10

#ifndef WIN32
#define SP_THREAD_CALL
#else 
#define SP_THREAD_CALL __stdcall
#endif


enum SERVER_KIND
{
	SERVER_KIND_NONE = 0,
	SERVER_KIND_GATE = 1,					//���ط�
	SERVER_KIND_CROSS = 2,					//���
	SERVER_KIND_GAME = 3,					//��Ϸ��
	SERVER_KIND_SYS = 4,					//��¼��ϵͳ��
	SERVER_KIND_LOG = 5,					//��־��
	SERVER_KIND_BATTLE = 6,					//ս����
	SERVER_KIND_FBATTLE = 7,				//ս�����ķ�
};

#define DAY_SECONDS 86400
#define WEEK_SECONDS 604800
#define MONTH_SECONDS 2592000