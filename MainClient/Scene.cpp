#include "stdafx.h"
#include "Scene.h"


CScene::CScene()
{
}


CScene::~CScene()
{
}

int CScene::MapOutSideProcessing()
{
	CObj* pObj = CObjMgr::GetInstance()->GetPlayer();

	// 좌측 맵 이동
	if (pObj->GetInfo().vPos.x < 0.f) 
	{
		pObj->SetPos(D3DXVECTOR3(0.f, pObj->GetInfo().vPos.y, 0.f));
		CDataSubject::GetInstance()->Notify(OBSERVER::PLAYER_INFO, &pObj->GetInfo());
		if (MapChange(CTerrainMgr::GetInstance()->GetCurTerrain()->GetMapLink().LeftMapID))
		{
			pObj->SetPos(D3DXVECTOR3(float(CTerrainMgr::GetInstance()->GetCurTerrain()->GetTileX() * TILESIZE) - 25.f, pObj->GetInfo().vPos.y, 0.f));
			CDataSubject::GetInstance()->Notify(OBSERVER::PLAYER_INFO, &pObj->GetInfo());
			return YES_CHANGE;
		}
		/*else
		{
			return NO_CHANGE;
		}*/
	}
	// 우측 맵 이동
	if (pObj->GetInfo().vPos.x > float(CTerrainMgr::GetInstance()->GetCurTerrain()->GetTileX() * TILESIZE)) 
	{
		pObj->SetPos(D3DXVECTOR3(float(CTerrainMgr::GetInstance()->GetCurTerrain()->GetTileX() * TILESIZE), pObj->GetInfo().vPos.y, 0.f)); // 맵밖으로 못나가게
		CDataSubject::GetInstance()->Notify(OBSERVER::PLAYER_INFO, &pObj->GetInfo());
		// 맵이 있으면 
		if (MapChange(CTerrainMgr::GetInstance()->GetCurTerrain()->GetMapLink().RightMapID))
		{
			pObj->SetPos(D3DXVECTOR3(25.f, pObj->GetInfo().vPos.y, 0.f)); // y 좌표는 그대로, x 좌표를 불러온 맵 좌측끝으로 이동
			CDataSubject::GetInstance()->Notify(OBSERVER::PLAYER_INFO, &pObj->GetInfo());
			return YES_CHANGE;
		}
		//else // 없으면
		//{
		//	return NO_CHANGE;
		//}
	}
	// 하단 맵 이동
	if (pObj->GetInfo().vPos.y > float(CTerrainMgr::GetInstance()->GetCurTerrain()->GetTileY() * TILESIZE))
	{
		pObj->SetPos(D3DXVECTOR3(pObj->GetInfo().vPos.x, float(CTerrainMgr::GetInstance()->GetCurTerrain()->GetTileY() * TILESIZE), 0.f)); // x 좌표는 그대로, y 좌표를 불러온 맵 상단 맵끝으로 이동
		CDataSubject::GetInstance()->Notify(OBSERVER::PLAYER_INFO, &pObj->GetInfo());
		if (MapChange(CTerrainMgr::GetInstance()->GetCurTerrain()->GetMapLink().DownMapID))
		{
			pObj->SetPos(D3DXVECTOR3(pObj->GetInfo().vPos.x, 25.f, 0.f));
			CDataSubject::GetInstance()->Notify(OBSERVER::PLAYER_INFO, &pObj->GetInfo());
			return YES_CHANGE;
		}
		/*else
		{
			
			return NO_CHANGE;
		}*/
	}
	// 상단 맵 이동
	if (pObj->GetInfo().vPos.y < 0.f)
	{
		pObj->SetPos(D3DXVECTOR3(pObj->GetInfo().vPos.x, 0.f, 0.f));
		CDataSubject::GetInstance()->Notify(OBSERVER::PLAYER_INFO, &pObj->GetInfo());
		if (MapChange(CTerrainMgr::GetInstance()->GetCurTerrain()->GetMapLink().UpMapID))
		{
			pObj->SetPos(D3DXVECTOR3(pObj->GetInfo().vPos.x, float(CTerrainMgr::GetInstance()->GetCurTerrain()->GetTileY() * TILESIZE) - 25.f, 0.f)); // x 좌표는 그대로, y 좌표를 불러온 맵 하단 끝으로 이동
			CDataSubject::GetInstance()->Notify(OBSERVER::PLAYER_INFO, &pObj->GetInfo());
			return YES_CHANGE;
		}
		/*else
		{
			
			return NO_CHANGE;
		}*/
	}

	return NO_CHANGE;
}

bool CScene::MapChange(wstring ID)
{
	vector<CTerrain*> list = CTerrainMgr::GetInstance()->GetMapList();

	// 벡터에 저장된 맵들 중 ID을 검색
	auto iter_find = find_if(list.begin(), list.end(),
		[&ID](CTerrain*& pMap)
	{
		return pMap->GetMapLink().MyMapID == ID;
	});

	// 맵이 없을 시 false 반환
	if (iter_find == list.end())
		return false;

	for (int i = OBJECT::ENEMY; i < OBJECT::END; ++i)
	{
		OBJLST ObjLst = CObjMgr::GetInstance()->GetObjLst(OBJECT::TYPE(i));
		for_each(ObjLst.begin(), ObjLst.end(), SafeDelete<CObj*>);
		CObjMgr::GetInstance()->GetObjLst(OBJECT::TYPE(i)).clear();
	}

	// 맵이 있을시 맵 교체
	CTerrainMgr::GetInstance()->ChangeTerrain((*iter_find));
	(*iter_find)->Initialize();

	// 맵 그리드의 데이터 초기화 진행
	CTerrainMgr::GetInstance()->GreedDataReset();
	// 맵 그리드 설정
	CTerrainMgr::GetInstance()->GreedInit();

	return true;
}