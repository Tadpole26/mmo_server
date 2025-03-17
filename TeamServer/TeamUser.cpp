#include "TeamUser.h"
#include "common.pb.h"
#include "fight3v3common.pb.h"
#include "fight5v5common.pb.h"

void TeamUser::setSceneHashId(uint32 hashId)
{
	if (_sceneHashId != hashId)
	{
		_sceneHashId = hashId;
		Log_Debug("setSceneHashId.%lu.%lu", _sceneHashId, hashId);
	}
}

void TeamUser::fill(common::RoleBrief& out)
{
	out = *_brief;
}

void TeamUser::fill(common::RoleExtend1& out)
{
	out = *_extd1;
}

void TeamUser::fill(common::RoleExtend2& out)
{
	out = *_extd2;
}

void TeamUser::fill(common::RoleExtend3& out)
{
	out = *_extd3;
}

void TeamUser::fill(common::ExtendFight3v3& out)
{
	out = *_f33;
}

void TeamUser::fill(common::ExtendFight5v5& out)
{
	out = *_f55;
}

void TeamUser::setBrief(const common::RoleBrief& in)
{
	*_brief = in;
	setNickName(_brief->nickname());
}

void TeamUser::setExtd1(const common::RoleExtend1& in)
{
	*_extd1 = in;
}

void TeamUser::setExtd2(const common::RoleExtend2& in)
{
	*_extd2 = in;
}

void TeamUser::setExtd3(const common::RoleExtend3& in)
{
	*_extd3 = in;
}

void TeamUser::setF33(const common::ExtendFight3v3& in)
{
	*_f33 = in;
}

void TeamUser::setF55(const common::ExtendFight5v5& in)
{
	*_f55 = in;
}