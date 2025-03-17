#pragma once

#include "ztype.h"
#include "zMacro.h"

enum {
	_DIR_UP				= 0,	//����
	_DIR_UPRIGHT		= 1,	//����
	_DIR_RIGHTUP		= 1,	//����
	_DIR_RIGHT			= 2,	//����
	_DIR_RIGHTDOWN		= 3,	//����
	_DIR_DOWNRIGHT		= 3,	//����
	_DIR_DOWN			= 4,	//����
	_DIR_DOWNLEFT		= 5,	//����
	_DIR_LEFTDOWN		= 5,	//����
	_DIR_LEFT			= 6,	//����
	_DIR_LEFTUP			= 7,	//����
	_DIR_UPLEFT			= 7,	//����
	_DIR_WRONG			= 8,	//������
};
//һά����
struct zPos1
{
	uint32 x;
	uint32 offsetx;
	//���캯��
	zPos1() : x(0), offsetx(0)
	{
	}
	zPos1(const uint32 x, const uint32 ox = SMALL_POS_OFFSET / 2)
	{
		this->x = x;
		this->offsetx = ox;
	}
	zPos1(const zPos1& p1)
	{
		this->x = p1.x;
		this->offsetx = p1.offsetx;
	}
	//���
	void reset()
	{
		x = 0;
		offsetx = 0;
	}
	//�Ƿ�Ϊ��
	bool empty() const
	{
		return x == 0;
	}
	//ȡ��С����
	uint32 getSmallX() const
	{
		return x * SMALL_POS_OFFSET + offsetx;
	}
	float getFloatX() const
	{
		return (float)getSmallX() / SMALL_POS_OFFSET;
	}
	//ͨ����������������
	void setFloatX(float x)
	{
		if (x <= 0.0f)
		{
			x = 0;
			offsetx = 0;
		}
		else
		{
			this->x = (int32)x;
			this->offsetx = uint32((x - this->x) * SMALL_POS_OFFSET + 0.5);
		}
	}
	//�жϴ������Ƿ����
	bool equal(const zPos1& p1) const
	{
		return x == p1.x;
	}
	//�ж�С�����Ƿ����
	bool smallEqual(const zPos1& p1) const
	{
		return x == p1.x && offsetx == p1.offsetx;
	}
	//��ֵ��������
	zPos1& operator= (const zPos1& p1)
	{
		x = p1.x;
		offsetx = p1.offsetx;
		return *this;
	}
	//==�߼��������
	bool operator== (const zPos1& p1) const
	{
		return equal(p1);
	}
};

//��ά����
struct zPos2 : public zPos1
{
	uint32 y;
	uint32 offsety;
	//���캯��
	zPos2() : y(0), offsety(0)
	{
	}
	zPos2(const uint32 x, const uint32 y) : zPos1(x)
	{
		this->y = y;
		this->offsety = SMALL_POS_OFFSET/2;
	}
	zPos2(const uint32 x, const uint32 y, const uint32 ox, const uint32 oy) : zPos1(x, ox)
	{
		this->y = y;
		this->offsety = oy;
	}
	zPos2(const zPos1 &p1) : zPos1(p1)
	{
		this->y = 0;
		this->offsety = 0;
	}
	zPos2(const zPos2 &p2)
	{
		this->x = p2.x;
		this->y = p2.y;
		this->offsetx = p2.offsetx;
		this->offsety = p2.offsety;
	}
	//���
	void reset()
	{
		zPos1::reset();
		y = 0;
		offsety = 0;
	}
	//�Ƿ�Ϊ��
	bool empty() const
	{
		return zPos1::empty() && y == 0;
	}
	//ȡ��С����
	uint32 getSmallY() const
	{
		return y * SMALL_POS_OFFSET + offsety;
	}
	float getFloatY() const
	{
		return (float)getSmallY() / SMALL_POS_OFFSET;
	}
	//ͨ����������������
	void setFloatY(float y)
	{
		if (y <= 0.0f)
		{
			y = 0;
			offsety = 0;
		}
		else
		{
			this->y = (int32)y;
			this->offsety = uint32((y - this->y) * SMALL_POS_OFFSET + 0.5);
		}
	}
	//�жϴ������Ƿ����
	bool equal(const zPos2& p2) const
	{
		return x == p2.x && y == p2.y;
	}
	//�ж�С�����Ƿ����
	bool smallEqual(const zPos2& p2) const
	{
		return x == p2.x && offsetx == p2.offsetx && y == p2.y && offsety == p2.offsety;
	}
	//��ֵ��������
	zPos2& operator= (const zPos2& p2)
	{
		x = p2.x;
		y = p2.y;
		offsetx = p2.offsetx;
		offsety = p2.offsety;
		return *this;
	}
	//==�߼��������
	bool operator== (const zPos2& p2) const
	{
		return equal(p2);
	}
	//!=�߼��������
	bool operator!= (const zPos2& p2) const
	{
		return !equal(p2);
	}
	//��ȡС�������
	const uint32 getSmallDistance(const zPos2& pos) const
	{
		double dx = abs((int)pos.getSmallX() - (int)getSmallX());
		double dy = abs((int)pos.getSmallY() - (int)getSmallY());
		return (uint32)sqrt(dx * dx + dy * dy);
	}
	//��ȡ���������
	const uint32 getDistance(const zPos2& pos) const
	{
		return (getSmallDistance(pos) + 1) / SMALL_POS_OFFSET;
	}
};

