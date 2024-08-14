#include "stdafx.h"
#include "Stage.h"

#include "Effect.h"
#include "NonAnimeEffect.h"

#include "Player.h"
#include "PlayerIMP.h"

#include "NomalImp.h"
#include "ShieldImp.h"
#include "Fennel.h"

#include "Cath.h"

CStage::CStage()
	: m_iMapChangeEvent(true), m_iChangeAlpha(255), m_bIsBlend(false),
	m_bIsBossLock(false), m_fCreateTime(0.f), m_iDeadAlpha(0)
{
}


CStage::~CStage()
{
	Release();
}

void CStage::Initialize()
{
	// ���� �� �ε��� ���н� ���α׷� ����
	if (!CScene::MapChange(CTerrainMgr::GetInstance()->GetMapList().front()->GetMapLink().MyMapID))
	{
		ERR_MSG(L"Stage MapLoad Failed!");
		exit(3);
	}

	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CPlayer>::CreateObject(), OBJECT::PLAYER);
	//CObjMgr::GetInstance()->AddObject(CAbstractFactory<CFennel>::CreateObject(), OBJECT::BOSS);
	//CObjMgr::GetInstance()->AddObject(CAbstractFactory<CCath>::CreateObject(), OBJECT::NPC);

	// ���� �����ͷ� ����
	for (auto*& pMonsterData : CTerrainMgr::GetInstance()->GetCurTerrain()->GetMonsterData())
	{
		if(pMonsterData->strID == L"ID_OO1")
			CObjMgr::GetInstance()->AddObject(CAbstractFactory<CNomalImp>::CreateObject(pMonsterData->vPos), OBJECT::ENEMY);
		if (pMonsterData->strID == L"ID_OO2")
			CObjMgr::GetInstance()->AddObject(CAbstractFactory<CShieldImp>::CreateObject(pMonsterData->vPos), OBJECT::ENEMY);
		if (pMonsterData->strID == L"ID_OO3")
			CObjMgr::GetInstance()->AddObject(CAbstractFactory<CFennel>::CreateObject(pMonsterData->vPos), OBJECT::BOSS);
	}

	CSoundMgr::GetInstance()->StopSound(CSoundMgr::BGM);
	CSoundMgr::GetInstance()->PlayBGM(L"Hallowed_Ground.mp3");
}

void CStage::Update()
{
	CObjMgr::GetInstance()->Update();
	CTerrainMgr::GetInstance()->Update();
}

void CStage::LateUpdate()
{
	CScrollMgr::ScrollInit();
	CScrollMgr::ScrollLock();
	
	if (CSceneMgr::GetInstance()->GetBoss())
		BossFight();
	else
	{
		CScrollMgr::ReturnScroll();
		m_bIsBossLock = false;
	}

	CScrollMgr::ScrollShake();

	///////////////////////////////////////// �� ��ȯ ����
	m_iMapChangeEvent = CScene::MapOutSideProcessing();
	if (m_iMapChangeEvent == YES_CHANGE)
	{
		CSceneMgr::GetInstance()->SetBoss(false);
		m_bIsBlend = true;
		m_iChangeAlpha = 255;

		if (CTerrainMgr::GetInstance()->GetCurTerrain()->GetMapLink().MyMapID == L"Map3")
			CObjMgr::GetInstance()->AddObject(CAbstractFactory<CCath>::CreateObject(), OBJECT::NPC);

		// �� ��ȯ �ÿ� ��ȯ�� ���� ���� �����͸� �޾ƿͼ� ����
		for (auto*& pMonsterData : CTerrainMgr::GetInstance()->GetCurTerrain()->GetMonsterData())
		{
			if (pMonsterData->strID == L"ID_001")
				CObjMgr::GetInstance()->AddObject(CAbstractFactory<CNomalImp>::CreateObject(pMonsterData->vPos, pMonsterData->HP), OBJECT::ENEMY);
			if (pMonsterData->strID == L"ID_002")
				CObjMgr::GetInstance()->AddObject(CAbstractFactory<CShieldImp>::CreateObject(pMonsterData->vPos, pMonsterData->HP), OBJECT::ENEMY);
			if (pMonsterData->strID == L"ID_003")
				CObjMgr::GetInstance()->AddObject(CAbstractFactory<CFennel>::CreateObject(pMonsterData->vPos, pMonsterData->HP), OBJECT::BOSS);
		}
	}

	if (m_bIsBlend)
		--m_iChangeAlpha;

	// ���� �� ������ ������ �ʵ��� �ϴ� ����
	NotMapOut();

	///////////////////////////////////////// �浹 ����
	// �浹�� �׸��带 Ȯ�� �ϱ����� ������ 
	// �浹�� �׸��� ���� ����Ʈ�� ����!!
	CoiisionCollect();

	// ��� LateUpdate
	CObjMgr::GetInstance()->LateUpdate();
	CTerrainMgr::GetInstance()->LateUpdate();
}

