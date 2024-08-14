#pragma once
#include "Observer.h"
class CBossInfo :
	public CObserver
{
public:
	CBossInfo();
	virtual ~CBossInfo();

	// CObserver��(��) ���� ��ӵ�
	virtual void Update(int iMessage, void * pData) override;

public:
	const STATUS& GetStatus() const { return m_tStatus; }

private:
	STATUS m_tStatus;
};

