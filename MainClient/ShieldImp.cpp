#include "stdafx.h"
#include "ShieldImp.h"
#include "PlayerInfo.h"
#include "Dagger.h"

#include "Money.h"

#include "Effect.h"
#include "AnimeEffect.h"

CShieldImp::CShieldImp()
	: m_pObserver(nullptr), m_eCurStance(END), m_fMoveTime(0.f), m_fAttackTime(0.f), m_fHurtTime(0.f),
	m_fHurtDirX(0.f), m_bIsThrow(false), m_bIsMove(false), m_fSoundTime(0.f)
{
}


CShieldImp::~CShieldImp()
{
	Release();
}

void CShieldImp::Initialize()
{
	m_tInfo.vDir = { 1.f, 0.f, 0.f };
	m_tInfo.vSize = { 36.f, 44.f, 0.f };
	m_vModify = { 0.f, 10.f };

	m_tStatus.HP = m_tStatus.MaxHP;

	m_wstrObjKey = L"Imp";
	m_wstrStateKey = L"ImpShield";

	m_eCurStance = PEACE;

	m_tFrame.fFrame = 0.f;
	m_tFrame.fMax = CTextureMgr::GetInstance()->GetTextureSize(m_wstrObjKey.c_str(), m_wstrStateKey.c_str());
	m_tFrame.fFrameAccel = 1.f;
	m_tFrame.fRestartFrame = 0.f;

	m_tStatus.Attack = 2;
}

void CShieldImp::LateInit()
{
	m_pObserver = new CPlayerInfo;
	CDataSubject::GetInstance()->Subscribe(m_pObserver);
}