void CStage::Render()
{
	CTerrainMgr::GetInstance()->GetCurTerrain()->RenderBackGround();	// ���
	CTerrainMgr::GetInstance()->GetCurTerrain()->RenderBackObj();		// �� ���� ������Ʈ
	CTerrainMgr::GetInstance()->GetCurTerrain()->RenderInteractObj();	// ��ȣ�ۿ� ������Ʈ

	CObjMgr::GetInstance()->Render();									

	CTerrainMgr::GetInstance()->GetCurTerrain()->RenderTile();			// �ٴ� ������Ʈ

	CObjMgr::GetInstance()->RenderBoss();
	CObjMgr::GetInstance()->RenderEffect();
	CObjMgr::GetInstance()->RenderPlayerAttackEffect();

	CTerrainMgr::GetInstance()->GetCurTerrain()->RenderFrontObj();		// �� ������Ʈ


	/////////////////////////////////////////////////////////////////
	if (CObjMgr::GetInstance()->GetPlayer()->GetTestView())
	{
		CDevice::GetInstance()->GetSprite()->End();
		CDevice::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

		CTerrainMgr::GetInstance()->RenderGreed();

		CDevice::GetInstance()->GetSprite()->End();
		CDevice::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	}
	/////////////////////////////////////////////////////////////////
	if (m_bIsBlend)
		RenderChangeBlend();

	if (CObjMgr::GetInstance()->GetPlayer()->GetIsDead())
	{
		DeadRender();
		CObjMgr::GetInstance()->GetPlayer()->Render();

		if (CKeyMgr::GetInstance()->KeyDown(KEY_R))
		{
			CSceneMgr::GetInstance()->SceneChange(CSceneMgr::LOGO);
			CSceneMgr::GetInstance()->SceneChange(CSceneMgr::STAGE);
		}
	}
}

void CStage::Release()
{
	CObjMgr::GetInstance()->DestroyInstance();
	CUiMgr::GetInstance()->DestroyInstance();
	CScrollMgr::Reset();
	CSceneMgr::GetInstance()->SetBoss(false);
	CSceneMgr::GetInstance()->SetBossfight(false);
}

void CStage::RenderChangeBlend()
{
	if (m_iChangeAlpha < 0)
	{
		m_iChangeAlpha = 255;
		m_bIsBlend = false;
		
		return;
	}

	D3DXMATRIX matWorld, matTrans;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixTranslation(&matTrans, CLIENTWINCX * 0.5f, CLIENTWINCY * 0.5f, 0.f);
	matWorld = matTrans;

	const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"BackGround", L"Effect", 0);

	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;
	
	CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		&D3DXVECTOR3(0.f, 0.f, 0.f), D3DCOLOR_ARGB(m_iChangeAlpha, 255, 255, 255));
}

