#pragma once
#include "PlayerIMP.h"
class CCrouchIMP :
	public CPlayerIMP
{
public:
	CCrouchIMP();
	virtual ~CCrouchIMP();

	// CPlayerIMP��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Release() override;
};