int CShieldImp::Update()
{
	CObj::LateInit();

	if (m_bIsDead)
	{
		for (int i = 0; i < 10; ++i)
			CObjMgr::GetInstance()->AddObject(CAbstractFactory<CMoney>::CreateObject(m_tInfo.vPos), OBJECT::MONEY);

		return DEAD_OBJ;
	}

	Falling();

	if (m_bIsMove)
		Move();

	switch (m_eCurStance)
	{
	case PEACE:
		if (!m_bIsView)
		{
			if (m_fMoveTime == 0.f)
			{
				// 랜덤으로 이동 방향을 정함
				if ((rand() % 10 + 1) % 2 == 0)
					m_tInfo.vDir.x = -1.f;
				else
					m_tInfo.vDir.x = 1.f;
			}
			m_bIsMove = true;
		}
		else
		{
			D3DXVECTOR3 vPos = dynamic_cast<CPlayerInfo*>(m_pObserver)->GetInfo().vPos;
			// 시야범위 내 일정 거리가 안되면 플레이어쪽으로 다가옴
			if (CMath::CalculateDistanceX(vPos, m_tInfo.vPos) > 300.f)
			{
				if (m_fMoveTime == 0.f)
				{
					if (vPos.x > m_tInfo.vPos.x)
						m_tInfo.vDir.x = 1.f;
					else
						m_tInfo.vDir.x = -1.f;
				}
				m_bIsMove = true;
			}
			else
			{
				m_eCurStance = ATTACK;
				m_bIsMove = false;
				m_fMoveTime = 0.f;
			}
		}

		m_fSoundTime += CTimeMgr::GetInstance()->GetTime();
		if (m_fSoundTime > 5.f)
		{
			if ((rand() % 2 + 1) % 2 == 0)
				CSoundMgr::GetInstance()->PlaySound(L"sndImp1.wav.", CSoundMgr::MONSTER);
			else
				CSoundMgr::GetInstance()->PlaySound(L"sndImp2.wav", CSoundMgr::MONSTER);
			m_fSoundTime = 0.f;
		}

		break;
	case ATTACK:
		if (CMath::CalculateDistanceX(dynamic_cast<CPlayerInfo*>(m_pObserver)->GetInfo().vPos, m_tInfo.vPos) > 300.f)
			m_eCurStance = PEACE;
		else
		{
			D3DXVECTOR3 vPos = dynamic_cast<CPlayerInfo*>(m_pObserver)->GetInfo().vPos;
			// 플레이어가 임프보다 위에 있을때 플레이어 쪽으로 이동만 한다
			if (fabs(vPos.y - m_tInfo.vPos.y) > 40.f)
			{
				if (vPos.x > m_tInfo.vPos.x)
					m_tInfo.vDir.x = 1.f;
				else
					m_tInfo.vDir.x = -1.f;
				m_bIsMove = true;
			}
			else
			{
				m_fAttackTime += CTimeMgr::GetInstance()->GetTime();

				if (m_fAttackTime > 2.f)
				{	
					if (vPos.x > m_tInfo.vPos.x)
						m_tInfo.vDir.x = 1.f;
					else
						m_tInfo.vDir.x = -1.f;

					int randomAction = (rand() % 4 + 1) % 2;
					if (randomAction == 0) // 단검 던지기 
						FrameChange(L"ImpShieldThrow", 1.f, 0.f);
					else if(randomAction == 1) // 플레이어 쪽으로 이동
						m_bIsMove = true;

					m_bIsThrow = false;
					m_fAttackTime = 0.f;
				}
				if ((int)m_tFrame.fFrame == 2 && !m_bIsThrow)
				{
					if (m_wstrStateKey == L"ImpShieldThrow")
						CObjMgr::GetInstance()->AddObject(CAbstractFactory<CDagger>::CreateObject(D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y + 13.f, 0.f), m_tInfo.vDir),
							OBJECT::DAGGER);
					CSoundMgr::GetInstance()->PlaySound(L"sndThrow.wav", CSoundMgr::EFFECT);
					m_bIsThrow = true;
				}
			}
		}
		break;
	case HURT:
		// 피격 이펙트
		if (m_eColliObjType == OBJECT::PLAYER_ATTACK && !m_fHurtTime)
		{
			wstring HitStateKey = L"";
			INFO HitInfo = {};

			if (dynamic_cast<CPlayerInfo*>(m_pObserver)->GetInfo().vPos.x > m_tInfo.vPos.x)
				HitInfo.vDir.x = -1.f;
			else
				HitInfo.vDir.x = 1.f;

			HitInfo.vPos.x = m_tInfo.vPos.x + (30.f * HitInfo.vDir.x);
			HitInfo.vPos.y = m_tInfo.vPos.y;
			switch (m_iDamage)
			{
				// 1타 공격
			case 2:
				HitStateKey = L"HitSmall";
				break;
				// 2타 공격
			case 3:
				HitStateKey = L"HitMedium";
				break;
				// 3타 공격
			case 5:
				HitStateKey = L"HitLarge";
				break;
				// 공중 공격
			case 4:
				HitStateKey = L"HitAir";
				break;
			}
			CObjMgr::GetInstance()->AddObject(CEffectFactory<CEffect, CAnimeEffect>::CreateEffect(L"HitEffect", HitStateKey.c_str(),
				HitInfo, FRAME(2.5f)), OBJECT::EFFECT);
			CSoundMgr::GetInstance()->PlaySound(L"sndPressurePlate.wav", CSoundMgr::EFFECT);
		}

		if (!m_fHurtTime)
		{
			m_tStatus.HP -= m_iDamage;
			m_iDamage = 0;
			m_fHurtDirX = dynamic_cast<CPlayerInfo*>(m_pObserver)->GetInfo().vDir.x;
			m_tInfo.vDir.x = -m_fHurtDirX;
			CSoundMgr::GetInstance()->PlaySound(L"sndImpDeathAlt.wav", CSoundMgr::MONSTER);
		}
		if (m_fHurtTime > 0.8f)
		{
			NotHit();
			m_fHurtTime = 0.f;
			m_eCurStance = PEACE;
			break;
		}

		m_fHurtTime += CTimeMgr::GetInstance()->GetTime();
		m_tInfo.vPos.x += 10.f * m_fHurtDirX * CTimeMgr::GetInstance()->GetTime();

		break;
	}

	m_tInfo.matWorld = CMath::CalculateMatrix(m_tInfo.vPos + CScrollMgr::GetScroll(), D3DXVECTOR3(m_tInfo.vDir.x, 1.f, 0.f));
	
	m_tFrame.fFrame += m_tFrame.fMax * CTimeMgr::GetInstance()->GetTime() * m_tFrame.fFrameAccel;

	if (m_tFrame.fFrame > m_tFrame.fMax)
	{
		m_tFrame.fFrame = m_tFrame.fRestartFrame;
		if (m_eCurStance == ATTACK)
			FrameChange(L"ImpShield", 1.f, 0.f);
	}

	return NO_EVENT;
}

void CShieldImp::LateUpdate()
{
	if (m_bIsHit && (m_eColliObjType != OBJECT::PLAYER))
	{
		// 등뒤에서 때렸을때
		if (m_tInfo.vDir.x == dynamic_cast<CPlayerInfo*>(m_pObserver)->GetInfo().vDir.x)
		{
			m_eCurStance = HURT;
			FrameChange(L"ImpShieldHit", 1.f, 0.f);
			CScrollMgr::ScreenShakeSmallOn();
		}
		else if((m_tInfo.vDir.x != dynamic_cast<CPlayerInfo*>(m_pObserver)->GetInfo().vDir.x) && (m_eCurStance != HURT))
		{
			CSoundMgr::GetInstance()->PlaySound(L"sndNoDamage.wav", CSoundMgr::EFFECT);
			NotHit();
		}

		if (m_wstrStateKey == L"ImpShieldThrow")
		{
			m_eCurStance = HURT;
			FrameChange(L"ImpShieldHit", 1.f, 0.f);
			CScrollMgr::ScreenShakeSmallOn();
		}
		
	}

	if (m_tStatus.HP < 0.f)
	{
		CSoundMgr::GetInstance()->PlaySound(L"sndImpDeath.wav", CSoundMgr::MONSTER);
		CSoundMgr::GetInstance()->PlaySound(L"sndGeyser.wav", CSoundMgr::EFFECT);
		CScrollMgr::ScreenShakeOn();
		m_bIsDead = true;
	}
	
	if (m_wstrStateKey != L"ImpShieldThrow" && m_wstrStateKey != L"ImpShieldHit")
	{
		AttackColliClear();
		m_vAttackSize.push_back(D3DXVECTOR2(26.f, 44.f));
		m_vAttackModify.push_back(D3DXVECTOR2(17.f, 10.f));
	}
	else
	{
		AttackColliClear();
		m_vAttackSize.push_back(D3DXVECTOR2(0.f, 0.f));
		m_vAttackModify.push_back(D3DXVECTOR2(0.f, 0.f));
	}
	
}

