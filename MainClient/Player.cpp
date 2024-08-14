#include "stdafx.h"
#include "Player.h"
#include "PlayerBridge.h"

#include "JumpEffect.h"

#include "PlayerHp.h"
#include "ItemBox.h"

#include "ChargeEffect.h"
#include "NonAnimeEffect.h"

#include "Terrain.h"

CPlayer::CPlayer()
	: m_pBridge(nullptr), m_bIsControl(false), iEvent(0), m_fAttackTime(0.f),
	m_bIsJumping(false), m_fJumpPower(0.f), m_fPosY(0.f), m_fJumpTime(0.f), m_fWalkTime(0.f),
	m_fChargeTime(0.f), m_bIsBowCharge(false), m_fEffectTime(0.f), m_fBlendTime(0.f), m_iChargeBlend(0),
	m_bIsSecondJumping(false), m_fInviTime(0.f), m_iInviAlpha(0), m_iMoney(0), m_iItemIndex(0), m_tCurActive(nullptr),
	m_fWalkSoundTime(0.f), m_fChargeSoundTime(0.f), m_iDeadAlpha(0)
{
} 


CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	m_tInfo.vPos = { CLIENTWINCX * 0.5f, 600.f, 0.f };

	m_tInfo.vSize = { 30.f, 70.f, 0.f };
	m_vModify = { 0.f, 13.f };

	m_tInfo.vDir = { 1.f, 0.f, 0.f };

	m_wstrObjKey = L"Player";

	m_tStatus.MaxHP = 50;
	m_tStatus.HP = m_tStatus.MaxHP;

	m_fSpeed = 300.f;
	m_fJumpPower = 35.f;

	m_vecActiveLst.push_back(new ITEM(None));
	//m_vecActiveLst.push_back(new ITEM(CDataMgr::GetInstance()->AddActiveItem(Bellflower, 5)));
	m_vecActiveLst.push_back(new ITEM(CDataMgr::GetInstance()->AddActiveItem(CrystalSeed, 1)));
	m_vecActiveLst.push_back(new ITEM(CDataMgr::GetInstance()->AddActiveItem(CatSphere, 1)));
	m_vecPassiveLst.push_back(new ITEM(None));
	m_vecPassiveLst.push_back(new ITEM(CDataMgr::GetInstance()->AddPassiveItem(AdornedRing)));
	m_vecPassiveLst.push_back(new ITEM(CDataMgr::GetInstance()->AddPassiveItem(AstralCharm)));
	m_vecPassiveLst.push_back(new ITEM(CDataMgr::GetInstance()->AddPassiveItem(DrillingArrows)));
}

void CPlayer::LateInit()
{
	CUiMgr::GetInstance()->Add_UI(CAbstractFactory<CPlayerHp>::CreateObject(), CUiMgr::PLAYER);
	CUiMgr::GetInstance()->Add_UI(CAbstractFactory<CItemBox>::CreateObject(), CUiMgr::PLAYER);

	m_pBridge = new CIdleIMP;

	CDataSubject::GetInstance()->AddData(OBSERVER::PLAYER_INFO, &m_tInfo);
	CDataSubject::GetInstance()->AddData(OBSERVER::PLAYER_STATUS, &m_tStatus);
	CDataSubject::GetInstance()->AddData(OBSERVER::PLAYER_MONEY, &m_iMoney);
}

