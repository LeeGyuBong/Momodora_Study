#pragma once
#include "PlayerIMP.h"
class CCrouchBowIMP :
	public CPlayerIMP
{
public:
	CCrouchBowIMP();
	virtual ~CCrouchBowIMP();

	// CPlayerIMP��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Release() override;
};

