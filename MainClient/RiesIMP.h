#pragma once
#include "PlayerIMP.h"
class CRiesIMP :
	public CPlayerIMP
{
public:
	CRiesIMP();
	virtual ~CRiesIMP();

	// CPlayerIMP��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Release() override;
};

