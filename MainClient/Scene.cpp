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

	// ���� �� �̵�
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
	// ���� �� �̵�
	if (pObj->GetInfo().vPos.x > float(CTerrainMgr::GetInstance()->GetCurTerrain()->GetTileX() * TILESIZE)) 
	{
		pObj->SetPos(D3DXVECTOR3(float(CTerrainMgr::GetInstance()->GetCurTerrain()->GetTileX() * TILESIZE), pObj->GetInfo().vPos.y, 0.f)); // �ʹ����� ��������
		CDataSubject::GetInstance()->Notify(OBSERVER::PLAYER_INFO, &pObj->GetInfo());
		// ���� ������ 
		if (MapChange(CTerrainMgr::GetInstance()->GetCurTerrain()->GetMapLink().RightMapID))
		{
			pObj->SetPos(D3DXVECTOR3(25.f, pObj->GetInfo().vPos.y, 0.f)); // y ��ǥ�� �״��, x ��ǥ�� �ҷ��� �� ���������� �̵�
			CDataSubject::GetInstance()->Notify(OBSERVER::PLAYER_INFO, &pObj->GetInfo());
			return YES_CHANGE;
		}
		//else // ������
		//{
		//	return NO_CHANGE;
		//}
	}
	// �ϴ� �� �̵�
	if (pObj->GetInfo().vPos.y > float(CTerrainMgr::GetInstance()->GetCurTerrain()->GetTileY() * TILESIZE))
	{
		pObj->SetPos(D3DXVECTOR3(pObj->GetInfo().vPos.x, float(CTerrainMgr::GetInstance()->GetCurTerrain()->GetTileY() * TILESIZE), 0.f)); // x ��ǥ�� �״��, y ��ǥ�� �ҷ��� �� ��� �ʳ����� �̵�
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
	// ��� �� �̵�
	if (pObj->GetInfo().vPos.y < 0.f)
	{
		pObj->SetPos(D3DXVECTOR3(pObj->GetInfo().vPos.x, 0.f, 0.f));
		CDataSubject::GetInstance()->Notify(OBSERVER::PLAYER_INFO, &pObj->GetInfo());
		if (MapChange(CTerrainMgr::GetInstance()->GetCurTerrain()->GetMapLink().UpMapID))
		{
			pObj->SetPos(D3DXVECTOR3(pObj->GetInfo().vPos.x, float(CTerrainMgr::GetInstance()->GetCurTerrain()->GetTileY() * TILESIZE) - 25.f, 0.f)); // x ��ǥ�� �״��, y ��ǥ�� �ҷ��� �� �ϴ� ������ �̵�
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

	// ���Ϳ� ����� �ʵ� �� ID�� �˻�
	auto iter_find = find_if(list.begin(), list.end(),
		[&ID](CTerrain*& pMap)
	{
		return pMap->GetMapLink().MyMapID == ID;
	});

	// ���� ���� �� false ��ȯ
	if (iter_find == list.end())
		return false;

	for (int i = OBJECT::ENEMY; i < OBJECT::END; ++i)
	{
		OBJLST ObjLst = CObjMgr::GetInstance()->GetObjLst(OBJECT::TYPE(i));
		for_each(ObjLst.begin(), ObjLst.end(), SafeDelete<CObj*>);
		CObjMgr::GetInstance()->GetObjLst(OBJECT::TYPE(i)).clear();
	}

	// ���� ������ �� ��ü
	CTerrainMgr::GetInstance()->ChangeTerrain((*iter_find));
	(*iter_find)->Initialize();

	// �� �׸����� ������ �ʱ�ȭ ����
	CTerrainMgr::GetInstance()->GreedDataReset();
	// �� �׸��� ����
	CTerrainMgr::GetInstance()->GreedInit();

	return true;
}