int CPlayer::Update()
{
	CObj::LateInit();

	if(!m_bIsHit && !CTextMgr::GetInstance()->GetIsTalk() && !m_bIsDead)
		Control();
	if (CTextMgr::GetInstance()->GetIsTalk())
	{
		TalkControl();
		if(m_pBridge->GetStateKey() != L"KahoIdle")
			CreateBridge<CIdleIMP>();
		m_bIsControl = false;
	}

	if(!m_bIsDead)
		Falling();
	else
	{
		if (m_iDeadAlpha < 255)
		{
			m_fInviTime += CTimeMgr::GetInstance()->GetTime();
			if (m_fInviTime > 0.01f)
			{
				++m_iDeadAlpha;
				m_fInviTime = 0.f;
			}
		}
	}

	if (m_bIsJumping)
		Jump();

	// ȸ�ǽ� �����Ÿ� �̵�
	if(m_pBridge->GetStateKey() == L"KahoRoll")
		m_tInfo.vPos += m_tInfo.vDir * 300.f * CTimeMgr::GetInstance()->GetTime();

	// ���ݽ� ��¦ ������
	if (m_pBridge->GetStateKey() == L"KahoAttack1" && (int)m_pBridge->GetFrame().fFrame < 3)
		m_tInfo.vPos += m_tInfo.vDir * 80.f * CTimeMgr::GetInstance()->GetTime();
	else if (m_pBridge->GetStateKey() == L"KahoAttack2" && (int)m_pBridge->GetFrame().fFrame < 3)
		m_tInfo.vPos += m_tInfo.vDir * 90.f * CTimeMgr::GetInstance()->GetTime();
	else if (m_pBridge->GetStateKey() == L"KahoAttack3" && (int)m_pBridge->GetFrame().fFrame < 5)
		m_tInfo.vPos += m_tInfo.vDir * 100.f * CTimeMgr::GetInstance()->GetTime();
	
	if (m_bIsHit)
	{
		if (m_pBridge->GetStateKey() != L"KahoHurt")
			CreateBridge<CHurtIMP>();

		m_tInfo.vPos += (m_tInfo.vDir * -1) * 50.f * CTimeMgr::GetInstance()->GetTime();
	}

	iEvent = m_pBridge->Update();

	if (iEvent == ANIMATION_END)
	{
		if (m_pBridge->GetStateKey() == L"KahoCrouchBow")
			CreateBridge<CCrouchIMP>();
		else
			CreateBridge<CIdleIMP>();
		m_bIsControl = false;
	}
	else if (iEvent == ATTACK_END)
		m_bIsControl = false;

	BowCharge();

	// �÷��̾� ��ġ ��� ���
	D3DXVECTOR3 vPos = m_tInfo.vPos + CScrollMgr::GetScroll();
	m_tInfo.matWorld = CMath::CalculateMatrix(vPos, D3DXVECTOR3(m_tInfo.vDir.x, 1.f, 0.f));
	D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vLook, &m_tInfo.matWorld);

	CDataSubject::GetInstance()->Notify(OBSERVER::PLAYER_INFO, &m_tInfo);
	CDataSubject::GetInstance()->Notify(OBSERVER::PLAYER_STATUS, &m_tStatus);
	CDataSubject::GetInstance()->Notify(OBSERVER::PLAYER_MONEY, &m_iMoney);

	return NO_EVENT;
}

void CPlayer::LateUpdate()
{
	Hit();

	if (m_tStatus.HP <= 0)
	{
		m_bIsDead = true;
		m_tInfo.vSize = {};
		m_vModify = {};
		m_bIsHit = false;
		m_bIsJumping = false;
		if (m_pBridge->GetStateKey() != L"KahoDeath")
		{
			CreateBridge<CDeadIMP>();
			CSoundMgr::GetInstance()->StopSound(CSoundMgr::BGM);
		}
	}


	SetAttackColliRect();

	m_tCurActive = m_tActiveSlot[m_iItemIndex];

	m_pBridge->LateUpdate();
}

