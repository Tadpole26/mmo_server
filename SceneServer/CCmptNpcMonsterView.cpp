#include "zType.h"
#include "CCmptNpcMonsterView.h"
#include "CNpcMonster.h"

CCmptNpcMonsterView::CCmptNpcMonsterView(CNpcMonster& oMonster) :
	CCmptNpcView(oMonster)
{
}

CCmptNpcMonsterView::~CCmptNpcMonsterView()
{
}

bool CCmptNpcMonsterView::Init()
{
	if (!CCmptNpcView::Init())
		return false;
	return true;
}

void CCmptNpcMonsterView::Final()
{
	CCmptNpcView::Final();
}