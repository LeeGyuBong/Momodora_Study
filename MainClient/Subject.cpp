#include "stdafx.h"
#include "Subject.h"
#include "Observer.h"

CSubject::CSubject()
{
}


CSubject::~CSubject()
{
	Release();
}

void CSubject::Subscribe(CObserver * pObserver)
{
	m_ObserveLst.push_back(pObserver);
}

void CSubject::UnSubscribe(CObserver * pObserver)
{
	for (auto& iter_begin = m_ObserveLst.begin(); iter_begin != m_ObserveLst.end(); )
	{
		if (pObserver == *iter_begin)
			iter_begin = m_ObserveLst.erase(iter_begin);
		else
			++iter_begin;
	}
}

void CSubject::Notify(int iMessage, void * pData)
{
	for (auto& pObs : m_ObserveLst)
		pObs->Update(iMessage, pData);
}

void CSubject::Release()
{
	m_ObserveLst.clear();
}