void CStage::CoiisionCollect()
{
	// üũ�� ������Ʈ�� �ִ� �׸��带 ������ ����Ʈ �ʱ�ȭ
	CCollisionMgr::GreedLstReset();
	// ������Ʈ�� ��� �׸��忡 �ִ��� Ȯ��
	CCollisionMgr::CheckTrueGreed();
	// �÷��̾� �� ���Ͱ� �ִ� Ÿ�Ͽ� ���� �ѹ��� Ÿ�� ������ ����
	CCollisionMgr::GreedTileDataInit();
	// �浹üũ�� ���ư� �׸��忡 ���� ������Ʈ�� ������ ����
	CCollisionMgr::GreedObjectDataInit();

	// �浹 ó��
	// �÷��̾� �ٴ� �浹
	CCollisionMgr::CollisionRectEX(OBJECT::PLAYER);
	// �� �ٴ� �浹
	CCollisionMgr::CollisionRectEX(OBJECT::ENEMY);
	CCollisionMgr::CollisionRectEX(OBJECT::BOSS);
	CCollisionMgr::CollisionRectEX(OBJECT::MONEY);
	CCollisionMgr::CollisionRectEX(OBJECT::NPC);
	// ȭ�� �ٴ� �浹
	CCollisionMgr::CollisionFloor(OBJECT::ARROW);
	CCollisionMgr::CollisionFloor(OBJECT::POISIONBOMB);

	// ��ȣ�ۿ� ������ üũ
	CCollisionMgr::ColliInteractObj();

	// �þ� ���� �浹
	CCollisionMgr::CollisionEnemyView();

	// ȸ���� �ǰ� x
	if (dynamic_cast<CPlayer*>(CObjMgr::GetInstance()->GetPlayer())->GetCurState()->GetStateKey() != L"KahoRoll")
	{
		// �÷��̾�� ��
		CCollisionMgr::CollisionRect(OBJECT::PLAYER, OBJECT::ENEMY);
		//CCollisionMgr::CollisionRect(OBJECT::PLAYER, OBJECT::BOSS);
		CCollisionMgr::CollisionRect(OBJECT::PLAYER, OBJECT::DAGGER);
		CCollisionMgr::CollisionRect(OBJECT::PLAYER, OBJECT::POISIONBOMB);

		// �÷��̾�� �� ����
		CCollisionMgr::CollisionEnemyAttack(OBJECT::PLAYER);
	}
	CCollisionMgr::CollisionRect(OBJECT::PLAYER, OBJECT::MONEY);


	// ȭ��� ��
	CCollisionMgr::CollisionRect(OBJECT::ARROW, OBJECT::ENEMY);
	CCollisionMgr::CollisionRect(OBJECT::ARROW, OBJECT::BOSS);
	CCollisionMgr::CollisionEnemyAttack(OBJECT::ARROW);
	//CCollisionMgr::CollisionRect(OBJECT::ARROW, OBJECT::POISIONBOMB);

	// �÷��̾� ���ݰ� ��
	CCollisionMgr::CollisionPlayerAttack(OBJECT::ENEMY);
	CCollisionMgr::CollisionPlayerAttack(OBJECT::BOSS);
	// �÷��̾� ���ݰ� �� ���� (�ܰ˰��� ��)
	CCollisionMgr::CollisionPlayerAttack(OBJECT::DAGGER);
	CCollisionMgr::CollisionPlayerAttack(OBJECT::POISIONBOMB);
}

void CStage::NotMapOut()
{
	OBJLST MonsterLst = CObjMgr::GetInstance()->GetObjLst(OBJECT::ENEMY);
	for (auto*& pMonster : MonsterLst)
	{
		if (pMonster->GetInfo().vPos.x < 0)
			pMonster->SetPos(D3DXVECTOR3(0.f, pMonster->GetInfo().vPos.y, 0.f));
		else if (pMonster->GetInfo().vPos.x > float(CTerrainMgr::GetInstance()->GetCurTerrain()->GetTileX() * TILESIZE))
			pMonster->SetPos(D3DXVECTOR3(float(CTerrainMgr::GetInstance()->GetCurTerrain()->GetTileX() * TILESIZE), pMonster->GetInfo().vPos.y, 0.f));
	}
}

