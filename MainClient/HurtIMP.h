#pragma once
#include "PlayerIMP.h"
class CHurtIMP :
	public CPlayerIMP
{
public:
	CHurtIMP();
	virtual ~CHurtIMP();

	// CPlayerIMP��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Release() override;
};

