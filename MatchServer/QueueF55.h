#pragma once

#include "MatchQueue.h"

class QueueMgrF55;
class QueueF55 : public MatchQueue
{
public:
	QueueF55(uint64 key);
	virtual ~QueueF55();
	void final() override;
	bool needRemoveFromActive() override;
public:
	uint32 getIdFirst();
	uint32 getIdSecond();
	QueueMgrF55& mgrF55Ref();
};

//ƥ������˶���
class QueueRobotF55 : public QueueF55
{
public:
	QueueRobotF55(uint32 eIdRobot);
	virtual ~QueueRobotF55();
	void addUnit(MatchUnit* pUnit) override;
	void tick() override;
public:
	uint32 getIdRobot();
};
//ʱ�����
class QueueTimeF55 : public QueueF55
{
public:
	QueueTimeF55(uint32 eIdTime);
	virtual ~QueueTimeF55();
	void addUnit(MatchUnit* pUnit) override;
	void tick() override;
public:
	uint32 getIdTime();
};
//ƥ���λ&����&ְҵ����
class QueueNumCareerF55 : public QueueF55
{
public:
	QueueNumCareerF55(uint32 eIdLvId, uint32 eIdNum, uint32 eIdCareer);
	virtual ~QueueNumCareerF55();
public:
	uint32 getIdLvId();
	uint32 getIdNum();
	uint32 getIdCareer();
};

class QueueFormF55 : public QueueF55
{
public:
	QueueFormF55(uint64 key);
	virtual ~QueueFormF55();
	void addUnit(MatchUnit* pUnit) override;
	bool needRemoveFromActive() override;
	void tick() override;
public:
	uint32 getIdLvId();
	uint32 getIdNum();
	uint32 getIdCarrer();
public:
	std::string firstStr;
	std::string secondStr;
	TTNumCareerSidePairT first;
	TTNumCareerSidePairT second;
};