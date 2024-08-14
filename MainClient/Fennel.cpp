#include "stdafx.h"
#include "Fennel.h"

#include "BossHp.h"

#include "Effect.h"
#include "AnimeEffect.h"
#include "NonAnimeEffect.h"

#include "PlayerInfo.h"

CFennel::CFennel()
	: m_pObserver(nullptr), m_eCurStance(END), m_fJumpPower(0.f), m_fJumpTime(0.f), m_fPosY(0.f),
	TextIndex(0), m_bIsBuff(false), m_fAttackTime(0.f), m_eCurAttack(ATTACKSTANCE_END), m_fAttackRandomTime(0.f),
	m_fDashTime(0.f), m_fInviTime(0.f), m_bIsLighting(false), m_bIsLighting2(false), m_fLightingDirX(1.f),
	m_bIsJump(false), m_iLightingCount(0), m_iHitRandom(0), m_iAlpha(255), m_fActivateTime(0.f), m_bIsActivate(false)
{
	D3DXMatrixIdentity(&matPreWorld);
	D3DXMatrixIdentity(&matPreWorld2);
}


CFennel::~CFennel()
{
	Release();
}

void CFennel::Initialize()
{
	m_tInfo.vPos = { 1000.f, CLIENTWINCX * 0.7f, 0.f };

	m_tInfo.vDir = { 1.f, 0.f, 0.f };

	m_tInfo.vSize = { 34.f, 70.f, 0.f };
	m_vModify = { 0.f, -1.f };

	m_tStatus.MaxHP = 226;
	m_tStatus.HP = m_tStatus.MaxHP;

	m_fSpeed = 300.f;
	m_fJumpPower = 35.f;

	m_wstrObjKey = L"Fennel";
	m_wstrStateKey = L"FennelIntro1";

	m_eCurStance = PEACE;

	m_tFrame.fFrame = 0.f;
	m_tFrame.fMax = 0.f;
	m_tFrame.fFrameAccel = 1.1f;
	m_tFrame.fRestartFrame = m_tFrame.fMax;
}

void CFennel::LateInit()
{
	m_pObserver = new CPlayerInfo;
	CDataSubject::GetInstance()->Subscribe(m_pObserver);

	CDataSubject::GetInstance()->AddData(OBSERVER::BOSS_STATUS, &m_tStatus);
}

