#pragma once
#include "PlayerIMP.h"
class CMoveIMP :
	public CPlayerIMP
{
public:
	CMoveIMP();
	virtual ~CMoveIMP();

public:
	// CPlayerIMP��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Release() override;
};

