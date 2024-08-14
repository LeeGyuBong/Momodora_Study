#pragma once
#include "PlayerIMP.h"
class CShootBowIMP :
	public CPlayerIMP
{
public:
	CShootBowIMP();
	virtual ~CShootBowIMP();

public:
	// CPlayerIMP을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Release() override;
};