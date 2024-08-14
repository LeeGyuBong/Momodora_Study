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
	// 최초 맵 로딩을 실패시 프로그램 종료
	if (!CScene::MapChange(CTerrainMgr::GetInstance()->GetMapList().front()->GetMapLink().MyMapID))
	{
		ERR_MSG(L"Stage MapLoad Failed!");
		exit(3);
	}

	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CPlayer>::CreateObject(), OBJECT::PLAYER);
	//CObjMgr::GetInstance()->AddObject(CAbstractFactory<CFennel>::CreateObject(), OBJECT::BOSS);
	//CObjMgr::GetInstance()->AddObject(CAbstractFactory<CCath>::CreateObject(), OBJECT::NPC);

	// 몬스터 데이터로 생성
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

	///////////////////////////////////////// 맵 전환 구문
	m_iMapChangeEvent = CScene::MapOutSideProcessing();
	if (m_iMapChangeEvent == YES_CHANGE)
	{
		CSceneMgr::GetInstance()->SetBoss(false);
		m_bIsBlend = true;
		m_iChangeAlpha = 255;

		if (CTerrainMgr::GetInstance()->GetCurTerrain()->GetMapLink().MyMapID == L"Map3")
			CObjMgr::GetInstance()->AddObject(CAbstractFactory<CCath>::CreateObject(), OBJECT::NPC);

		// 맵 전환 시에 전환된 맵의 몬스터 데이터를 받아와서 생성
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

	// 몬스터 맵 밖으로 나가지 않도록 하는 구문
	NotMapOut();

	///////////////////////////////////////// 충돌 구문
	// 충돌할 그리드를 확인 하기전에 무조건 
	// 충돌된 그리드 저장 리스트를 비울것!!
	CoiisionCollect();

	// 대상 LateUpdate
	CObjMgr::GetInstance()->LateUpdate();
	CTerrainMgr::GetInstance()->LateUpdate();
}

void CStage::Render()
{
	CTerrainMgr::GetInstance()->GetCurTerrain()->RenderBackGround();	// 배경
	CTerrainMgr::GetInstance()->GetCurTerrain()->RenderBackObj();		// 뒷 배경용 오브젝트
	CTerrainMgr::GetInstance()->GetCurTerrain()->RenderInteractObj();	// 상호작용 오브젝트

	CObjMgr::GetInstance()->Render();									

	CTerrainMgr::GetInstance()->GetCurTerrain()->RenderTile();			// 바닥 오브젝트

	CObjMgr::GetInstance()->RenderBoss();
	CObjMgr::GetInstance()->RenderEffect();
	CObjMgr::GetInstance()->RenderPlayerAttackEffect();

	CTerrainMgr::GetInstance()->GetCurTerrain()->RenderFrontObj();		// 앞 오브젝트


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
	// 체크전 오브젝트가 있는 그리드를 저장할 리스트 초기화
	CCollisionMgr::GreedLstReset();
	// 오브젝트가 어느 그리드에 있는지 확인
	CCollisionMgr::CheckTrueGreed();
	// 플레이어 및 몬스터가 있는 타일에 대해 한번만 타일 데이터 삽입
	CCollisionMgr::GreedTileDataInit();
	// 충돌체크가 돌아갈 그리드에 있을 오브젝트의 데이터 삽입
	CCollisionMgr::GreedObjectDataInit();

	// 충돌 처리
	// 플레이어 바닥 충돌
	CCollisionMgr::CollisionRectEX(OBJECT::PLAYER);
	// 적 바닥 충돌
	CCollisionMgr::CollisionRectEX(OBJECT::ENEMY);
	CCollisionMgr::CollisionRectEX(OBJECT::BOSS);
	CCollisionMgr::CollisionRectEX(OBJECT::MONEY);
	CCollisionMgr::CollisionRectEX(OBJECT::NPC);
	// 화살 바닥 충돌
	CCollisionMgr::CollisionFloor(OBJECT::ARROW);
	CCollisionMgr::CollisionFloor(OBJECT::POISIONBOMB);

	// 상호작용 아이템 체크
	CCollisionMgr::ColliInteractObj();

	// 시야 범위 충돌
	CCollisionMgr::CollisionEnemyView();

	// 회피중 피격 x
	if (dynamic_cast<CPlayer*>(CObjMgr::GetInstance()->GetPlayer())->GetCurState()->GetStateKey() != L"KahoRoll")
	{
		// 플레이어와 적
		CCollisionMgr::CollisionRect(OBJECT::PLAYER, OBJECT::ENEMY);
		//CCollisionMgr::CollisionRect(OBJECT::PLAYER, OBJECT::BOSS);
		CCollisionMgr::CollisionRect(OBJECT::PLAYER, OBJECT::DAGGER);
		CCollisionMgr::CollisionRect(OBJECT::PLAYER, OBJECT::POISIONBOMB);

		// 플레이어와 적 공격
		CCollisionMgr::CollisionEnemyAttack(OBJECT::PLAYER);
	}
	CCollisionMgr::CollisionRect(OBJECT::PLAYER, OBJECT::MONEY);


	// 화살과 적
	CCollisionMgr::CollisionRect(OBJECT::ARROW, OBJECT::ENEMY);
	CCollisionMgr::CollisionRect(OBJECT::ARROW, OBJECT::BOSS);
	CCollisionMgr::CollisionEnemyAttack(OBJECT::ARROW);
	//CCollisionMgr::CollisionRect(OBJECT::ARROW, OBJECT::POISIONBOMB);

	// 플레이어 공격과 적
	CCollisionMgr::CollisionPlayerAttack(OBJECT::ENEMY);
	CCollisionMgr::CollisionPlayerAttack(OBJECT::BOSS);
	// 플레이어 공격과 적 공격 (단검같은 것)
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