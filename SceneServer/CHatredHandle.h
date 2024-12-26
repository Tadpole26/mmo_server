#pragma once
#include "sdk.h"
#include "rank_map.h"

//动态npc仇恨列表
class CHatredHandle
{
public:
	struct Entity
	{
		uint32 uiType = 0;
		uint64 llUid = 0;
	
	//这里要把类型和uid哈希,因为可能出现monster和role相同的uid情况
	uint64 hashValue() const { return zMisc::hash(uiType, llUid); }
	bool operator==(const Entity& other) const { return equal(other); }
	bool empty() const { return llUid == 0; }
	void clear()
	{
		uiType = 0;
		llUid = 0;
	}
	bool equal(const Entity& other) const
	{
		if (this == &other) return true;
		return uiType == other.uiType && llUid == other.llUid;
	}
	};
public:
	static constexpr uint32 GetHatredLimit() { return (1u << 31) - 1; }
public:
	CHatredHandle() {}
	virtual ~CHatredHandle() {}
	virtual void Init() = 0;
	virtual void Final() = 0;
	virtual bool Empty() const = 0;
	virtual void Clear() = 0;
	virtual bool ForeachByOrder(std::function<bool(uint32, uint64, const Entity&)> func) = 0;
	virtual Entity SelectHatred(std::function<bool(const Entity&)> func) = 0;
	virtual void ClearHatred(const Entity& oEntity) = 0;
	virtual void AddHatred(const Entity& oEntity, uint32 uiVal) = 0;
	virtual void SubHatred(const Entity& oEntity, uint32 uiVal) = 0;
	virtual uint64 GetHatred(const Entity& oEntity) = 0;
	virtual bool HasHatred(const Entity& oEntity) = 0;
	virtual Entity GetLastHatred() const = 0;
	virtual void SetLastHatred(const Entity& oEntity) = 0;
	virtual bool CheckUpdateFlag() = 0;
	virtual bool FetchUpdateFlag(std::function<bool(const Entity&, uint32)> func) = 0;
};

//小怪仇恨列表
class CHatredHandleArr : public CHatredHandle
{
public:
	struct HatredEntity
	{
		Entity entity;
		uint32 index = 0;
		uint64 value = 0;
		uint64 updateTime = 0;

		void clear()
		{
			entity.clear();
			value = 0;
			updateTime = 0;
		}
	};
public:
	static constexpr uint32 kMaxHatredNum = 5;
	static constexpr uint32 kLoseHatredInterval = 10;
	static constexpr uint32 kInvalidHatredIndex = std::numeric_limits<uint32>::max();
public:
	CHatredHandleArr();
	~CHatredHandleArr();
	void Init() override;
	void Final() override;
	bool Empty() const override;
	void Clear() override;
	bool ForeachByOrder(std::function<bool(uint32, uint64, const Entity&)> func) override;
	Entity SelectHatred(std::function<bool(const Entity&)> func) override;
	void ClearHatred(const Entity& oEntity) override;
	void AddHatred(const Entity& oEntity, uint32 uiVal) override;
	void SubHatred(const Entity& oEntity, uint32 uiVal) override;
	uint64 GetHatred(const Entity& oEntity) override;
	bool HasHatred(const Entity& oEntity) override;
	Entity GetLastHatred() const override;
	void SetLastHatred(const Entity& oEntity) { m_oLastHatred = oEntity; }
	bool CheckUpdateFlag() override { return false; }
	bool FetchUpdateFlag(std::function<bool(const Entity&, uint32)> func) override { return true; }
private:
	std::array<HatredEntity, kMaxHatredNum> m_arrHatred;
	Entity m_oLastHatred;
};

//Boss仇恨列表
class CHatredHandleMap : public CHatredHandle
{
public:
	CHatredHandleMap();
	~CHatredHandleMap();
	void Init() override;
	void Final() override;
	bool Empty() const override;
	void Clear() override;
	bool ForeachByOrder(std::function<bool(uint32, uint64, const Entity&)> func) override;
	Entity SelectHatred(std::function<bool(const Entity&)> func) override;
	void ClearHatred(const Entity& oEntity) override;
	void AddHatred(const Entity& oEntity, uint32 uiVal) override;
	void SubHatred(const Entity& oEntity, uint32 uiVal) override;
	uint64 GetHatred(const Entity& oEntity) override;
	bool HasHatred(const Entity& oEntity) override;
	Entity GetLastHatred() const override { return m_oLastHatred; }
	void SetLastHatred(const Entity& oEntity) { m_oLastHatred = oEntity; }
	bool CheckUpdateFlag() override { return false; }
	bool FetchUpdateFlag(std::function<bool(const Entity&, uint32)> func) override { return false; }
private:
	RankMap<uint64, Entity> m_zsetHatred;									//boss仇恨列表数量过多,用跳表
	Entity m_oLastHatred;
	std::unordered_map<uint64, uint32> m_mapUpdateFlag;
	uint32 m_uiResetFlag = 0;
	uint64 m_ullUpdateTime = 0;
};