int CFennel::Update()
{
	CObj::LateInit();

	if (m_bIsDead)
		return DEAD_OBJ;

	Falling();

	if (!m_bIsActivate)
	{
		m_fActivateTime += CTimeMgr::GetInstance()->GetTime();

		if (m_fActivateTime > 1.f)
			m_bIsActivate = true;
	}

	switch (m_eCurStance)
	{
	case PEACE :
		if (CTextMgr::GetInstance()->GetTalkIndex() == 1)
		{
			m_tFrame.fMax = CTextureMgr::GetInstance()->GetTextureSize(m_wstrObjKey.c_str(), m_wstrStateKey.c_str());
			m_tFrame.fRestartFrame = m_tFrame.fMax;
			break;
		}
		if (CTextMgr::GetInstance()->GetTalkIndex() == 3)
		{
			if (m_wstrStateKey == L"FennelIntro1")
			{
				CSoundMgr::GetInstance()->PlaySound(L"sndFennelIntrohead.wav", CSoundMgr::MONSTER);
				CSoundMgr::GetInstance()->PlayBGM(L"Pardoner's Dance.mp3");
			}

			FrameChange(L"FennelIntro2", 1.f, -1.f);
			break;
		}
		else if (CTextMgr::GetInstance()->GetTalkIndex() == 6)
		{
			if (m_wstrStateKey == L"FennelIntro2")
				CSoundMgr::GetInstance()->PlaySound(L"sndFennelIntroattack.wav", CSoundMgr::MONSTER);

			FrameChange(L"FennelIntro3", 0.9f, -1.f);
			break;
		}
		else if (m_wstrStateKey == L"FennelIntro3" && !CTextMgr::GetInstance()->GetIsTalk())
		{
			FrameChange(L"FennelIntro4", 8.f, -1.f);
			break;
		}
		else if (m_wstrStateKey == L"FennelIntro4" && m_tFrame.m_bIsEnd)
		{
			m_eCurStance = IDLE;
			CUiMgr::GetInstance()->Add_UI(CAbstractFactory<CBossHp>::CreateObject(), CUiMgr::BOSS);
			CDataSubject::GetInstance()->Notify(OBSERVER::BOSS_STATUS, &m_tStatus);
			CSceneMgr::GetInstance()->SetBossfight(TRUE);

			break;
		}
		// 플레이어가 일정 거리에 들어오면 대화가 시작됨
		if (CMath::CalculateDistanceX(m_tInfo.vPos, dynamic_cast<CPlayerInfo*>(m_pObserver)->GetInfo().vPos) < 350.f && m_bIsActivate)
		{
			if (m_wstrStateKey == L"FennelIntro1")
			{
				CTextMgr::GetInstance()->StartTalk(0, 7);
				CSceneMgr::GetInstance()->SetBoss(TRUE);
				CSoundMgr::GetInstance()->StopSound(CSoundMgr::BGM);
			}
		}
		break;
	case IDLE:
		FrameChange(L"FennelIdle", 1.f, 0.f);

		if (dynamic_cast<CPlayerInfo*>(m_pObserver)->GetInfo().vPos.x > m_tInfo.vPos.x)
			m_tInfo.vDir.x = 1.f;
		else
			m_tInfo.vDir.x = -1.f;

		// 1~3초 간격
		if(!m_fAttackRandomTime)
			m_fAttackRandomTime = static_cast<float>(rand() % 3 + 1);

		m_fAttackTime += CTimeMgr::GetInstance()->GetTime();

		// 일정시간마다 공격
		if (m_fAttackTime > m_fAttackRandomTime)
		{
			int random = rand() % 3 + 1;

			if (random == 1)
			{
				m_eCurAttack = NOMAL_ATTACK;
				CSoundMgr::GetInstance()->PlaySound(L"sndFennelAttack.wav", CSoundMgr::MONSTER);
			}
			else if (random == 2)
				m_eCurAttack = SLAM_LIGHTING;
			else if (random == 3)
			{
				if (CMath::CalculateDistanceX(m_tInfo.vPos, dynamic_cast<CPlayerInfo*>(m_pObserver)->GetInfo().vPos) < 250.f)
					m_eCurAttack = PLUNGE_IMPACT;
				else
					m_eCurAttack = DASH_ATTACK;
			}

			m_eCurStance = ATTACK;
			m_fAttackTime = 0.f;
			m_fAttackRandomTime = 0.f;
		}
		break;
	case ATTACK:
		switch (m_eCurAttack)
		{
		case NOMAL_ATTACK:
			FrameChange(L"FennelAttack", 1.f, 0.f);
			m_tInfo.vPos.x += m_tInfo.vDir.x * 200.f * CTimeMgr::GetInstance()->GetTime();

			if (m_tFrame.fFrame > 14.f && m_tFrame.fFrame < 14.13f)
				CSoundMgr::GetInstance()->PlaySound(L"sndFennelAttack.wav", CSoundMgr::MONSTER);

			if (m_tFrame.m_bIsEnd)
			{
				if (dynamic_cast<CPlayerInfo*>(m_pObserver)->GetInfo().vPos.x > m_tInfo.vPos.x)
					m_tInfo.vDir.x = 1.f;
				else
					m_tInfo.vDir.x = -1.f;

				if (m_bIsBuff)
					m_eCurAttack = JUMP_ATTACK;
				else
				{
					m_eCurAttack = BACKFLIP;
					CSoundMgr::GetInstance()->PlaySound(L"sndFennelBackflip.wav", CSoundMgr::MONSTER);
				}
			}
			break;
		case DASH_ATTACK:
			if (!m_fDashTime)
			{
				FrameChange(L"FennelDash", 1.f, 0.f);
				CSoundMgr::GetInstance()->PlaySound(L"sndMonkeyJump.wav", CSoundMgr::MONSTER);
			}
			if (m_wstrStateKey == L"FennelDash")
			{
				m_fDashTime += CTimeMgr::GetInstance()->GetTime();

				if (m_fDashTime < 0.5f)
					m_tInfo.vPos.x += m_tInfo.vDir.x * 400.f * CTimeMgr::GetInstance()->GetTime();
				else if (m_fDashTime > 0.5f)
				{
					m_eCurAttack = NOMAL_ATTACK;
					CSoundMgr::GetInstance()->PlaySound(L"sndFennelAttack.wav", CSoundMgr::MONSTER);
					m_fDashTime = 0;
				}
			}
			break;
		case SLAM_LIGHTING:
			FrameChange(L"FennelSlam", 0.6f, 0.f);

			if ((int)m_tFrame.fFrame == 4)
			{
				if (!m_bIsLighting)
				{
					if (dynamic_cast<CPlayerInfo*>(m_pObserver)->GetInfo().vPos.x > m_tInfo.vPos.x)
						m_fLightingDirX = -1.f;
					else
						m_fLightingDirX = 1.f;

					for (int i = 0; i < 4; ++i)
					{
						float x = dynamic_cast<CPlayerInfo*>(m_pObserver)->GetInfo().vPos.x;
						int randomX = rand() % 100 + 1;
						if ((rand() % 2 + 1) % 2 == 0)
							randomX *= -1;

						CObjMgr::GetInstance()->AddObject(CEffectFactory<CEffect, CNonAnimeEffect>::CreateEffect(m_wstrObjKey.c_str(), L"Lighting", 
							D3DXVECTOR3(x + (float)randomX, CLIENTWINCY * 0.5f + 100.f, 0.f), 0, 130, 0.2f), OBJECT::EFFECT);
						SlamLightingPos[i] = { x + (float)randomX , CLIENTWINCY * 0.5f + 100.f };
					}
					m_bIsLighting = true;
					CSoundMgr::GetInstance()->PlaySound(L"sndFennelThunder1.wav", CSoundMgr::MONSTER);
				}
			}
			if ((int)m_tFrame.fFrame == 16)
			{
				if (!m_bIsLighting2)
				{
					for (int i = 0; i < 4; ++i)
					{
						CObjMgr::GetInstance()->AddObject(CEffectFactory<CEffect, CAnimeEffect>::CreateEffect(m_wstrObjKey.c_str(), L"Lighting", 
							D3DXVECTOR3(SlamLightingPos[i].x, SlamLightingPos[i].y, 0.f), FRAME(2.f)), OBJECT::EFFECT);
					}
					m_bIsLighting2 = true;
					CScrollMgr::ScreenShakeOn();
					CSoundMgr::GetInstance()->PlaySound(L"sndFennelThunder2.wav", CSoundMgr::MONSTER);
				}
			}
			if (m_tFrame.m_bIsEnd)
			{
				m_eCurStance = IDLE;
				m_eCurAttack = ATTACKSTANCE_END;
				m_bIsLighting = false;
				m_bIsLighting2 = false;
			}
			break;
		case JUMP_ATTACK:
			if (!m_bIsJump)
			{
				m_fPosY = m_tInfo.vPos.y;
				m_fJumpPower = 35.f;
				m_bIsJump = true;
				FrameChange(L"FennelAttackAir", 1.6f, -1.f);
				CSoundMgr::GetInstance()->PlaySound(L"sndFennelAttack2.wav", CSoundMgr::MONSTER);
			}

			if (m_bIsJump)
			{
				float Height = (powf(m_fJumpTime, 2.f) * -GRAVITY * 0.5f) + (m_fJumpTime * m_fJumpPower);
		
				m_tInfo.vPos.x += m_tInfo.vDir.x * 300.f * CTimeMgr::GetInstance()->GetTime();
				m_tInfo.vPos.y = m_fPosY - Height;

				m_fJumpTime += CTimeMgr::GetInstance()->GetTime() * 10.f;

				if (m_fPosY < m_tInfo.vPos.y)
				{
					if (dynamic_cast<CPlayerInfo*>(m_pObserver)->GetInfo().vPos.x > m_tInfo.vPos.x)
						m_tInfo.vDir.x = 1.f;
					else
						m_tInfo.vDir.x = -1.f;

					m_eCurAttack = BACKFLIP;
					CSoundMgr::GetInstance()->PlaySound(L"sndFennelBackflip.wav", CSoundMgr::MONSTER);
					m_bIsJump = false;
					m_fJumpTime = 0.f;
				}
			}
			break;
		case BUFF:
			if (!m_bIsBuff)
			{
				INFO Info;
				Info.vPos = m_tInfo.vPos;
				Info.vDir = { m_tInfo.vDir.x * -1.f, 1.f, 0.f };
			
				FrameChange(L"FennelBuff", 0.5f, -1.f);
				CObjMgr::GetInstance()->AddObject(CEffectFactory<CEffect, CAnimeEffect>::CreateEffect(m_wstrObjKey.c_str(), L"FennelBuff2",
					Info, FRAME(m_tFrame.fFrameAccel)), OBJECT::EFFECT);
				m_bIsBuff = ON;
			}

			if((int)m_tFrame.fFrame % 2 == 1)
				CSoundMgr::GetInstance()->PlaySound(L"sndFennelBuff.wav", CSoundMgr::MONSTER);

			if (m_tFrame.m_bIsEnd)
			{
				m_eCurStance = IDLE;
				m_eCurAttack = ATTACKSTANCE_END;
				m_fAttackTime = 0.f;
				m_fAttackRandomTime = 0.f;
				m_fDashTime = 0.f;
				m_bIsHit = false;
			}
			break;
		case PLUNGE_IMPACT:
			if (m_wstrStateKey != L"FennelPlunge")
			{
				if (!m_bIsJump)
				{
					m_fPosY = m_tInfo.vPos.y;
					m_fJumpPower = 50.f;
					m_bIsJump = true;
					FrameChange(L"FennelJump", 2.f, -1.f);
					CSoundMgr::GetInstance()->PlaySound(L"sndFennelJump.wav", CSoundMgr::MONSTER);
				}
				if (m_bIsJump)
				{
					float Height = (powf(m_fJumpTime, 2.f) * -GRAVITY * 0.5f) + (m_fJumpTime * m_fJumpPower);
					// 최대 높이
					float MaxHeight = powf(m_fJumpPower, 2.f) / (GRAVITY * 2.f);

					m_tInfo.vPos.x += m_tInfo.vDir.x * 350.f * CTimeMgr::GetInstance()->GetTime();
					m_tInfo.vPos.y = m_fPosY - Height;

					m_fJumpTime += CTimeMgr::GetInstance()->GetTime() * 10.f;

					if (static_cast<int>(MaxHeight) < Height)
					{
						FrameChange(L"FennelPlunge", 1.f, -1.f);
						CObjMgr::GetInstance()->AddObject(CEffectFactory<CEffect, CAnimeEffect>::CreateEffect(m_wstrObjKey.c_str(), L"FennelImpact",
							D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y - 90.f, 0.f),
							FRAME(m_tFrame.fFrameAccel), 170), OBJECT::EFFECT);
						m_fJumpTime = 0.f;
						m_bIsJump = false;
						CScrollMgr::ScreenShakeOn();
						CSoundMgr::GetInstance()->PlaySound(L"sndFennelAttack3.wav", CSoundMgr::MONSTER);
					}
				}
			}
			else
				if (m_tFrame.m_bIsEnd)
				{
					m_eCurStance = IDLE;
					m_eCurAttack = ATTACKSTANCE_END;
				}
			break;
		case BACKFLIP:
			FrameChange(L"FennelBackflip", 1.f, 0.f);
			if((int)m_tFrame.fFrame >= 3 && (int)m_tFrame.fFrame <= 10)
				m_tInfo.vPos.x += m_tInfo.vDir.x * -1.f * 150.f * CTimeMgr::GetInstance()->GetTime();
			if (m_tFrame.m_bIsEnd)
			{
				m_eCurStance = IDLE;
				m_eCurAttack = ATTACKSTANCE_END;
			}
			break;
		}
		break;
	case HURT:
		if ((rand() % 2 + 1) % 2 == 0)
		{
			FrameChange(L"FennelHurt1", 1.f, 0.f);
			m_tFrame.fMax = 0.f;
		}
		else
		{
			FrameChange(L"FennelHurt1", 1.f, 1.f);
			m_tFrame.fFrame = 1.f;
		}

		if (m_bIsHit && m_fInviTime == 0.f)
		{
			m_tInfo.vDir.x = dynamic_cast<CPlayerInfo*>(m_pObserver)->GetInfo().vDir.x;
			m_tStatus.HP -= m_iDamage;
			CDataSubject::GetInstance()->Notify(OBSERVER::BOSS_STATUS, &m_tStatus);
		}

		m_fAttackTime += CTimeMgr::GetInstance()->GetTime();
		m_fInviTime += CTimeMgr::GetInstance()->GetTime();
		m_tInfo.vPos.x += m_tInfo.vDir.x * 50.f * CTimeMgr::GetInstance()->GetTime();

		if (m_fInviTime > 0.7f)
		{
			m_fInviTime = 0.f;
			m_bIsHit = false;
			m_eCurStance = IDLE;
			m_eCurAttack = ATTACKSTANCE_END;
			m_iDamage = 0;
		}
		break;
	case DEAD:
		FrameChange(L"FennelDeath", 1.f, -1.f);
		
		if (m_iAlpha <= 0)
		{
			m_bIsDead = true;
			CSceneMgr::GetInstance()->SetBoss(FALSE);
			break;
		}

		if (m_tFrame.m_bIsEnd && !CTextMgr::GetInstance()->GetIsTalk())
		{
			if (m_iAlpha > 0)
				--m_iAlpha;
		}
		if (m_iAlpha == 255)
			CTextMgr::GetInstance()->StartTalk(8, 8);
		break;
	}

	float dirX = 1.f;
	if (m_eCurStance == ATTACK || m_eCurStance == IDLE || m_eCurStance == DEAD)
		dirX = -1.f;

	matPreWorld2 = matPreWorld;
	matPreWorld = m_tInfo.matWorld;

	m_tInfo.matWorld = CMath::CalculateMatrix(m_tInfo.vPos + CScrollMgr::GetScroll(), D3DXVECTOR3(m_tInfo.vDir.x * dirX, 1.f, 0.f));

	m_tFrame.fFrame += m_tFrame.fMax * CTimeMgr::GetInstance()->GetTime() * m_tFrame.fFrameAccel;

	if (m_tFrame.fFrame > m_tFrame.fMax)
	{
		m_tFrame.fFrame = m_tFrame.fRestartFrame;
		m_tFrame.m_bIsEnd = true;
	}

	return NO_EVENT;
}

