#include "inner.pb.h"
#include "scenesvr.pb.h"
#include "familysvr.pb.h"

#include "zNullCmd.h"
#include "zLogMgr.h"

#include "parse_pb.h"
#include "server_logic_face.h"

void CSvrLogicFace::handle_logic_msg(const tagNetMsg* pNetMsg)
{
	const tagHostHd& hostHead = pNetMsg->m_hd;
	tagMsgHead* pMsgHead = (tagMsgHead*)(pNetMsg->m_body);
	switch (pMsgHead->usModuleId)
	{
	case inner::enInnerFirst_Scenesvr:		//to scenesvr
		{
			inner::InnerScenesvr innerReq;
			PARSE_PTL_HEAD(innerReq, pMsgHead);
			__msgParseScenesvr(0, hostHead, std::move(innerReq));
		}
		break;
	case inner::enInnerFirst_Family:		//to familysvr
		{
			inner::InnerFamilysvr innerReq;
			PARSE_PTL_HEAD(innerReq, pMsgHead);
			__msgParseFamilysvr(0, hostHead, std::move(innerReq));
		}
		break;			
	default:
		Log_Error("handle_logic_msg.moduleId:%u,cmdId:%u", pMsgHead->usModuleId, pMsgHead->uiCmdId);
		break;
	}
}

bool CSvrLogicFace::__msgParseScenesvr(DWORD hashId, const tagHostHd& hostHead, inner::InnerScenesvr innerReq)
{
	if (innerReq.Fromgate_case() != inner::InnerScenesvr::FromgateCase::FROMGATE_NOT_SET)
		return netMsgFromGate(std::move(hashId), hostHead, std::move(innerReq));
	else if (innerReq.Fromfriend_case() != inner::InnerScenesvr::FromfriendCase::FROMFRIEND_NOT_SET)
		return netMsgFromFriend(std::move(hashId), hostHead, std::move(innerReq));
	Log_Error("%s,%s", innerReq.GetTypeName().c_str(), innerReq.ShortDebugString().c_str());
	return false;
}

bool CSvrLogicFace::__msgParseFamilysvr(DWORD hashId, const tagHostHd& hostHead, inner::InnerFamilysvr innerReq)
{
	if (innerReq.Fromscene_case() != inner::InnerFamilysvr::FromsceneCase::FROMSCENE_NOT_SET)
		return netMsgFromScene(std::move(hashId), hostHead, std::move(innerReq));
	return false;
}

bool CSvrLogicFace::netMsgFromGate(DWORD hashId, const tagHostHd& hostHead, inner::InnerScenesvr innerReq) { return false; }
bool CSvrLogicFace::netMsgFromFriend(DWORD hashId, const tagHostHd& hostHead, inner::InnerScenesvr innerReq) { return false; }
bool CSvrLogicFace::netMsgFromScene(DWORD hashId, const tagHostHd& hostHead, inner::InnerFamilysvr innerReq) { return false; }