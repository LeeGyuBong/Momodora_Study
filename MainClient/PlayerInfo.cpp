#include "stdafx.h"
#include "PlayerInfo.h"


CPlayerInfo::CPlayerInfo()
{
}


CPlayerInfo::~CPlayerInfo()
{
}

void CPlayerInfo::Update(int iMessage, void * pData)
{
	list<void*>* pDataLst = CDataSubject::GetInstance()->GetDataLst(iMessage);

	if (pDataLst != nullptr)
	{
		auto iter_find = find(pDataLst->begin(), pDataLst->end(), pData);
		
		if (iter_find == pDataLst->end())
			return;

		switch (iMessage)
		{
		case OBSERVER::PLAYER_INFO:
			m_tInfo = *reinterpret_cast<INFO*>(*iter_find);
			break;
		case OBSERVER::PLAYER_STATUS:
			m_tStatus = *reinterpret_cast<STATUS*>(*iter_find);
			break;
		case OBSERVER::PLAYER_MONEY:
			m_iMoney = *reinterpret_cast<int*>(*iter_find);
			break;
		}
	}
}
