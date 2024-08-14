#pragma once
#include "UserInterface.h"
class CBossHp :
	public CUserInterface
{
public:
	CBossHp();
	virtual ~CBossHp();

	// CUserInterface��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;

	void RenderSurface();
	void RenderInside();

private:
	wstring m_wstrStateKey;

	int m_iHp;
	int m_iPreHp;

	int m_iDamage;
	float m_fDamageTime;

	float m_fTime;
};

