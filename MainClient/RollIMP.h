#pragma once
#include "PlayerIMP.h"
class CRollIMP :
	public CPlayerIMP
{
public:
	CRollIMP();
	virtual ~CRollIMP();

public:
	// CPlayerIMP��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Release() override;
};

