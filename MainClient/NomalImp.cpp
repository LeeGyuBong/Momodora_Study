#include "stdafx.h"
#include "NomalImp.h"
#include "PlayerInfo.h"
#include "Dagger.h"
#include "PoisionBomb.h"

#include "Money.h"

#include "Effect.h"
#include "AnimeEffect.h"

CNomalImp::CNomalImp()
	: m_fPosY(0.f), m_fMoveTime(0.f), m_fJumpTime(0.f), m_fJumpPower(30.f),
	m_eCurStance(END), m_pObserver(nullptr), m_bIsJumping(false), m_fAttackTime(0.f),
	m_fHurtTime (0.f), m_iInviAlpha(0), m_fHurtDirX(0.f), m_bIsThrow(false),
	m_fSoundTime(0.f)
{
}


CNomalImp::~CNomalImp()
{
	Release();
}

void CNomalImp::Initialize()
{
	m_tInfo.vDir = { 1.f, 0.f, 0.f };
	m_tInfo.vSize = { 25.f, 38.f, 0.f };
	m_vModify = { 0.f, 13.f };

	m_tStatus.HP = m_tStatus.MaxHP;

	m_wstrObjKey = L"Imp";
	m_wstrStateKey = L"Imp";

	m_eCurStance = PEACE;

	m_tFrame.fFrame = 0.f;
	m_tFrame.fMax = CTextureMgr::GetInstance()->GetTextureSize(m_wstrObjKey.c_str(), m_wstrStateKey.c_str());
	m_tFrame.fFrameAccel = 1.f;
	m_tFrame.fRestartFrame = 0.f;
}

void CNomalImp::LateInit()
{
	m_pObserver = new CPlayerInfo;
	CDataSubject::GetInstance()->Subscribe(m_pObserver);
}

