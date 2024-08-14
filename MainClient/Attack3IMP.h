#pragma once
#include "PlayerIMP.h"
class CAttack3IMP :
	public CPlayerIMP
{
public:
	CAttack3IMP();
	virtual ~CAttack3IMP();

	// CPlayerIMP을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Release() override;
};