void CPlayer::Render()
{
	CObj::UpdateRect();
	CObj::UpdateAttackRect();

	m_pBridge->Render();

	/////////////////////////////////////////////////////////////////////////////
	if (m_bIsColliRectView)
	{
		TCHAR szIndex[MIN_STR] = L"";
		D3DXMATRIX matTrans;
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + CScrollMgr::GetScroll().x, m_tInfo.vPos.y + CScrollMgr::GetScroll().y, 0.f);
		swprintf_s(szIndex, L"%.2f, %.2f", m_tInfo.vPos.x, m_tInfo.vPos.y);
		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
		CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite(),
			szIndex, lstrlen(szIndex), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

		RenderRect();
		RenderAttackRect();
	}

	if (m_bIsDead)
	{
		D3DXMATRIX matWorld;
		matWorld = CMath::CalculateMatrix(D3DXVECTOR3(CLIENTWINCX * 0.5f - 50.f, CLIENTWINCY * 0.5f - 50.f, 0.f));

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
		CDevice::GetInstance()->GetBigFont()->DrawTextW(CDevice::GetInstance()->GetSprite(),
			L"�׾����", lstrlen(L"�׾����"), nullptr, 0, D3DCOLOR_ARGB(m_iDeadAlpha, 255, 255, 255));
	}
}

void CPlayer::RenderRect()
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

void CPlayer::RenderAttackRect()
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

void CPlayer::Release()
{
	SafeDelete(m_pBridge);

	for_each (m_vecActiveLst.begin(), m_vecActiveLst.end(), SafeDelete<ITEM*>);
	m_vecActiveLst.clear();
	m_vecActiveLst.shrink_to_fit();
	for_each(m_vecPassiveLst.begin(), m_vecPassiveLst.end(), SafeDelete<ITEM*>);
	m_vecPassiveLst.clear();
	m_vecPassiveLst.shrink_to_fit();
	for_each(m_vecImprotLst.begin(), m_vecImprotLst.end(), SafeDelete<ITEM*>);
	m_vecImprotLst.clear();
	m_vecImprotLst.shrink_to_fit();

	for (int i = 0; i < 3; ++i)
		SafeDelete(m_tActiveSlot[i]);
	for (int i = 0; i < 2; ++i)
		SafeDelete(m_tPassiveSlot[i]);
	for (int i = 0; i < 2; ++i)
		SafeDelete(m_tImportSlot[i]);
}

