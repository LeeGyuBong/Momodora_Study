#pragma once
#include "PlayerIMP.h"
class CAttack2IMP :
	public CPlayerIMP
{
public:
	CAttack2IMP();
	virtual ~CAttack2IMP();

	// CPlayerIMP��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Release() override;
};

