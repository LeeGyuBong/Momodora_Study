#pragma once
#include "PlayerIMP.h"
class CUseItemIMP :
	public CPlayerIMP
{
public:
	CUseItemIMP();
	virtual ~CUseItemIMP();

	// CPlayerIMP을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Release() override;

private:
	bool m_bIsEffect;
};

