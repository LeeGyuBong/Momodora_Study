#include "stdafx.h"
#include "BossInfo.h"


CBossInfo::CBossInfo()
{
	ZeroMemory(&m_tStatus, sizeof(STATUS));
}


CBossInfo::~CBossInfo()
{
}

void CBossInfo::Update(int iMessage, void * pData)
{
	list<void*>* pDataLst = CDataSubject::GetInstance()->GetDataLst(iMessage);

	if (pDataLst != nullptr)
	{
		auto iter_find = find(pDataLst->begin(), pDataLst->end(), pData);

		if (iter_find == pDataLst->end())
			return;

		switch (iMessage)
		{
		case OBSERVER::BOSS_STATUS:
			m_tStatus = *reinterpret_cast<STATUS*>(*iter_find);
			break;
		}
	}
}
