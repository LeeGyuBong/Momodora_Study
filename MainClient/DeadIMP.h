#pragma once
#include "PlayerIMP.h"
class CDeadIMP :
	public CPlayerIMP
{
public:
	CDeadIMP();
	virtual ~CDeadIMP();

	// CPlayerIMP��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Release() override;

private:
	bool m_bIsSound;
};

