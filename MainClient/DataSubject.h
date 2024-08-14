#pragma once
#include "Subject.h"
class CDataSubject :
	public CSubject
{
	DECLARE_SINGLETON(CDataSubject)

private:
	CDataSubject();
	virtual ~CDataSubject();

public:
	virtual void Release();

public:
	void AddData(int iMessage, void* pData);
	void RemoveData(int iMessage, void* pData);

public:
	list<void*>* GetDataLst(int iMessage);

private:
	map<int, list<void*>> m_DataMap;
};

