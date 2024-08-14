#pragma once
#include "UserInterface.h"
class CItemBox :
	public CUserInterface
{
public:
	CItemBox();
	virtual ~CItemBox();

	// CUserInterface을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;

	void RenderMoneyText();

private:
	D3DXVECTOR3 m_vMoneyPos;
	D3DXMATRIX m_matMoney;
	ITEM* m_pItem;
};

