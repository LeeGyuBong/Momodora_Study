#pragma once
#include "UserInterface.h"
class CPlayerHp :
	public CUserInterface
{
public:
	CPlayerHp();
	virtual ~CPlayerHp();

public:
	// CUserInterface을(를) 통해 상속됨
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

	int m_iMaxHp;
	int m_iHp;

	int m_iPreHp;

	int m_iDamage;
	float m_fDamageTime;
};

