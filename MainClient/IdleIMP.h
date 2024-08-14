#pragma once
#include "PlayerIMP.h"
class CIdleIMP :
	public CPlayerIMP
{
public:
	CIdleIMP();
	virtual ~CIdleIMP();

public:
	// CPlayerIMP��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Release() override;

private:
	int m_iRandomIdle;
	float m_fTime;
};

