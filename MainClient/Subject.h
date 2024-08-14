#pragma once

class CObserver;
class CSubject
{
public:
	CSubject();
	virtual ~CSubject();

public:
	virtual void Release();

public:
	void Subscribe(CObserver* pObserver);
	void UnSubscribe(CObserver* pObserver);
	void Notify(int iMessage, void* pData = nullptr);

private:
	list <CObserver*> m_ObserveLst;
};