void CPlayer::Control()
{
	// ȸ��
	if (CKeyMgr::GetInstance()->KeyDown(KEY_Q) && !m_bIsControl)
	{
		// �������¿��� ������ x
		if (!m_bIsJumping)
		{
			CreateBridge<CRollIMP>();

			m_bIsControl = true;
			CSoundMgr::GetInstance()->PlaySound(L"sndRoll.wav", CSoundMgr::CHANNEL::PLAYER);
		}
	}

	// ����
	if (CKeyMgr::GetInstance()->KeyDown(KEY_S) && !m_bIsControl)
	{
		// ���� ���°� �ƴ� ������ ��
		if (!m_bIsJumping)
		{
			if (iEvent == ATTACK_END && m_pBridge->GetStateKey() == L"KahoAttack2")
			{
				CreateBridge<CAttack3IMP>();
				CSoundMgr::GetInstance()->PlaySound(L"sndLeaf3.wav", CSoundMgr::CHANNEL::PLAYER);
			}
			else if (iEvent == ATTACK_END && m_pBridge->GetStateKey() == L"KahoAttack1")
			{
				CreateBridge<CAttack2IMP>();
				CSoundMgr::GetInstance()->PlaySound(L"sndLeaf2.wav", CSoundMgr::CHANNEL::PLAYER);
			}
			else
			{
				CreateBridge<CAttack1IMP>();
				CSoundMgr::GetInstance()->PlaySound(L"sndLeaf.wav", CSoundMgr::CHANNEL::PLAYER);
			}

			m_fAttackTime = 0.f;
		}
		// ���� ���� ������ ��
		else
		{
			CreateBridge<CJumpAttack>();
			CSoundMgr::GetInstance()->PlaySound(L"sndLeaf3.wav", CSoundMgr::CHANNEL::PLAYER);
		}
		m_bIsControl = true;
	}

	// ���Ÿ� ����
	if (CKeyMgr::GetInstance()->KeyCombine(KEY_DOWN, KEY_D) && !m_bIsControl)
	{
		if (m_pBridge->GetStateKey() != L"KahoCrouchBow")
			CreateBridge<CCrouchBowIMP>();

		CSoundMgr::GetInstance()->PlaySound(L"sndArrow2.wav", CSoundMgr::CHANNEL::PLAYER);

		m_bIsControl = true;
		m_fChargeTime = 0.f;
		m_fEffectTime = 0.f;
		m_fBlendTime = 0.f;
		m_iChargeBlend = 0;
	}
	if (m_pBridge->GetStateKey() != L"KahoCrouchBow" && m_pBridge->GetStateKey() != L"KahoCrouch")
	{
		if (CKeyMgr::GetInstance()->KeyPressingUp(KEY_D) & !m_bIsControl) // ���Ÿ� ����
		{
			if (m_bIsJumping) // ���� ����
				CreateBridge<CJumpBowIMP>();
			else // �׳� ����
				CreateBridge<CShootBowIMP>();

			CSoundMgr::GetInstance()->PlaySound(L"sndArrow.wav", CSoundMgr::CHANNEL::PLAYER);

			m_bIsControl = true;
			m_fChargeTime = 0.f;
			m_fEffectTime = 0.f;
			m_fBlendTime = 0.f;
			m_iChargeBlend = 0;
		}
	}

	// ����
	if (CKeyMgr::GetInstance()->KeyDown(KEY_A)) // �������� �ƴҶ� AŰ /// 1�� ����
	{
		if (!m_bIsJumping) // 1�� ����
		{
			m_bIsJumping = true;
			m_bIsFloorColli = false;

			m_fPosY = m_tInfo.vPos.y;

			CreateBridge<CJumpIMP>();
			CSoundMgr::GetInstance()->PlaySound(L"sndJump.wav", CSoundMgr::CHANNEL::PLAYER);
		}
		else// 2�� ����
		{
			if (!m_bIsSecondJumping)
			{
				m_bIsSecondJumping = true;
			
				m_fJumpTime = 0.f;
				m_fPosY = m_tInfo.vPos.y;

				CreateBridge<CJumpIMP>();

				// 2�� ���� ����Ʈ ����
				int LeafCount = rand() % 6 + 4;
				for (int i = 0; i < LeafCount; ++i)
				{
					float x = static_cast<float>(rand() % 80 - 40);
					float y = static_cast<float>(rand() % 30 - 15);
					CObjMgr::GetInstance()->AddObject(CAbstractFactory<CJumpEffect>::CreateObject(
						D3DXVECTOR3(m_tInfo.vPos.x + x, m_tInfo.vPos.y + 25.f + y, 0.f)),
						OBJECT::EFFECT);
				}
				Jump();
				CSoundMgr::GetInstance()->PlaySound(L"sndJump.wav", CSoundMgr::CHANNEL::PLAYER);
			}
		}
	}

	// ������ �ٲٱ�
	if (CKeyMgr::GetInstance()->KeyDown(KEY_E))
	{
		++m_iItemIndex;
		if (m_iItemIndex >= 3)
			m_iItemIndex = 0;
		CSoundMgr::GetInstance()->PlaySound(L"sndSwitch.wav", CSoundMgr::CHANNEL::PLAYER);
	}

	// ������ ���
	if (CKeyMgr::GetInstance()->KeyDown(KEY_W) && !m_bIsControl && !m_bIsJumping)
	{
		if (m_tCurActive != nullptr)
		{
			if (m_tCurActive->count > 0)
			{
				if (m_pBridge->GetStateKey() != L"KahoItem")
					CreateBridge<CUseItemIMP>();
				ItemUse();
				m_bIsControl = true;
			}
		}
	}

	// UI �ѱ�
	if (CKeyMgr::GetInstance()->KeyDown(KEY_SHIFT))
	{
		CUiMgr::GetInstance()->UI_ON();
		CUiMgr::GetInstance()->UiChanger(CUiMgr::MENU);
		CSoundMgr::GetInstance()->PlaySound(L"sndUIopen.wav", CSoundMgr::EFFECT);
	}


	// ���� �̵�
	if (CKeyMgr::GetInstance()->KeyPressing(KEY_LEFT) & !m_bIsControl)
	{
		m_tInfo.vDir.x = -1.f;

		if (m_pBridge->GetStateKey() != L"KahoCrouch")
		{

			if (m_pBridge->GetStateKey() != L"KahoWalk" && !m_bIsJumping)
				CreateBridge<CMoveIMP>();

			m_tInfo.vPos.x += m_fSpeed * m_tInfo.vDir.x * CTimeMgr::GetInstance()->GetTime();
			m_fWalkTime += CTimeMgr::GetInstance()->GetTime();
			
		}
		if(!m_bIsJumping)
			m_fWalkSoundTime += CTimeMgr::GetInstance()->GetTime();
		if (m_fWalkSoundTime > 0.4f)
		{
			CSoundMgr::GetInstance()->PlaySound(L"sndgrass_footstep.wav", CSoundMgr::CHANNEL::PLAYER);
			m_fWalkSoundTime = 0.f;
		}
	}
	// ���� �̵�
	else if (CKeyMgr::GetInstance()->KeyPressing(KEY_RIGHT) & !m_bIsControl)
	{
		m_tInfo.vDir.x = 1.f;

		if (m_pBridge->GetStateKey() != L"KahoCrouch")
		{
			if (m_pBridge->GetStateKey() != L"KahoWalk" && !m_bIsJumping)
				CreateBridge<CMoveIMP>();

			m_tInfo.vPos.x += m_fSpeed * m_tInfo.vDir.x * CTimeMgr::GetInstance()->GetTime();
			m_fWalkTime += CTimeMgr::GetInstance()->GetTime();
		}
		if (!m_bIsJumping)
			m_fWalkSoundTime += CTimeMgr::GetInstance()->GetTime();
		if (m_fWalkSoundTime > 0.4f)
		{
			CSoundMgr::GetInstance()->PlaySound(L"sndgrass_footstep.wav", CSoundMgr::CHANNEL::PLAYER);
			m_fWalkSoundTime = 0.f;
		}
	}

	// �ɱ�
	if (!m_bIsJumping && m_pBridge->GetStateKey() != L"KahoCrouchBow")
	{
		// �ɱ�
		if (CKeyMgr::GetInstance()->KeyDown(KEY_DOWN))
		{
			if (m_pBridge->GetStateKey() != L"KahoCrouch")
				CreateBridge<CCrouchIMP>();
		}
		// ���� �� �Ͼ��
		else if (CKeyMgr::GetInstance()->KeyUp(KEY_DOWN))
		{
			if (m_pBridge->GetStateKey() == L"KahoCrouch")
				CreateBridge<CRiesIMP>();
		}
	}	

	// ��ȣ�ۿ�
	if (CKeyMgr::GetInstance()->KeyDown(KEY_UP) && m_bIsInteractObj)
	{
		// ������
		vector<INTERACT*>::iterator iter_begin = CTerrainMgr::GetInstance()->GetCurTerrain()->GetInteractItem().begin();
		vector<INTERACT*>::iterator iter_end = CTerrainMgr::GetInstance()->GetCurTerrain()->GetInteractItem().end();
		for (; iter_begin != iter_end;)
		{
			if ((*iter_begin)->CurInteract)
			{
				ITEM* pItem = new ITEM(CDataMgr::GetInstance()->AddActiveItem((*iter_begin)->ID, 5));

				m_vecActiveLst.push_back(pItem);

				INTERACT* pInter = (*iter_begin);

				CTerrainMgr::GetInstance()->GetCurTerrain()->InteractDataSync(*iter_begin);

				CTerrainMgr::GetInstance()->GetCurTerrain()->GetInteractItem().erase(iter_begin);

				SafeDelete(pInter);
				break;
			}
			else
				++iter_begin;
		}

		// NPC
		for (auto*& pNPC : CObjMgr::GetInstance()->GetObjLst(OBJECT::NPC))
		{
			if (pNPC->GetIsInteract())
			{
				CTextMgr::GetInstance()->StartTalk(0,11);
				break;
			}
		}
	}

	// ����Ű �Է��� ���� ��
	if ((CKeyMgr::GetInstance()->GetKeyState() & KEY_LEFT) != KEY_LEFT &&
		(CKeyMgr::GetInstance()->GetKeyState() & KEY_RIGHT) != KEY_RIGHT)
	{
		if (m_pBridge->GetStateKey() == L"KahoWalk" && m_fWalkTime > 0.3f)
		{
			CreateBridge<CBrakeIMP>();
			m_fWalkTime = 0.f;
			m_fWalkSoundTime = 0.f;
		}
		else if (m_pBridge->GetStateKey() == L"KahoWalk")
		{
			CreateBridge<CIdleIMP>();
			m_fWalkTime = 0.f;
			m_fWalkSoundTime = 0.f;
		}

		// ������ ���� ��
		if (iEvent == ATTACK_END)
		{
			// �����ð��� ������
			if (m_fAttackTime > 0.3f)
			{
				// Idle ���·� �Ѿ��
				CreateBridge<CIdleIMP>();
				m_bIsControl = false;
				m_fAttackTime = 0.f;
			}
			// ���� �ð��� �ɶ����� �ð� ���
			m_fAttackTime += CTimeMgr::GetInstance()->GetTime();
		}
	}
		
	if (CKeyMgr::GetInstance()->KeyDown(KEY_V))
	{
		if(m_bIsColliRectView)
			m_bIsColliRectView = false;
		else
			m_bIsColliRectView = true;
	}

	if (CKeyMgr::GetInstance()->KeyDown(KEY_1))
	{
		m_tStatus.MaxHP += 10;
		CDataSubject::GetInstance()->Notify(OBSERVER::PLAYER_STATUS, &m_tStatus);
	}
	if (CKeyMgr::GetInstance()->KeyDown(KEY_2))
	{
		++m_tCurActive->count;
		
	}
}

