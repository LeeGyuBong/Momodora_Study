#pragma once
#include "PlayerIMP.h"
class CFallIMP :
	public CPlayerIMP
{
public:
	CFallIMP();
	virtual ~CFallIMP();

	// CPlayerIMP��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Release() override;
};