//��ά����
struct zPos3 : public zPos2
{
	int32 z;
	int32 offsetz;

	//���캯��
	zPos3() : z(0), offsetz(0)
	{
	}
	zPos3(const uint32 x, const uint32 y, const uint32 ox=SMALL_POS_OFFSET/2, const uint32 oy = SMALL_POS_OFFSET / 2) : zPos2(x, y, ox, oy)
	{
		this->z = 0;
		this->offsetz = 0;
	}
	zPos3(const uint32 x, const uint32 y, const uint32 z) : zPos2(x, y)
	{
		this->z = z;
		this->offsetz = z < 0 ? (SMALL_POS_OFFSET) * (-1)/2 : SMALL_POS_OFFSET/2;
	}
	zPos3(const uint32 x, const uint32 y, const uint32 z, const uint32 ox, const uint32 oy, const uint32 oz) : zPos2(x, y, ox, oy)
	{
		this->z = z;
		this->offsetz = z;
	}
	zPos3(const zPos1& p1) : zPos2(p1)
	{
		this->z = 0;
		this->offsetz = 0;
	}
	zPos3(const zPos2& p2) : zPos2(p2)
	{
		this->z = 0;
		this->offsetz = 0;
	}
	zPos3(const zPos3& pos)
	{
		x = pos.x;
		y = pos.y;
		z = pos.z;
		offsetx = pos.offsetx;
		offsety = pos.offsety;
		offsetz = pos.offsetz;
	}
	//���
	void reset()
	{
		zPos2::reset();
		z = 0;
		offsetz = 0;
	}
	//�Ƿ�Ϊ��
	bool empty() const
	{
		return zPos2::empty() && z == 0;
	}
	bool empty2d() const
	{
		return zPos2::empty();
	}
	//ȡ��С����
	uint32 getSmallZ() const
	{
		return z * SMALL_POS_OFFSET + offsetz;
	}
	float getFloatZ() const
	{
		return (float)getSmallZ() / SMALL_POS_OFFSET;
	}
	//ͨ����������������
	void setFloatZ(float z)
	{
		this->z = (int32)z;
		this->offsetz = int32((z - this->z) * SMALL_POS_OFFSET + 0.5);
		
	}
	//�жϴ������Ƿ����
	bool equal(const zPos3& pos) const
	{
		return x == pos.x && y == pos.y && z == pos.z;
	}
	//2D�����Ƿ����
	bool equal2d(const zPos3& pos) const
	{
		return zPos2::equal(pos);
	}
	//�ж�С�����Ƿ����
	bool smallEqual(const zPos3& pos) const
	{
		return x == pos.x && offsetx == pos.offsetx && y == pos.y && offsety == pos.offsety && z == pos.z;
	}
	//2DС�����Ƿ����
	bool smallEqual2d(const zPos3& pos) const
	{
		return zPos2::smallEqual(pos);
	}
	//��ֵ��������
	zPos3& operator= (const zPos3& pos)
	{
		if (this == &pos) return *this;
		x = pos.x;
		y = pos.y;
		z = pos.z;
		offsetx = pos.offsetx;
		offsety = pos.offsety;
		offsetz = pos.offsetz;
		return *this;
	}
	//==�߼��������
	bool operator== (const zPos3& pos) const
	{
		return equal(pos);
	}
	//!=�߼��������
	bool operator!= (const zPos3& pos) const
	{
		return !equal(pos);
	}
};
typedef zPos3 zPos;
//����������
using zPosI = uint32;
//�������ŵ�����
using zPosIVector = std::vector<zPosI>;

inline zPosI zPos2zPosI(const zPos2& screenWH, const zPos2& pos)
{
	return ((screenWH.x+MAX_SCREEN_GRID-1)/ MAX_SCREEN_GRID) * (pos.y/ MAX_SCREEN_GRID) + (pos.x/ MAX_SCREEN_GRID);
}