void CPlayer::TalkControl()
{
	if (CKeyMgr::GetInstance()->KeyDown(KEY_A))
	{
		CTextMgr::GetInstance()->NextText();
	}
}

void CPlayer::Jump()
{
	float Height = (powf(m_fJumpTime, 2.f) * -GRAVITY * 0.5f) + (m_fJumpTime * m_fJumpPower);
	// �ִ� ����
	float MaxHeight = powf(m_fJumpPower, 2.f) / (GRAVITY * 2.f);

	m_tInfo.vPos.y = m_fPosY - Height;
	m_fJumpTime += CTimeMgr::GetInstance()->GetTime() * 10.f;

	// �ִ� ���� ���� �� �������� ���·� ��ȯ
	if ((int)MaxHeight < Height)
	{
		if (m_pBridge->GetStateKey() != L"KahoAttackAir" &&
			m_pBridge->GetStateKey() != L"KahoBowAir")
			CreateBridge<CFallIMP>();
	}

	// �ٴڿ� �浹�� ���� ���� ������ �ʱ�ȭ �� Idle ���·� ��ȯ
	if (m_bIsFloorColli)
	{
		m_bIsJumping = false;
		m_fJumpTime = 0.f;
		m_bIsControl = false;
		m_bIsSecondJumping = false;

		CreateBridge<CIdleIMP>();
	}
}

