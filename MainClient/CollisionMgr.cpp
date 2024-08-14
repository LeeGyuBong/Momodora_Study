#include "stdafx.h"
#include "CollisionMgr.h"
#include "Obj.h"
#include "Terrain.h"

list<GREED*> CCollisionMgr::pCheckLst;

CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::CollisionRect(OBJECT::TYPE eTypeDst, OBJECT::TYPE eTypeSrc)
{
	RECT rc = {};

	for (auto*& pGreed : pCheckLst)
	{
		for (auto*& pDst : pGreed->ColliObjLst[eTypeDst])
		{
			for (auto*& pSrc : pGreed->ColliObjLst[eTypeSrc])
			{
				if (pDst->GetIsHit() || pSrc->GetIsHit())
					continue;

				if (IntersectRect(&rc, &pDst->GetRect(), &pSrc->GetRect()))
				{
					if(eTypeSrc == OBJECT::MONEY)
						pDst->IsHit(eTypeSrc);
					else
						pDst->IsHit(eTypeSrc, 1);

					if(eTypeDst == OBJECT::ARROW)
						pSrc->IsHit(eTypeDst, 1);
					else
						pSrc->IsHit(eTypeDst);
				}
			}
		}
	}
}

void CCollisionMgr::CollisionRectEX(OBJECT::TYPE eType)
{
	float fMoveX = 0.f, fMoveY = 0.f;

	// 오브젝트가 있는 그리드에서
	for (GREED*& pGreed : pCheckLst)
	{
		// 해당 그리드의 오브젝트와
		for (auto*& pObj : pGreed->ColliObjLst[eType])
		{
			// 해당 그리드의 타일 데이터와
			for (TILE*& pTile : pGreed->ColliTileLst)
			{
				// 충돌하는지 검사
				if (CheckRect(pObj, pTile, fMoveX, fMoveY))
				{
					if (pTile->byOption == Y_BLOCK)
					{
						// 상하 충돌
						if (fMoveX > fMoveY)
						{
							float fX = pObj->GetInfo().vPos.x;
							float fY = pObj->GetInfo().vPos.y;

							// 오브젝트 바닥과 타일 위가 맞다았을 때
							if (pTile->GetPos().y > fY)
							{
								fMoveY *= -1.f;
								pObj->SetFallSpeed(0.f);
							}

							pObj->SetColliFloor(true);

							pObj->SetPos(D3DXVECTOR3(fX, fY + fMoveY, 0.f));
						}
						// 좌우 충돌
						else
						{
							float fX = pObj->GetInfo().vPos.x;
							float fY = pObj->GetInfo().vPos.y;

							if (pTile->GetPos().x > fX)
								fMoveX *= -1.f;

							pObj->SetPos(D3DXVECTOR3(fX + fMoveX, fY, 0.f));
						}
					}
					else if (pTile->byOption == TRAP)
						pObj->IsHit(OBJECT::ENEMY_ATTACK, 999999);
					
				}
			}
		}
	}
}

void CCollisionMgr::CollisionEnemyView()
{
	RECT rc = {};
	OBJLST EnemyLst = CObjMgr::GetInstance()->GetObjLst(OBJECT::ENEMY);

	for (auto*& pSrc : EnemyLst)
	{
		if (CollisionCircleToRect(CObjMgr::GetInstance()->GetPlayer(), pSrc))
			pSrc->SetView(true);
		else
			pSrc->SetView(false);
	}
}

void CCollisionMgr::CollisionFloor(OBJECT::TYPE eType)
{
	for (auto*& pGreed : pCheckLst)
	{
		for (auto*& pObj : pGreed->ColliObjLst[eType])
		{
			for (auto*& pTile : pGreed->ColliTileLst)
			{
				if (CheckRect(pObj, pTile))
					pObj->IsHit(OBJECT::TILE);
			}
		}
	}
}

void CCollisionMgr::CollisionPlayerAttack(OBJECT::TYPE eType)
{
	CObj*& pPlayer = CObjMgr::GetInstance()->GetPlayer();

	for (auto*& pSrc : CObjMgr::GetInstance()->GetObjLst(eType))
	{
		for (auto& AttackRect : pPlayer->GetAttackRect())
		{
			if (IntersectRect(&RECT(), &AttackRect, &pSrc->GetRect()))
				pSrc->IsHit(OBJECT::PLAYER_ATTACK, pPlayer->GetStatus().Attack);
		}
	}
}

void CCollisionMgr::CollisionEnemyAttack(OBJECT::TYPE eType)
{
	for (auto*& pObj : CObjMgr::GetInstance()->GetObjLst(eType))
	{
		for (auto*& pEnemy : CObjMgr::GetInstance()->GetObjLst(OBJECT::ENEMY))
		{
			for (auto& AttackRect : pEnemy->GetAttackRect())
			{
				if (IntersectRect(&RECT(), &pObj->GetRect(), &AttackRect))
					pObj->IsHit(OBJECT::ENEMY_ATTACK, pEnemy->GetStatus().Attack);
			}
		}

		for (auto*& pEnemy : CObjMgr::GetInstance()->GetObjLst(OBJECT::BOSS))
		{
			for (auto& AttackRect : pEnemy->GetAttackRect())
			{
				if (IntersectRect(&RECT(), &pObj->GetRect(), &AttackRect))
					pObj->IsHit(OBJECT::ENEMY_ATTACK, pEnemy->GetStatus().Attack);
			}
		}
	}
}