int CNomalImp::Update()
{
	CObj::LateInit();

	if (m_bIsDead)
	{
		for (int i = 0; i < 10; ++i)
			CObjMgr::GetInstance()->AddObject(CAbstractFactory<CMoney>::CreateObject(m_tInfo.vPos), OBJECT::MONEY);
		return DEAD_OBJ;
	}

	Falling();

	if (m_bIsJumping)
		Move();

	switch (m_eCurStance)
	{
	case PEACE:
		// 시야 범위 내에 플레이어가 없을 시
		if (!m_bIsView)
		{
			m_fMoveTime += CTimeMgr::GetInstance()->GetTime();
			if (!m_bIsJumping && m_fMoveTime > 2.f)
			{
				m_bIsJumping = true;
				m_bIsFloorColli = false;

				m_fPosY = m_tInfo.vPos.y;

				// 좌우 방향 랜덤으로 점프
				if ((rand() % 10 + 1) % 2 == 0)
					m_tInfo.vDir.x = -1.f;
				else
					m_tInfo.vDir.x = 1.f;

				FrameChange(L"ImpJump", 1.f, 0.f);
			}
		}
		// 시야 범위 내에 플레이어가 있을 시
		else
		{
			if (CMath::CalculateDistanceX(dynamic_cast<CPlayerInfo*>(m_pObserver)->GetInfo().vPos, m_tInfo.vPos) > 300.f)
			{
				m_fMoveTime += CTimeMgr::GetInstance()->GetTime();
				if (!m_bIsJumping && m_fMoveTime > 2.f)
				{
					m_bIsJumping = true;
					m_bIsFloorColli = false;

					m_fPosY = m_tInfo.vPos.y;

					if (dynamic_cast<CPlayerInfo*>(m_pObserver)->GetInfo().vPos.x > m_tInfo.vPos.x)
						m_tInfo.vDir.x = 1.f;
					else
						m_tInfo.vDir.x = -1.f;

					FrameChange(L"ImpJump", 1.f, 0.f);
				}
			}
			else
				m_eCurStance = ATTACK;
		}

		m_fSoundTime += CTimeMgr::GetInstance()->GetTime();
		if (m_fSoundTime > 5.f)
		{
			if ((rand() % 2 + 1) % 2 == 0)
				CSoundMgr::GetInstance()->PlaySound(L"sndImp1.wav", CSoundMgr::MONSTER);
			else
				CSoundMgr::GetInstance()->PlaySound(L"sndImp2.wav", CSoundMgr::MONSTER);
			m_fSoundTime = 0.f;
		}

		break;
	case ATTACK:
		// 일정 거리 내에 있지 않을 시
		if (CMath::CalculateDistanceX(dynamic_cast<CPlayerInfo*>(m_pObserver)->GetInfo().vPos, m_tInfo.vPos) > 300.f)
			m_eCurStance = PEACE;
		else
		{
			m_fAttackTime += CTimeMgr::GetInstance()->GetTime();
			//  일정 시간마다 공격
			if (m_fAttackTime > 2.f)
			{
				D3DXVECTOR3 vPos = dynamic_cast<CPlayerInfo*>(m_pObserver)->GetInfo().vPos;
				// 플레이어가 임프보다 위에 있을 때 플레이어 쪽으로 이동만 한다
				if (fabs(vPos.y - m_tInfo.vPos.y) > 40.f)
				{
					m_bIsJumping = true;
					m_bIsFloorColli = false;
					m_fPosY = m_tInfo.vPos.y;
				}
				else
				{
					int randomAction = (rand() % 10 + 1) % 3;
					if (randomAction == 0) // 칼 던지기
						FrameChange(L"ImpThrow", 1.f, 0.f);
					else if (randomAction == 1) // 폭탄 던지기
						FrameChange(L"ImpThrowBomb", 1.f, 0.f);
					else if(randomAction == 3) // 플레이어 쪽으로 이동
					{
						m_bIsJumping = true;
						m_bIsFloorColli = false;
						m_fPosY = m_tInfo.vPos.y;
					}
				}
				
				m_bIsThrow = false;
				m_fAttackTime = 0.f;

				if (vPos.x > m_tInfo.vPos.x)
					m_tInfo.vDir.x = 1.f;
				else
					m_tInfo.vDir.x = -1.f;
			}
			
			// 공격 오브젝트를 생성하는 구간(프레임)일 경우
			if ((int)m_tFrame.fFrame == 4 && !m_bIsThrow)
			{
				if (m_wstrStateKey == L"ImpThrow")
					CObjMgr::GetInstance()->AddObject(CAbstractFactory<CDagger>::CreateObject(D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y + 13.f, 0.f), m_tInfo.vDir), 
						OBJECT::DAGGER);
				else if (m_wstrStateKey == L"ImpThrowBomb")
					CObjMgr::GetInstance()->AddObject(CAbstractFactory<CPoisionBomb>::CreateObject(D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y + 13.f, 0.f), m_tInfo.vDir),
						OBJECT::POISIONBOMB);
				CSoundMgr::GetInstance()->PlaySound(L"sndThrow.wav", CSoundMgr::EFFECT);
				m_bIsThrow = true;
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
			m_fHurtDirX = dynamic_cast<CPlayerInfo*>(m_pObserver)->GetInfo().vDir.x;
			m_tInfo.vDir.x = -m_fHurtDirX;
			CSoundMgr::GetInstance()->PlaySound(L"sndImpDeathAlt.wav", CSoundMgr::MONSTER);
		}

		if (m_fHurtTime > 1.f)
		{
			NotHit();
			/*m_iInviAlpha = 0;*/
			m_fHurtTime = 0.f;
			m_iDamage = 0;
			m_eCurStance = PEACE;
			break;
		}

		m_fHurtTime += CTimeMgr::GetInstance()->GetTime();

		/*if (m_iInviAlpha)
			m_iInviAlpha = 0;
		else
			m_iInviAlpha = 1;*/

		m_tInfo.vPos.x += 10.f * m_fHurtDirX * CTimeMgr::GetInstance()->GetTime();
		
		break;
	}

	m_tInfo.matWorld = CMath::CalculateMatrix(m_tInfo.vPos + CScrollMgr::GetScroll(), D3DXVECTOR3(m_tInfo.vDir.x, 1.f, 0.f));

	m_tFrame.fFrame += m_tFrame.fMax * CTimeMgr::GetInstance()->GetTime() * m_tFrame.fFrameAccel;

	if (m_tFrame.fFrame > m_tFrame.fMax)
	{
		m_tFrame.fFrame = m_tFrame.fRestartFrame;
		if (m_eCurStance == ATTACK)
			FrameChange(L"Imp", 1.f, 0.f);
	}

	return NO_EVENT;
}

