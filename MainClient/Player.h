#pragma once
#include "Obj.h"

class CPlayerIMP;
class CPlayer :
	public CObj
{
public:
	CPlayer();
	virtual ~CPlayer();

public:
	// CObj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;

	void RenderRect();

	void RenderAttackRect();

private:
	template<typename T> inline void CreateBridge()
	{
		SafeDelete(m_pBridge);
		m_pBridge = new T;
		m_pBridge->Initialize();
	};
	// ���� ���� ����
	void Control();
	void TalkControl();
	// ���� ���� ����
	void Jump();
	// �߶� ���� ����
	void Falling();
	// Ȱ ���� ���� ����
	void BowCharge();
	// �ǰݽ� ����
	void Hit();
	// ���� �浹 ���� ���� ����
	void SetAttackColliRect();
	// ������ ���
	void ItemUse();

public:
	bool& GetBowCharge() { return m_bIsBowCharge; }
	int& GetChargeBlend() { return m_iChargeBlend; }
	int& GetInviAlpha() { return m_iInviAlpha; }
public:
	// ���Ÿ� ���� ������ �����ٴ� ���� ����
	inline void ChargeAttackDone() { m_bIsBowCharge = false; }
	CPlayerIMP*& GetCurState() { return m_pBridge; }

	vector<ITEM*>& GetActiveItemLst() { return m_vecActiveLst; }
	vector<ITEM*>& GetPassiveItemLst() { return m_vecPassiveLst; }
	ITEM*& GetActiveSlotItem(int index) { return m_tActiveSlot[index]; }
	ITEM*& GetPassiveSlotItem(int index) { return m_tPassiveSlot[index]; }
	ITEM*& GetCurItem() { return m_tCurActive; }

private:
	CPlayerIMP* m_pBridge;

	// ���� ����
	bool m_bIsJumping;
	float m_fPosY;
	float m_fJumpPower;
	float m_fJumpTime;
	bool m_bIsSecondJumping;

	// �̵��� ���� ����
	float m_fWalkTime;

	// ���� ���ݿ� ���� ����
	float m_fAttackTime;

	// ���Ÿ� ���ݿ� ���� ����
	bool m_bIsBowCharge;
	float m_fEffectTime;
	int m_iChargeBlend;
	float m_fBlendTime;
	float m_fChargeTime;

	// �ǰݽ� ���� ����
	float m_fInviTime;
	int m_iInviAlpha;

	bool m_bIsControl;
	int iEvent;

	// ��
	int m_iMoney;

	// ������ ���� ����
	int m_iItemIndex;
	ITEM* m_tCurActive;
	ITEM* m_tActiveSlot[3];
	vector<ITEM*> m_vecActiveLst;

	ITEM* m_tPassiveSlot[2];
	vector<ITEM*> m_vecPassiveLst;

	ITEM* m_tImportSlot[2];
	vector<ITEM*> m_vecImprotLst;

	// ���� ���� ����
	float m_fWalkSoundTime;
	float m_fChargeSoundTime;

	int m_iDeadAlpha;
};

