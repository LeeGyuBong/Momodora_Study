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
	// VECTOR 정보 반환
	INFO& GetInfo() { return m_tInfo; }
	// 스테이터스 정보 반환
	const STATUS& GetStatus() const { return m_tStatus; }
	// 각도 반환
	const float& GetDegree() const { return m_fDegree; }
	// 자기 자신의 RECT 반환
	const RECT& GetRect() const { return m_tRect; }
	// 시야 RECT 반환
	const RECT& GetViewRect() const { return m_tViewRect; }
	// 공격 RECT 반환
	const vector<RECT>& GetAttackRect() const { return m_vAttackRect; }
	// 오브젝트 키 반환
	const wstring& GetObjKey() { return m_wstrObjKey; }
	// 바닥 충돌 여부 반환
	const bool& GetFloorColli() const { return m_bIsFloorColli; }
	// 속도 반환
	const float& GetSpeed() const { return m_fSpeed; }
	// 중력 가속도 반환
	const float& GetFallSpeed() const { return m_fFallSpeed; }
	// 충돌 위치 수정 값 반환
	const D3DXVECTOR2& GetRectModify() { return m_vModify; }
	// 피격 여부 반환
	const bool& GetIsHit() const { return m_bIsHit; }

	// 테스트 충돌체크 표현 여부 반환
	const bool& GetTestView() const { return m_bIsColliRectView; }

	const bool& GetIsDead() const { return m_bIsDead; }
	const bool& GetIsInteract() const { return m_bIsInteractObj; }

public:
	// 위치 벡터 설정
	void SetPos(D3DXVECTOR3 pos) { m_tInfo.vPos = pos; }
	// 각도 설정
	void SetDegree(float degree) { m_fDegree = degree; }
	// 피격 시 데미지 설정
	//void IsHit(int att) { m_tStatus.HP -= att; }
	// 오브젝트 키 설정
	void SetObjKey(wstring objKey) { m_wstrObjKey = objKey; }
	// 바닥과의 충돌 여부 설정
	void SetColliFloor(bool bIsColli) { m_bIsFloorColli = bIsColli; }
	// 속도 설정
	void SetSpeed(float speed) { m_fSpeed = speed; }
	// 중력 가속도 설정
	void SetFallSpeed(float speed) { m_fFallSpeed = speed; }
	// 충돌 범위 설정
	void SetRect(RECT rc) { m_tRect = rc; }
	// 시야 범위 내 유무 설정
	void SetView(bool view) { m_bIsView = view; }
	// 체력 설정
	void SetHp(int hp) 
	{
		m_tStatus.MaxHP = hp; 
		m_tStatus.HP = m_tStatus.MaxHP;
	}
	// 방향 설정
	void SetDir(D3DXVECTOR3 dir) { m_tInfo.vDir = dir; }

	void SetInfo(INFO info) { m_tInfo = info; }

	void SetInteract(bool is) { m_bIsInteractObj = is; }

protected:
	// 위치,크기,방향,행렬 정보 구조체
	INFO m_tInfo;
	// 애니메이션 프레임 구조체
	FRAME m_tFrame;
	// 스테이터스 구조체
	STATUS m_tStatus;
	// 각도
	float m_fDegree;
	// 속도
	float m_fSpeed;
	// 중력속도
	float m_fFallSpeed;
	// 바닥과의 충돌 여부
	bool m_bIsFloorColli;
	// 자신자신의 충돌 RECT
	RECT m_tRect;
	// 시야 RECT
	RECT m_tViewRect;
	// 공격 RECT
	vector<RECT> m_vAttackRect;
	vector<D3DXVECTOR2> m_vAttackSize;
	// RECT 좌표 수정 값
	D3DXVECTOR2 m_vModify;
	vector<D3DXVECTOR2> m_vAttackModify;

	// 사망 여부
	bool m_bIsDead;
	// 피격 여부
	bool m_bIsHit;
	OBJECT::TYPE m_eColliObjType;
	int m_iDamage;

	// 시야 범위 내 유무
	bool m_bIsView;

	// 최초 1회 적용될 LateInit 변수
	bool m_bIsInit;

	// 텍스쳐 렌더링용 오브젝트키
	wstring	m_wstrObjKey;

	// 테스트용 충돌 범위 표현 여부
	bool m_bIsColliRectView;

	bool m_bIsInteractObj;
};