void CFennel::LateUpdate()
{
	if (m_bIsHit && (m_eCurAttack != BUFF))
	{
		if (m_eColliObjType == OBJECT::PLAYER_ATTACK)
		{
			// 피격 이펙트
			if (m_eColliObjType == OBJECT::PLAYER_ATTACK && !m_fInviTime)
			{
				wstring HitStateKey = L"";
				INFO HitInfo = {};

				if (dynamic_cast<CPlayerInfo*>(m_pObserver)->GetInfo().vPos.x > m_tInfo.vPos.x)
					HitInfo.vDir.x = -1.f;
				else
					HitInfo.vDir.x = 1.f;

				HitInfo.vPos.x = m_tInfo.vPos.x + (30.f * HitInfo.vDir.x);
				HitInfo.vPos.y = m_tInfo.vPos.y - 45.f;
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
				CSoundMgr::GetInstance()->PlaySound(L"sndAssAttack.wav", CSoundMgr::EFFECT);
			}

			if (m_eCurStance == IDLE)
			{
				m_eCurStance = HURT;

				if(m_tStatus.HP <= 1)
					CScrollMgr::ScreenShakeOn();
				else
					CScrollMgr::ScreenShakeSmallOn();
			}
			else if(m_eCurStance != IDLE)
			{
				if (!m_fInviTime)
				{
					m_tStatus.HP -= m_iDamage;
					CDataSubject::GetInstance()->Notify(OBSERVER::BOSS_STATUS, &m_tStatus);

					if (m_tStatus.HP <= 0)
						CScrollMgr::ScreenShakeOn();
					else
						CScrollMgr::ScreenShakeSmallOn();
				}
				m_fInviTime += CTimeMgr::GetInstance()->GetTime();
				if (m_fInviTime > 1.f)
				{
					m_fInviTime = 0.f;
					m_iDamage = 0;
					m_bIsHit = false;
				}
			}
		}

		if (m_eCurAttack != BACKFLIP)
		{
			if (m_eColliObjType == OBJECT::ARROW)
			{
				if (!m_fInviTime)
				{
					m_tStatus.HP -= m_iDamage;
					m_iDamage = 0;
					CDataSubject::GetInstance()->Notify(OBSERVER::BOSS_STATUS, &m_tStatus);

					if (m_tStatus.HP <= 0)
						CScrollMgr::ScreenShakeOn();
				}
				m_fInviTime += CTimeMgr::GetInstance()->GetTime();
				if (m_fInviTime > 0.1f)
				{
					m_fInviTime = 0.f;
					m_bIsHit = false;
				}
			}
		}
	}

	if (m_tStatus.HP <= 0)
	{
		if (m_eCurStance != DEAD)
		{
			CSoundMgr::GetInstance()->PlaySound(L"sndFennelDeathscream.wav", CSoundMgr::MONSTER);
			CSoundMgr::GetInstance()->StopSound(CSoundMgr::BGM);
		}

		m_eCurStance = DEAD;
		m_eCurAttack = ATTACKSTANCE_END;
		m_bIsBuff = OFF;
		CSceneMgr::GetInstance()->SetBossfight(FALSE);
	}
	else if (m_tStatus.HP <= (m_tStatus.MaxHP * 0.5))
	{
		if (!m_bIsBuff)
		{
			m_eCurStance = ATTACK;
			m_eCurAttack = BUFF;
			m_bIsHit = false;
		}
	}

	SetAttackColliRect();
}