void CStage::BossFight()
{
	if (!CScrollMgr::GetChangeInit())
	{
		CScrollMgr::ChangeScroll();
		BossScrollPos.x = fabs(CObjMgr::GetInstance()->GetPlayer()->GetInfo().vPos.x -
			CObjMgr::GetInstance()->GetObjLst(OBJECT::BOSS).front()->GetInfo().vPos.x) * 0.5f;
		BossScrollPos.y = fabs(CObjMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y -
			CObjMgr::GetInstance()->GetObjLst(OBJECT::BOSS).front()->GetInfo().vPos.y);	
		BossScrollPos += CObjMgr::GetInstance()->GetPlayer()->GetInfo().vPos;

		MovePos.x = CObjMgr::GetInstance()->GetPlayer()->GetInfo().vPos.x;
		MovePos.y = CObjMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y + fabs(CObjMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y -
			CObjMgr::GetInstance()->GetObjLst(OBJECT::BOSS).front()->GetInfo().vPos.y) - 15.f;
	}

	if (!m_bIsBossLock)
	{
		if (MovePos.x > BossScrollPos.x)
		{
			MovePos = BossScrollPos;
			m_bIsBossLock = true;
		}
		else
			MovePos.x += 100.f * CTimeMgr::GetInstance()->GetTime();
	}

	CScrollMgr::ChangeScrollInit(MovePos);

	CObj* pPlayer = CObjMgr::GetInstance()->GetPlayer();
	CObj* pBoss = CObjMgr::GetInstance()->GetObjLst(OBJECT::BOSS).front();
	
	if (MovePos.x - 350.f > pPlayer->GetInfo().vPos.x)
		pPlayer->SetPos(D3DXVECTOR3(BossScrollPos.x - 350.f, pPlayer->GetInfo().vPos.y, 0.f));
	if(MovePos.x + 350.f < pPlayer->GetInfo().vPos.x)
		pPlayer->SetPos(D3DXVECTOR3(BossScrollPos.x + 350.f, pPlayer->GetInfo().vPos.y, 0.f));

	if (MovePos.x - 350.f > pBoss->GetInfo().vPos.x)
		pBoss->SetPos(D3DXVECTOR3(BossScrollPos.x - 350.f, pBoss->GetInfo().vPos.y, 0.f));
	if (MovePos.x + 350.f < pBoss->GetInfo().vPos.x)
		pBoss->SetPos(D3DXVECTOR3(BossScrollPos.x + 350.f, pBoss->GetInfo().vPos.y, 0.f));

	if (CSceneMgr::GetInstance()->GetBossFight())
	{
		m_fCreateTime += CTimeMgr::GetInstance()->GetTime();
		
		if (m_fCreateTime > 0.05f)
		{
			D3DXVECTOR3 CreatePos1 = MovePos;
			D3DXVECTOR3 CreatePos2 = MovePos;

			CreatePos1.x += float(rand() % 30 - 15) - 385.f;
			CreatePos1.y += float(rand() % 400 - 200) - 180.f;

			CreatePos2.x += float(rand() % 30 - 15) + 385.f;
			CreatePos2.y += float(rand() % 400 - 200) - 180.f;

			CObjMgr::GetInstance()->AddObject(CEffectFactory<CEffect, CNonAnimeEffect>::CreateEffect(L"Effect", L"BossFight", CreatePos1, 0, 150, 1.f, float(rand() % 360)),
				OBJECT::EFFECT);
			CObjMgr::GetInstance()->AddObject(CEffectFactory<CEffect, CNonAnimeEffect>::CreateEffect(L"Effect", L"BossFight", CreatePos2, 0, 150, 1.f, float(rand() % 360)),
				OBJECT::EFFECT);

			m_fCreateTime = 0.f;
		}
	}
}

void CStage::DeadRender()
{
	if (m_iDeadAlpha < 255)
		++m_iDeadAlpha;

	D3DXMATRIX matWorld, matTrans;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixTranslation(&matTrans, CLIENTWINCX * 0.5f, CLIENTWINCY * 0.5f, 0.f);
	matWorld = matTrans;

	const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"BackGround", L"Effect", 0);

	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		&D3DXVECTOR3(0.f, 0.f, 0.f), D3DCOLOR_ARGB(m_iDeadAlpha, 255, 255, 255));
}