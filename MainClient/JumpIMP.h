#pragma once
#include "PlayerIMP.h"

class CJumpIMP :
	public CPlayerIMP
{
public:
	CJumpIMP();
	virtual ~CJumpIMP();

public:
	// CPlayerIMP��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Release() override;
};