bool CCollisionMgr::CheckCircle(CObj*& pDst, CObj*& pSrc)
{
	float DstRadius = (pDst->GetViewRect().right - pDst->GetViewRect().left) * 0.5f;
	float SrcRadius = (pSrc->GetViewRect().right - pSrc->GetViewRect().left) * 0.5f;
	float Radius = DstRadius + SrcRadius;

	if (CMath::CalculateDistance(pDst->GetInfo().vPos, pSrc->GetInfo().vPos) < Radius)
		return true;

	return false;
}

bool CCollisionMgr::CheckRect(CObj*& pDst, TILE*& pSrc, float& moveX, float& moveY)
{
	float fSumX = (pDst->GetInfo().vSize.x + pSrc->GetSize().x) * 0.5f;
	float fSumY = (pDst->GetInfo().vSize.y + pSrc->GetSize().y) * 0.5f;

	float fDistX = fabs(pDst->GetInfo().vPos.x + pDst->GetRectModify().x - pSrc->GetPos().x);
	float fDistY = fabs(pDst->GetInfo().vPos.y + pDst->GetRectModify().y - pSrc->GetPos().y);

	if (fSumX > fDistX && fSumY > fDistY)
	{
		moveX = fSumX - fDistX;
		moveY = fSumY - fDistY;

		return true;
	}

	return false;
}

bool CCollisionMgr::CheckRect(CObj*& pDst, TILE*& pSrc)
{
	float fSumX = (pDst->GetInfo().vSize.x + pSrc->GetSize().x) * 0.5f;
	float fSumY = (pDst->GetInfo().vSize.y + pSrc->GetSize().y) * 0.5f;

	float fDistX = fabs(pDst->GetInfo().vPos.x + pDst->GetRectModify().x - pSrc->GetPos().x);
	float fDistY = fabs(pDst->GetInfo().vPos.y + pDst->GetRectModify().y - pSrc->GetPos().y);

	if (fSumX > fDistX && fSumY > fDistY)
		return true;

	return false;
}

bool CCollisionMgr::CollisionCircleToRect(CObj*& pDst, CObj*& pSrc)
{
	float fViewCenterX = pSrc->GetInfo().vPos.x;
	float fViewCenterY = pSrc->GetInfo().vPos.y;
	float fRadius = float(pSrc->GetViewRect().right - pSrc->GetViewRect().left) * 0.5f;

	if (((float)pDst->GetRect().left < fViewCenterX && fViewCenterX < (float)pDst->GetRect().right) ||
		((float)pDst->GetRect().top < fViewCenterY && fViewCenterY < (float)pDst->GetRect().bottom))
	{
		RECT rEX = {
			LONG((float)pDst->GetRect().left - fRadius),
			LONG((float)pDst->GetRect().top - fRadius),
			LONG((float)pDst->GetRect().right + fRadius),
			LONG((float)pDst->GetRect().bottom + fRadius),
		};

		if ((float)rEX.left < fViewCenterX && fViewCenterX < (float)rEX.right &&
			(float)rEX.top < fViewCenterY && fViewCenterY < (float)rEX.bottom)
			return true;
	}
	else
	{
		// 사각형 좌상단 꼭지점과 원 충돌
		if (CCollisionMgr::CollisionCircleToPoint(D3DXVECTOR2((float)pDst->GetRect().left, (float)pDst->GetRect().top), 
			D3DXVECTOR2(fViewCenterX, fViewCenterY), fRadius))
			return true;
		// 사각형 우상단 꼭지점과 원 충돌
		if (CCollisionMgr::CollisionCircleToPoint(D3DXVECTOR2((float)pDst->GetRect().right, (float)pDst->GetRect().top),
			D3DXVECTOR2(fViewCenterX, fViewCenterY), fRadius))
			return true;
		// 사각형 우하단 꼭지점과 원 충돌
		if (CCollisionMgr::CollisionCircleToPoint(D3DXVECTOR2((float)pDst->GetRect().right, (float)pDst->GetRect().bottom),
			D3DXVECTOR2(fViewCenterX, fViewCenterY), fRadius))
			return true;
		// 사각형 좌하단 꼭지점과 원 충돌
		if (CCollisionMgr::CollisionCircleToPoint(D3DXVECTOR2((float)pDst->GetRect().left, (float)pDst->GetRect().bottom),
			D3DXVECTOR2(fViewCenterX, fViewCenterY), fRadius))
			return true;
	}

	return false;
}

bool CCollisionMgr::CollisionCircleToPoint(D3DXVECTOR2 DstPos, D3DXVECTOR2 srcPos, float radius)
{
	float deltaX = DstPos.x - srcPos.x;
	float deltaY = DstPos.y - srcPos.y;

	float Length = sqrtf(powf(deltaX, 2) + powf(deltaY, 2));

	// 중점과 점의 거리가 반지름보다 작으면 충돌
	if (Length < radius)
		return true;

	return false;
}