void CFennel::Render()
{
	CObj::UpdateRect();
	CObj::UpdateAttackRect();

	if(m_bIsBuff || m_eCurAttack == DASH_ATTACK || m_wstrStateKey == L"FennelJump")
		AfterImageRender();

	const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
		m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), (int)m_tFrame.fFrame);

	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
		D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

	////////////////////////////////////////////////////// Text

	CTextMgr::GetInstance()->TalkText(m_wstrObjKey, D3DXVECTOR3(m_tInfo.vPos.x - 150.f, m_tInfo.vPos.y - 180.f, 0.f) + CScrollMgr::GetScroll());

	//////////////////////////////////////////////////////

	/*TCHAR szIndex[MIN_STR] = L"";
	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + CScrollMgr::GetScroll().x, m_tInfo.vPos.y + CScrollMgr::GetScroll().y + 100.f, 0.f);
	swprintf_s(szIndex, L"%d / %d", m_tStatus.HP, m_tStatus.MaxHP);
	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite(),
		szIndex, lstrlen(szIndex), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 0));*/

	if (CObjMgr::GetInstance()->GetPlayer()->GetTestView())
	{
		RenderRect();
		RenderAttackRect();
	}
}

void CFennel::AfterImageRender()
{
	int frame = (int)m_tFrame.fFrame;
	if (frame - 1 < 0)
		frame = 0;
	else
		frame -= 1;

	int frame2 = frame;
	if (frame2 - 1 < 0)
		frame2 = 0;
	else
		frame2 -= 1;

	const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
		m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), frame2);

	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	if (m_eCurAttack == DASH_ATTACK)
	{
		D3DXMATRIX matWorld, matTrans;
		matWorld = matPreWorld2;
		D3DXMatrixTranslation(&matTrans, 20.f, 0.f, 0.f);
		matPreWorld2 = matTrans * matWorld;
	}

	CDevice::GetInstance()->GetSprite()->SetTransform(&matPreWorld2);
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
		D3DCOLOR_ARGB(120, 255, 255, 255));

	pTexInfo = CTextureMgr::GetInstance()->GetTexture(
		m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), frame);

	NULL_CHECK(pTexInfo);

	fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	if (m_eCurAttack == DASH_ATTACK || m_wstrStateKey == L"FennelJump")
	{
		D3DXMATRIX matWorld, matTrans;
		matWorld = matPreWorld;
		D3DXMatrixTranslation(&matTrans, 15.f, 0.f, 0.f);
		matPreWorld = matTrans * matWorld;
	}

	CDevice::GetInstance()->GetSprite()->SetTransform(&matPreWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
		D3DCOLOR_ARGB(180, 255, 255, 255));
}

