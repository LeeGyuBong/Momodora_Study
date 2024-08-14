#pragma once

class CObj
{
public:
	CObj();
	virtual ~CObj();

public:
	virtual void Initialize() PURE;
	virtual void LateInit();
	virtual int Update() PURE;
	virtual void LateUpdate() PURE;
	virtual void Render() PURE;
	virtual void Release() PURE;

public:
	void UpdateRect();
	void UpdateViewRect(float fSize = 0.f);
	void UpdateAttackRect();
	inline void IsHit(OBJECT::TYPE eType, int damage = 0) 
	{ 
		m_bIsHit = true; 
		m_eColliObjType = eType;
		m_iDamage = damage;
	}
	inline void NotHit()
	{
		m_bIsHit = false;
		m_eColliObjType = OBJECT::END;
	}

	void AttackColliClear()
	{ 
		m_vAttackRect.clear();
		m_vAttackRect.shrink_to_fit();
		m_vAttackModify.clear();
		m_vAttackModify.shrink_to_fit();
		m_vAttackSize.clear();
		m_vAttackSize.shrink_to_fit();
	}

public:
	// VECTOR ���� ��ȯ
	INFO& GetInfo() { return m_tInfo; }
	// �������ͽ� ���� ��ȯ
	const STATUS& GetStatus() const { return m_tStatus; }
	// ���� ��ȯ
	const float& GetDegree() const { return m_fDegree; }
	// �ڱ� �ڽ��� RECT ��ȯ
	const RECT& GetRect() const { return m_tRect; }
	// �þ� RECT ��ȯ
	const RECT& GetViewRect() const { return m_tViewRect; }
	// ���� RECT ��ȯ
	const vector<RECT>& GetAttackRect() const { return m_vAttackRect; }
	// ������Ʈ Ű ��ȯ
	const wstring& GetObjKey() { return m_wstrObjKey; }
	// �ٴ� �浹 ���� ��ȯ
	const bool& GetFloorColli() const { return m_bIsFloorColli; }
	// �ӵ� ��ȯ
	const float& GetSpeed() const { return m_fSpeed; }
	// �߷� ���ӵ� ��ȯ
	const float& GetFallSpeed() const { return m_fFallSpeed; }
	// �浹 ��ġ ���� �� ��ȯ
	const D3DXVECTOR2& GetRectModify() { return m_vModify; }
	// �ǰ� ���� ��ȯ
	const bool& GetIsHit() const { return m_bIsHit; }

	// �׽�Ʈ �浹üũ ǥ�� ���� ��ȯ
	const bool& GetTestView() const { return m_bIsColliRectView; }

	const bool& GetIsDead() const { return m_bIsDead; }
	const bool& GetIsInteract() const { return m_bIsInteractObj; }

public:
	// ��ġ ���� ����
	void SetPos(D3DXVECTOR3 pos) { m_tInfo.vPos = pos; }
	// ���� ����
	void SetDegree(float degree) { m_fDegree = degree; }
	// �ǰ� �� ������ ����
	//void IsHit(int att) { m_tStatus.HP -= att; }
	// ������Ʈ Ű ����
	void SetObjKey(wstring objKey) { m_wstrObjKey = objKey; }
	// �ٴڰ��� �浹 ���� ����
	void SetColliFloor(bool bIsColli) { m_bIsFloorColli = bIsColli; }
	// �ӵ� ����
	void SetSpeed(float speed) { m_fSpeed = speed; }
	// �߷� ���ӵ� ����
	void SetFallSpeed(float speed) { m_fFallSpeed = speed; }
	// �浹 ���� ����
	void SetRect(RECT rc) { m_tRect = rc; }
	// �þ� ���� �� ���� ����
	void SetView(bool view) { m_bIsView = view; }
	// ü�� ����
	void SetHp(int hp) 
	{
		m_tStatus.MaxHP = hp; 
		m_tStatus.HP = m_tStatus.MaxHP;
	}
	// ���� ����
	void SetDir(D3DXVECTOR3 dir) { m_tInfo.vDir = dir; }

	void SetInfo(INFO info) { m_tInfo = info; }

	void SetInteract(bool is) { m_bIsInteractObj = is; }

protected:
	// ��ġ,ũ��,����,��� ���� ����ü
	INFO m_tInfo;
	// �ִϸ��̼� ������ ����ü
	FRAME m_tFrame;
	// �������ͽ� ����ü
	STATUS m_tStatus;
	// ����
	float m_fDegree;
	// �ӵ�
	float m_fSpeed;
	// �߷¼ӵ�
	float m_fFallSpeed;
	// �ٴڰ��� �浹 ����
	bool m_bIsFloorColli;
	// �ڽ��ڽ��� �浹 RECT
	RECT m_tRect;
	// �þ� RECT
	RECT m_tViewRect;
	// ���� RECT
	vector<RECT> m_vAttackRect;
	vector<D3DXVECTOR2> m_vAttackSize;
	// RECT ��ǥ ���� ��
	D3DXVECTOR2 m_vModify;
	vector<D3DXVECTOR2> m_vAttackModify;

	// ��� ����
	bool m_bIsDead;
	// �ǰ� ����
	bool m_bIsHit;
	OBJECT::TYPE m_eColliObjType;
	int m_iDamage;

	// �þ� ���� �� ����
	bool m_bIsView;

	// ���� 1ȸ ����� LateInit ����
	bool m_bIsInit;

	// �ؽ��� �������� ������ƮŰ
	wstring	m_wstrObjKey;

	// �׽�Ʈ�� �浹 ���� ǥ�� ����
	bool m_bIsColliRectView;

	bool m_bIsInteractObj;
};