#pragma once
#include "PlayerIMP.h"
class CAttack2IMP :
	public CPlayerIMP
{
public:
	CAttack2IMP();
	virtual ~CAttack2IMP();

	// CPlayerIMP을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Release() override;
};

