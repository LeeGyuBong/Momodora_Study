#pragma once
#include "PlayerIMP.h"
class CAttack1IMP :
	public CPlayerIMP
{
public:
	CAttack1IMP();
	virtual ~CAttack1IMP();

	// CPlayerIMP을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Release() override;
};

