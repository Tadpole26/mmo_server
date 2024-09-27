#pragma once
#include "dtype.h"
#include "log_mgr.h"
#include "lru_hashmap.h"
#include "singleton.h"
#include "result_code.pb.h"
#include "record_associate_container.h"
#include "CUserInfoRecord.h"
#include "CUserCardRecord.h"
#include "CUserItemRecord.h"
#include "CUserMailRecord.h"



class CUserInfo
{
public:
    CUserInfo() : m_llUid(0), m_iPlatId(0), m_iGroupId(0), m_bOnline(false), m_iSequence(0)
    {
    }

    bool SendToClient(uint32_t uiRspModule, uint32_t uiRspCmd, google::protobuf::Message* pRspMsg, ProtoMsg::ResultCode eCode, bool bNotify);
public:
    using card_container_type = RecordAssociateContainer<int32_t, user_card_table_t*, user_card_table_value_type>;
    using item_container_type = RecordAssociateContainer<int32_t, user_item_table_t*, user_item_table_value_type>;
    using mail_container_type = RecordAssociateContainer<int64_t, user_mail_table_t*, user_mail_table_value_type>;

    int64_t                                         m_llUid;
    int32_t											m_iPlatId;
    int32_t                                         m_iGroupId;
    int32_t                                         m_iSequence;                    //�ͻ�������Ψһid
    bool                                            m_bOnline;
    user_info_table_t                               m_stUserBaseInfo;               //��һ�������
    card_container_type                             m_mapUserCard;                  //���Ӣ�۽�ɫ����          
    item_container_type                             m_mapUserItem;                  //��ҵ�������
    mail_container_type                             m_mapUserMail;                  //����ʼ�����

};

using user_info_ptr_type = CUserInfo*;
//��lur_hashmap�߳���һص�
struct LRUCallBackFunctor {
    void operator() (const int64_t& ulKey, const CUserInfo* pUserInfo)
    {
        delete pUserInfo;

        Log_Warning("user cache info lru hash map kick off|uin:%lld", ulKey);
    }
};

using user_cache_info_hashmap_type = LogicLRUHashmap<int64_t, CUserInfo*>;
//���嵥����creator��destroy����
template <class T>
class CUserInfoLRUHashmapCreator
{
public:
    static user_cache_info_hashmap_type* create()
    {
        user_cache_info_hashmap_type* pCacheUserInfo = new user_cache_info_hashmap_type;
        assert(pCacheUserInfo != NULL);
        std::function<void(const int64_t&, const user_info_ptr_type&)> stLRUCallBackFunctor = LRUCallBackFunctor();
        pCacheUserInfo->SetKickOutNotifier(stLRUCallBackFunctor);

        return pCacheUserInfo;
    }

    static void destroy(user_cache_info_hashmap_type* pCacheUserInfo)
    {
        if (pCacheUserInfo != NULL)
        {
            for (auto stIter = pCacheUserInfo->Begin(); stIter != pCacheUserInfo->End(); stIter++)
            {
                delete stIter->second;
            }

            delete pCacheUserInfo;
        }
    }
};

#define g_UserInfoLRUHashmap Singleton<user_cache_info_hashmap_type, CUserInfoLRUHashmapCreator>::getInstance()

