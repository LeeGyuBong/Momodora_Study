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
	// CObj을(를) 통해 상속됨
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
	// 조작 관련 내용
	void Control();
	void TalkControl();
	// 점프 관련 내용
	void Jump();
	// 추락 관련 내용
	void Falling();
	// 활 차지 공격 내용
	void BowCharge();
	// 피격시 내용
	void Hit();
	// 공격 충돌 범위 설정 내용
	void SetAttackColliRect();
	// 아이템 사용
	void ItemUse();

public:
	bool& GetBowCharge() { return m_bIsBowCharge; }
	int& GetChargeBlend() { return m_iChargeBlend; }
	int& GetInviAlpha() { return m_iInviAlpha; }
public:
	// 원거리 차지 어택이 끝났다는 것을 설정
	inline void ChargeAttackDone() { m_bIsBowCharge = false; }
	CPlayerIMP*& GetCurState() { return m_pBridge; }

	vector<ITEM*>& GetActiveItemLst() { return m_vecActiveLst; }
	vector<ITEM*>& GetPassiveItemLst() { return m_vecPassiveLst; }
	ITEM*& GetActiveSlotItem(int index) { return m_tActiveSlot[index]; }
	ITEM*& GetPassiveSlotItem(int index) { return m_tPassiveSlot[index]; }
	ITEM*& GetCurItem() { return m_tCurActive; }

private:
	CPlayerIMP* m_pBridge;

	// 점프 변수
	bool m_bIsJumping;
	float m_fPosY;
	float m_fJumpPower;
	float m_fJumpTime;
	bool m_bIsSecondJumping;

	// 이동에 관한 변수
	float m_fWalkTime;

	// 근접 공격에 관한 변수
	float m_fAttackTime;

	// 원거리 공격에 관한 변수
	bool m_bIsBowCharge;
	float m_fEffectTime;
	int m_iChargeBlend;
	float m_fBlendTime;
	float m_fChargeTime;

	// 피격시 관련 변수
	float m_fInviTime;
	int m_iInviAlpha;

	bool m_bIsControl;
	int iEvent;

	// 돈
	int m_iMoney;

	// 아이템 관련 변수
	int m_iItemIndex;
	ITEM* m_tCurActive;
	ITEM* m_tActiveSlot[3];
	vector<ITEM*> m_vecActiveLst;

	ITEM* m_tPassiveSlot[2];
	vector<ITEM*> m_vecPassiveLst;

	ITEM* m_tImportSlot[2];
	vector<ITEM*> m_vecImprotLst;

	// 사운드 관련 변수
	float m_fWalkSoundTime;
	float m_fChargeSoundTime;

	int m_iDeadAlpha;
};

