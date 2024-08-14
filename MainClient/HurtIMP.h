#pragma once
#include "PlayerIMP.h"
class CHurtIMP :
	public CPlayerIMP
{
public:
	CHurtIMP();
	virtual ~CHurtIMP();

	// CPlayerIMP을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Release() override;
};