void CFennel::RenderRect()
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
}

void CFennel::RenderAttackRect()
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

void CFennel::Release()
{
	SafeDeleteObserver<CDataSubject>(m_pObserver);
}

void CFennel::Falling()
{
	m_tInfo.vPos.y += m_fFallSpeed;

	// 지속적으로 중력을 받음
	m_fFallSpeed += (GRAVITY * 0.3f) * CTimeMgr::GetInstance()->GetTime();
}

void CFennel::SetAttackColliRect()
{
	AttackColliClear();

	int Frame = static_cast<int>(m_tFrame.fFrame);

	if (m_wstrStateKey == L"FennelAttack")
	{
		if (m_bIsBuff)
			m_tStatus.Attack = 6;
		else
			m_tStatus.Attack = 4;
		switch (Frame)
		{
		case 6:
			m_vAttackSize.push_back(D3DXVECTOR2(190.f, 88.f));
			m_vAttackModify.push_back(D3DXVECTOR2(0.f, 0.f));
			break;
		case 7:
			m_vAttackSize.push_back(D3DXVECTOR2(196.f, 58.f));
			m_vAttackModify.push_back(D3DXVECTOR2(0.f, 0.f));
			break;
		case 15:
			m_vAttackSize.push_back(D3DXVECTOR2(160.f, 90.f));
			m_vAttackModify.push_back(D3DXVECTOR2(0.f, 0.f));
			break;
		case 16:
			m_vAttackSize.push_back(D3DXVECTOR2(166.f, 64.f));
			m_vAttackModify.push_back(D3DXVECTOR2(0.f, 0.f));
			break;
		default:
			m_vAttackRect.clear();
			m_vAttackModify.clear();
			m_vAttackSize.clear();
			break;
		}
	}
	else if (m_wstrStateKey == L"FennelAttackAir")
	{
		if (m_bIsBuff)
			m_tStatus.Attack = 11;
		else
			m_tStatus.Attack = 7;
		switch (Frame)
		{
		case 1:
			m_vAttackSize.push_back(D3DXVECTOR2(84.f, 86.f));
			m_vAttackModify.push_back(D3DXVECTOR2(0.f, 0.f));
			break;
		case 2:
			m_vAttackSize.push_back(D3DXVECTOR2(86.f, 80.f));
			m_vAttackModify.push_back(D3DXVECTOR2(0.f, 0.f));
			break;
		case 4:
			m_vAttackSize.push_back(D3DXVECTOR2(196.f, 74.f));
			m_vAttackModify.push_back(D3DXVECTOR2(0.f, 0.f));
			break;
		case 5:
			m_vAttackSize.push_back(D3DXVECTOR2(214.f, 110.f));
			m_vAttackModify.push_back(D3DXVECTOR2(0.f, 0.f));
			break;
		case 6:
			m_vAttackSize.push_back(D3DXVECTOR2(188.f, 66.f));
			m_vAttackModify.push_back(D3DXVECTOR2(0.f, 0.f));
			break;
		case 7:
			m_vAttackSize.push_back(D3DXVECTOR2(120.f, 64.f));
			m_vAttackModify.push_back(D3DXVECTOR2(0.f, 0.f));
			break;
		default:
			m_vAttackRect.clear();
			m_vAttackModify.clear();
			m_vAttackSize.clear();
			break;
		}
	}
	else if (m_wstrStateKey == L"FennelSlam")
	{
		if (m_bIsBuff)
			m_tStatus.Attack = 9;
		else
			m_tStatus.Attack = 6;

		switch (Frame)
		{
		case 16:
		case 17:
		case 18:
			for (int i = 0; i < 4; ++i)
			{
				m_vAttackSize.push_back(D3DXVECTOR2(22.f, 454.f));
				m_vAttackModify.push_back(D3DXVECTOR2((m_tInfo.vPos.x - SlamLightingPos[i].x) * m_fLightingDirX, m_tInfo.vPos.y - SlamLightingPos[i].y - 400.f));
			}
			break;
		default:
			m_vAttackRect.clear();
			m_vAttackModify.clear();
			m_vAttackSize.clear();
			break;
		}
	}
	else if (m_wstrStateKey == L"FennelPlunge")
	{
		if (m_bIsBuff)
			m_tStatus.Attack = 13;
		else
			m_tStatus.Attack = 9;
		switch (Frame)
		{
		case 0:
			m_vAttackSize.push_back(D3DXVECTOR2(58.f, 40.f));
			m_vAttackModify.push_back(D3DXVECTOR2(0.f, 10.f));
			break;
		case 1:
			m_vAttackSize.push_back(D3DXVECTOR2(114.f, 68.f));
			m_vAttackModify.push_back(D3DXVECTOR2(0.f, 0.f));

			m_vAttackSize.push_back(D3DXVECTOR2(46.f, 146.f));
			m_vAttackModify.push_back(D3DXVECTOR2(0.f, -42.f));
			break;
		case 2:
			m_vAttackSize.push_back(D3DXVECTOR2(152.f, 54.f));
			m_vAttackModify.push_back(D3DXVECTOR2(0.f, 10.f));

			m_vAttackSize.push_back(D3DXVECTOR2(46.f, 190.f));
			m_vAttackModify.push_back(D3DXVECTOR2(0.f, -62.f));
			break;
		case 3:
			m_vAttackSize.push_back(D3DXVECTOR2(18.f, 132.f));
			m_vAttackModify.push_back(D3DXVECTOR2(0.f, -42.f));
			break;
		default:
			m_vAttackRect.clear();
			m_vAttackModify.clear();
			m_vAttackSize.clear();
			break;
		}
	}
}