#include "stdafx.h"
#include "DataSubject.h"

IMPLEMENT_SINGLETON(CDataSubject)

CDataSubject::CDataSubject()
{
}


CDataSubject::~CDataSubject()
{
	Release();
}

list<void*>* CDataSubject::GetDataLst(int iMessage)
{
	auto iter_find = m_DataMap.find(iMessage);

	if (iter_find == m_DataMap.end())
		return nullptr;

	return &iter_find->second;
}

void CDataSubject::AddData(int iMessage, void * pData)
{
	if (pData != nullptr)
	{
		m_DataMap[iMessage].push_back(pData);
		CSubject::Notify(iMessage, pData);
	}
}

void CDataSubject::RemoveData(int iMessage, void * pData)
{
	auto iter_find = m_DataMap.find(iMessage);

	if (iter_find == m_DataMap.end())
		return;

	auto iter_begin = iter_find->second.begin();
	auto iter_end = iter_find->second.end();

	for (; iter_begin != iter_end;)
	{
		if (pData == *iter_begin)
			iter_begin = iter_find->second.erase(iter_begin);
		else
			++iter_begin;
	}
}

void CDataSubject::Release()
{
	for (auto& MyPair : m_DataMap)
		MyPair.second.clear();
	
	m_DataMap.clear();
}