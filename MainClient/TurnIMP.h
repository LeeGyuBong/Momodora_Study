#pragma once
#include "PlayerIMP.h"
class CTurnIMP :
	public CPlayerIMP
{
public:
	CTurnIMP();
	virtual ~CTurnIMP();

public:
	// CPlayerIMP��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Release() override;
};