void CPlayer::Falling()
{
	m_tInfo.vPos.y += m_fFallSpeed;

	// ���� ���°� �ƴϰ� ���� �߷� �ӵ��� �����ϸ�
	if (m_fFallSpeed > 0.3f && !m_bIsJumping)
	{
		// Ư�� ���°� �ƴϰ�
		if (m_pBridge->GetStateKey() != L"KahoAttackAir" && 
			m_pBridge->GetStateKey() != L"KahoBowAir")
		{
			// �������� ���°� �ƴϸ� �������� ���·� ��ȯ�Ѵ�
			if (m_pBridge->GetStateKey() != L"KahoFall")
				CreateBridge<CFallIMP>();
		}
	}
	// ���� ���°� �ƴϰ� ���� �������� ��
	else if (m_fFallSpeed < 0.1f && !m_bIsJumping)
	{
		// �������� ���¿��ٸ� IDLE ���·� ��ȯ�Ѵ�
		if (m_pBridge->GetStateKey() == L"KahoFall")
		{
			CreateBridge<CIdleIMP>();
			m_bIsControl = false;
		}
	}

	// ���������� �߷��� ����
	m_fFallSpeed += (GRAVITY * 0.3f) * CTimeMgr::GetInstance()->GetTime();
}

void CPlayer::BowCharge()
{
	if (CKeyMgr::GetInstance()->KeyPressing(KEY_D) && CKeyMgr::GetInstance()->GetPressingUpTime())
		m_fChargeTime += CTimeMgr::GetInstance()->GetTime();
	if (m_fChargeTime > 2.f)
	{
		m_bIsBowCharge = true;

		m_fBlendTime += CTimeMgr::GetInstance()->GetTime();

		m_fChargeSoundTime += CTimeMgr::GetInstance()->GetTime();
		if (m_fChargeSoundTime > 0.8f)
		{
			CSoundMgr::GetInstance()->PlaySound(L"sndCharge2.wav", CSoundMgr::PLAYER);
			m_fChargeSoundTime = 0.f;
		}

		if (m_fBlendTime > 0.2f)
		{
			if (m_iChargeBlend)
				m_iChargeBlend = 0;
			else
				m_iChargeBlend = 1;
			m_fBlendTime = 0.f;
		}
	}
	else if (m_fChargeTime > 0.4f)
	{
		float x = static_cast<float>(rand() % 120 - 70);
		float y = static_cast<float>(rand() % 120 - 70);

		m_fEffectTime += CTimeMgr::GetInstance()->GetTime();
		m_fChargeSoundTime += CTimeMgr::GetInstance()->GetTime();
		if (m_fChargeSoundTime > 0.1f)
		{
			CSoundMgr::GetInstance()->PlaySound(L"sndCharge1.wav", CSoundMgr::CHANNEL::PLAYER);
			m_fChargeSoundTime = 0.f;
		}

		if (m_fEffectTime > 0.02f)
		{
			CObjMgr::GetInstance()->AddObject(CEffectFactory<CChargeEffect, CNonAnimeEffect>::
				CreateEffect(m_wstrObjKey.c_str(), L"Effect", D3DXVECTOR3(x + m_tInfo.vPos.x, y + m_tInfo.vPos.y, 0.f), 2, 190, 0.1f),
				OBJECT::EFFECT);
			//CSoundMgr::GetInstance()->PlaySound(L"sndCharge1.wav", CSoundMgr::CHANNEL::EFFECT);
			m_fEffectTime = 0.f;
		}
	}
}

