#pragma once
#include "PlayerIMP.h"
class CUseItemIMP :
	public CPlayerIMP
{
public:
	CUseItemIMP();
	virtual ~CUseItemIMP();

	// CPlayerIMP��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Release() override;

private:
	bool m_bIsEffect;
};

