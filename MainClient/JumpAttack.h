#pragma once
#include "PlayerIMP.h"
class CJumpAttack :
	public CPlayerIMP
{
public:
	CJumpAttack();
	virtual ~CJumpAttack();

	// CPlayerIMP��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Release() override;
};