void CNomalImp::LateUpdate()
{
	if (m_bIsHit && (m_eColliObjType != OBJECT::PLAYER))
	{
		m_eCurStance = HURT;
		FrameChange(L"ImpHurt", 1.f, 0.f);
		CScrollMgr::ScreenShakeSmallOn();
	}

	if (m_tStatus.HP < 0.f)
	{
		m_bIsDead = true;
		CSoundMgr::GetInstance()->PlaySound(L"sndImpDeath.wav", CSoundMgr::MONSTER);
		CSoundMgr::GetInstance()->PlaySound(L"sndGeyser.wav", CSoundMgr::EFFECT);
		CScrollMgr::ScreenShakeOn();
	}
}

void CNomalImp::Render()
{
	CObj::UpdateRect();
	CObj::UpdateViewRect(800.f);

	const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
		m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), (int)m_tFrame.fFrame);

	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
		D3DCOLOR_ARGB(255 /*+ m_iInviAlpha*/, 255, 255, 255));

	if (CObjMgr::GetInstance()->GetPlayer()->GetTestView())
		RenderRect();
}

void CNomalImp::RenderRect()
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

	D3DXMATRIX matWorld = CMath::CalculateMatrix(m_tInfo.vPos + CScrollMgr::GetScroll(), D3DXVECTOR3(0.8f, 0.8f,0.f));

	CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CNomalImp::Release()
{
	SafeDeleteObserver<CDataSubject>(m_pObserver);
}

void CNomalImp::Move()
{
	float Height = (powf(m_fJumpTime, 2.f) * -GRAVITY * 0.5f) + (m_fJumpTime * m_fJumpPower);
	// 최대 높이
	float MaxHeight = powf(m_fJumpPower, 2.f) / (GRAVITY * 2.f);

	m_tInfo.vPos.y = m_fPosY - Height;
	m_fJumpTime += CTimeMgr::GetInstance()->GetTime() * 10.f;

	m_tInfo.vPos.x += m_tInfo.vDir.x * 100.f * CTimeMgr::GetInstance()->GetTime();

	// 최대 높이 도달 시 떨어지는 상태로 전환
	if ((int)MaxHeight < Height)
	{
		FrameChange(L"ImpFall", 3.5f, CTextureMgr::GetInstance()->GetTextureSize(m_wstrObjKey.c_str(), m_wstrStateKey.c_str()));
	}

	// 바닥에 충돌시 점프 변수 값들을 초기화 및 Idle 상태로 전환
	if (m_fPosY < m_tInfo.vPos.y)
	{
		m_bIsJumping = false;
		m_fJumpTime = 0.f;

		m_fMoveTime = 0.f;
	
		FrameChange(L"Imp", 1.f, 0.f);
	}
}

void CNomalImp::Falling()
{
	m_tInfo.vPos.y += m_fFallSpeed;

	// 점프 상태가 아니고 땅에 도달했을 때
	if (m_fFallSpeed < 0.1f && !m_bIsJumping && m_eCurStance == PEACE)
		FrameChange(L"Imp", 1.f, 0.f);

	// 지속적으로 중력을 받음
	m_fFallSpeed += (GRAVITY * 0.3f) * CTimeMgr::GetInstance()->GetTime();
}