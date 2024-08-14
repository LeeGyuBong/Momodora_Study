#pragma once
#include "Observer.h"
class CPlayerInfo :
	public CObserver
{
public:
	CPlayerInfo();
	virtual ~CPlayerInfo();

public:
	// CObserver을(를) 통해 상속됨
	virtual void Update(int iMessage, void * pData) override;

public:
	const INFO& GetInfo() const { return m_tInfo;}
	const STATUS& GetStatus() const { return m_tStatus; }
	const int& GetMoney() const { return m_iMoney; }

private:
	INFO m_tInfo;
	STATUS m_tStatus;
	int m_iMoney;

};