void CPlayer::Hit()
{
	if (m_tStatus.HP <= 0)
	{
		m_iInviAlpha = 0;
		return;
	}

	if (m_eColliObjType == OBJECT::MONEY)
	{
		++m_iMoney;
		CSoundMgr::GetInstance()->PlaySound(L"sndItemUse.wav", CSoundMgr::EFFECT);
		NotHit();
		return;
	}

	if (m_bIsHit && !m_fInviTime)
	{
		m_tStatus.HP -= m_iDamage;
		m_fInviTime += CTimeMgr::GetInstance()->GetTime();
		m_iDamage = 0;
		CSoundMgr::GetInstance()->PlaySound(L"sndKahoHurt.wav", CSoundMgr::PLAYER);
	}
	
	// �ǰݽ� ���ĺ��� �� ���� �ð����� ����
	if (m_fInviTime > 0.f)
	{
		if (m_iInviAlpha)
			m_iInviAlpha = 0;
		else
			m_iInviAlpha = 1;
		m_fInviTime += CTimeMgr::GetInstance()->GetTime();
	}
	if (m_fInviTime > 1.5f)
	{
		m_fInviTime = 0.f;
		m_iInviAlpha = 0;
		m_bIsHit = false;
		CreateBridge<CIdleIMP>();
		m_bIsControl = false;
	}
}