void CShieldImp::Render()
{
	CObj::UpdateRect();
	CObj::UpdateViewRect(800.f);
	CObj::UpdateAttackRect();

	const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
		m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), (int)m_tFrame.fFrame);

	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	if (CObjMgr::GetInstance()->GetPlayer()->GetTestView())
	{
		RenderRect();
		RenderAttackRect();
	}
}

void CShieldImp::RenderRect()
{
	D3DXVECTOR2 m_Line[5];
	m_Line[0] = { (float)m_tRect.left + CScrollMgr::GetScroll().x, (float)m_tRect.top + CScrollMgr::GetScroll().y };
	m_Line[1] = { (float)m_tRect.right + CScrollMgr::GetScroll().x, (float)m_tRect.top + CScrollMgr::GetScroll().y };
	m_Line[2] = { (float)m_tRect.right + CScrollMgr::GetScroll().x, (float)m_tRect.bottom + CScrollMgr::GetScroll().y };
	m_Line[3] = { (float)m_tRect.left + CScrollMgr::GetScroll().x, (float)m_tRect.bottom + CScrollMgr::GetScroll().y };
	m_Line[4] = { (float)m_tRect.left + CScrollMgr::GetScroll().x, (float)m_tRect.top + CScrollMgr::GetScroll().y };

	CDevice::GetInstance()->GetSprite()->End();
	CDevice::GetInstance()->GetLine()->Begin();
	CDevice::GetInstance()->GetLine()->Draw(m_Line, 5, D3DCOLOR_XRGB(0, 0, 255));
	CDevice::GetInstance()->GetLine()->End();
	CDevice::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

	// 시야 범위
	const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Circle");
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	D3DXMATRIX matWorld = CMath::CalculateMatrix(m_tInfo.vPos + CScrollMgr::GetScroll(), D3DXVECTOR3(0.8f, 0.8f, 0.f));

	CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CShieldImp::RenderAttackRect()
{
	for (auto& AttackRect : m_vAttackRect)
	{
		D3DXVECTOR2 m_Line[5];
		m_Line[0] = { (float)AttackRect.left + CScrollMgr::GetScroll().x, (float)AttackRect.top + CScrollMgr::GetScroll().y };
		m_Line[1] = { (float)AttackRect.right + CScrollMgr::GetScroll().x, (float)AttackRect.top + CScrollMgr::GetScroll().y };
		m_Line[2] = { (float)AttackRect.right + CScrollMgr::GetScroll().x, (float)AttackRect.bottom + CScrollMgr::GetScroll().y };
		m_Line[3] = { (float)AttackRect.left + CScrollMgr::GetScroll().x, (float)AttackRect.bottom + CScrollMgr::GetScroll().y };
		m_Line[4] = { (float)AttackRect.left + CScrollMgr::GetScroll().x, (float)AttackRect.top + CScrollMgr::GetScroll().y };

		CDevice::GetInstance()->GetSprite()->End();
		CDevice::GetInstance()->GetLine()->Begin();
		CDevice::GetInstance()->GetLine()->Draw(m_Line, 5, D3DCOLOR_XRGB(0, 255, 0));
		CDevice::GetInstance()->GetLine()->End();
		CDevice::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	}
}

void CShieldImp::Release()
{
	SafeDeleteObserver<CDataSubject>(m_pObserver);
}

void CShieldImp::Falling()
{
	m_tInfo.vPos.y += m_fFallSpeed;

	// 점프 상태가 아니고 땅에 도달했을 때
	if (m_fFallSpeed < 0.1f && m_eCurStance == PEACE)
		FrameChange(L"ImpShield", 1.f, 0.f);

	// 지속적으로 중력을 받음
	m_fFallSpeed += (GRAVITY * 0.3f) * CTimeMgr::GetInstance()->GetTime();
}

void CShieldImp::Move()
{
	m_fMoveTime += CTimeMgr::GetInstance()->GetTime();

	if (m_fMoveTime > 3.f && m_fMoveTime < 5.f)
	{
		m_tInfo.vPos.x += m_tInfo.vDir.x * 50.f * CTimeMgr::GetInstance()->GetTime();

		FrameChange(L"ImpShieldWalk", 1.f, 0.f);
	}
	if (m_fMoveTime > 5.f)
	{
		m_fMoveTime = 0.f;
		m_bIsMove = false;
	}
}