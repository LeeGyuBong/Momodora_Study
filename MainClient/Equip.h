#pragma once
#include "UserInterface.h"
class CEquip :
	public CUserInterface
{
public:
	enum TYPE { SLOT, INVEN, END };
	enum ITEMTYPE { ACTIVE, PASSIVE };

public:
	CEquip();
	virtual ~CEquip();

	// CUserInterface을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;

	void RenderBack();
	void RenderBox();
	void InsertSlot();
	void RenderText();
	void RenderItemList();

private:
	void Control();

private:
	TYPE m_eType;
	ITEMTYPE m_eItemType;
	wstring m_wstrStateKey;

	CObj* pObj;

	int m_iAlpha;

	float m_fAlphaTime;
	int m_iInvenAlpha;
	int m_iCalcul;

	vector<CObj*> m_vecSlot;
	int m_iCurIndex;
	int m_iInvenIndex;

	vector<ITEM*> m_CurLst;
};

