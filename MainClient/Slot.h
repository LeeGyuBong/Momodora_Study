#pragma once
#include "UserInterface.h"
class CSlot :
	public CUserInterface
{
public:
	CSlot();
	virtual ~CSlot();

	// CUserInterface을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;

public:
	void SetItem(ITEM* item) { m_pItem = item; }
	void SetCurSlot(bool bIs) { m_bIsCur = bIs; }

	ITEM*& GetSlotItem() { return m_pItem; }

private:
	ITEM* m_pItem;

	int m_iAlpha;
	bool m_bIsCur;
	int m_iCurAlpha;
	int m_iCalcul;
	float m_fAlphaTime;
};