void CPlayer::SetAttackColliRect()
{
	AttackColliClear();

	int Frame = (int)m_pBridge->GetFrame().fFrame;

	if (m_pBridge->GetStateKey() == L"KahoAttack1")
	{
		m_tStatus.Attack = 2;
		switch (Frame)
		{
		case 3:
			m_vAttackSize.push_back(D3DXVECTOR2(116.f, 66.f));
			m_vAttackModify.push_back(D3DXVECTOR2(50.f, 0.f));
			break;
		case 4:
			m_vAttackSize.push_back(D3DXVECTOR2(66.f, 64.f));
			m_vAttackModify.push_back(D3DXVECTOR2(50.f, 0.f));
			break;
		default:
			m_vAttackRect.clear();
			m_vAttackModify.clear();
			m_vAttackSize.clear();
			break;
		}
	}
	else if (m_pBridge->GetStateKey() == L"KahoAttack2")
	{
		m_tStatus.Attack = 3;
		switch (Frame)
		{
		case 3:
			m_vAttackSize.push_back(D3DXVECTOR2(114.f, 80.f));
			m_vAttackModify.push_back(D3DXVECTOR2(50.f, 0.f));
			break;
		case 4:
			m_vAttackSize.push_back(D3DXVECTOR2(72.f, 76.f));
			m_vAttackModify.push_back(D3DXVECTOR2(50.f, 0.f));
			break;
		default:
			m_vAttackRect.clear();
			m_vAttackModify.clear();
			m_vAttackSize.clear();
			break;
		}
	}
	else if (m_pBridge->GetStateKey() == L"KahoAttack3")
	{
		m_tStatus.Attack = 5;
		switch (Frame)
		{
		case 5:
			m_vAttackSize.push_back(D3DXVECTOR2(110.f, 86.f));
			m_vAttackModify.push_back(D3DXVECTOR2(50.f, 0.f));
			break;
		case 6:
			m_vAttackSize.push_back(D3DXVECTOR2(122.f, 88.f));
			m_vAttackModify.push_back(D3DXVECTOR2(50.f, 0.f));
			break;
		default:
			m_vAttackRect.clear();
			m_vAttackModify.clear();
			m_vAttackSize.clear();
			break;
		}
	}
	else if (m_pBridge->GetStateKey() == L"KahoAttackAir")
	{
		m_tStatus.Attack = 4;
		switch (Frame)
		{
		case 2:
			m_vAttackSize.push_back(D3DXVECTOR2(76.f, 96.f));
			m_vAttackModify.push_back(D3DXVECTOR2(50.f, 0.f));
			break;
		case 3:
			m_vAttackSize.push_back(D3DXVECTOR2(62.f, 88.f));
			m_vAttackModify.push_back(D3DXVECTOR2(50.f, 0.f));
			break;
		default:
			m_vAttackRect.clear();
			m_vAttackModify.clear();
			m_vAttackSize.clear();
			break;
		}
	}
}

void CPlayer::ItemUse()
{
	switch (m_tCurActive->ID)
	{
	case Bellflower:
		m_tStatus.HP += 10;
		if (m_tStatus.MaxHP < m_tStatus.HP)
			m_tStatus.HP = m_tStatus.MaxHP;
		CSoundMgr::GetInstance()->PlaySound(L"sndBellflower.wav", CSoundMgr::CHANNEL::EFFECT);
		break;
	}

	--m_tCurActive->count;
	CDataSubject::GetInstance()->Notify(OBSERVER::PLAYER_STATUS, &m_tStatus);
}