void CCollisionMgr::CheckTrueGreed()
{
	vector<GREED*> pLst = CTerrainMgr::GetInstance()->GetGreed();

	// 그리드 충돌 여부 초기화
	for_each(pLst.begin(), pLst.end(),
		[](GREED*& pData)
	{
		pData->NotColli();
		pData->ObjLstClear();
	});

	for (int arr = 0; arr < OBJECT::END; ++arr)
	{
		for (auto*& pObj : CObjMgr::GetInstance()->GetObjLst((OBJECT::TYPE)arr))
		{
			// 오브젝트의 중점이 있는 그리드 인덱스 반환
			int index = CMath::CalculateGreedIndex(pObj->GetInfo().vPos);

			// 그리드 사이즈가 홀수이면 인덱스 + 1, 짝수이면 그냥 
			if (GREEDSIZE % 2 == 1)
				index += 1;

			int Greed = -CTerrainMgr::GetInstance()->GetGreedX();

			// 오브젝트가 있는 그리드를 포함 8방향의 그리드만 조사
			for (int y = 0; y < 3; ++y)
			{
				int num = -1;
				for (int x = 0; x < 3; ++x)
				{
					int Check = index;
					Check += Greed + num;
					++num;

					// 범위를 넘어가면 다음 인덱스 검사
					if (Check < 0 || pLst.size() <= (size_t)Check)
						continue;

					// 이미 충돌리스트에 충돌이 되어있다면 (다수의 오브젝트가 한 그리드에 있는 상황)
					if (pLst[Check]->bIsColli == true)
						continue;

					if (IntersectRect(&RECT(), &pObj->GetRect(), &pLst[Check]->rect))
					{
						pLst[Check]->IsColli();
						pCheckLst.push_back(pLst[Check]);
					}
				}
				Greed += CTerrainMgr::GetInstance()->GetGreedX();
			}
		}
	}
}

void CCollisionMgr::GreedTileDataInit()
{
	vector<TILE*>& srcLst = dynamic_cast<CTerrain*>(CTerrainMgr::GetInstance()->GetCurTerrain())->GetTile();

	// 그리드 내 타일 데이터 추가 구문
	for (GREED*& pGreed : pCheckLst)
	{
		// 이미 검사를 했다면 다음 그리드로 넘어간다
		if (!pGreed->ColliTileLst.empty())
			continue;

		for (TILE*& pTile : srcLst)
		{
			// 바닥 타일이인 옵션만 검사
			if (pTile->byOption == N_BLOCK)
				continue;

			RECT rc = { LONG(pTile->GetPos().x - SIZEGAP), LONG(pTile->GetPos().y - SIZEGAP),
				LONG(pTile->GetPos().x + SIZEGAP), LONG(pTile->GetPos().y + SIZEGAP) };

			// 그리드와 충돌시 리스트에 추가
			if (IntersectRect(&RECT(), &rc, &pGreed->rect))
				pGreed->ColliTileLst.push_back(pTile);
		}
	}
}

void CCollisionMgr::GreedObjectDataInit()
{
	for (GREED*& pGreed : pCheckLst)
	{
		for (int i = 0; i < OBJECT::END; ++i)
		{
			OBJLST pLst = CObjMgr::GetInstance()->GetObjLst((OBJECT::TYPE)i);

			for (auto*& pObj : pLst)
			{
				if (IntersectRect(&RECT(), &pObj->GetRect(), &pGreed->rect))
					pGreed->ColliObjLst[(OBJECT::TYPE)i].push_back(pObj);
			}
		}
	}
}

void CCollisionMgr::ColliInteractObj()
{
	CObj* pPlayer = CObjMgr::GetInstance()->GetPlayer();

	for (auto*& pInteract : CTerrainMgr::GetInstance()->GetCurTerrain()->GetInteractItem())
	{
		float gapX = pInteract->vSize.x * 0.5f;
		float gapY = pInteract->vSize.y * 0.5f;

		RECT rc = { LONG(pInteract->vPos.x - gapX), LONG(pInteract->vPos.y - gapY),
			LONG(pInteract->vPos.x + gapX), LONG(pInteract->vPos.y + gapY) };

		if (IntersectRect(&RECT(), &pPlayer->GetRect(), &rc))
		{
			pInteract->CurInteract = true;
			pPlayer->SetInteract(true);
		}
		else
		{
			pInteract->CurInteract = false;
			pPlayer->SetInteract(false);
		}
	}

	for (auto*& pNPC : CObjMgr::GetInstance()->GetObjLst(OBJECT::NPC))
	{
		if (IntersectRect(&RECT(), &pPlayer->GetRect(), &pNPC->GetRect()))
		{
			pPlayer->SetInteract(true);
			pNPC->SetInteract(true);
		}
		else
		{
			pPlayer->SetInteract(false);
			pNPC->SetInteract(false);
		}
	}
	
}