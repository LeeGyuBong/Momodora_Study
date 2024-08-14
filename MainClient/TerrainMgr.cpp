#include "stdafx.h"
#include "TerrainMgr.h"
#include "Terrain.h"

IMPLEMENT_SINGLETON(CTerrainMgr)

CTerrainMgr::CTerrainMgr()
	: m_pTerrain(nullptr), m_iGreedX(0)
{
	LoadMapData();

}


CTerrainMgr::~CTerrainMgr()
{
	Release();
}

void CTerrainMgr::Update()
{
	m_pTerrain->Update();
}

void CTerrainMgr::LateUpdate()
{
	m_pTerrain->LateUpdate();
}

void CTerrainMgr::Release()
{
	for_each(m_MapList.begin(), m_MapList.end(), SafeDelete<CTerrain*>);
	m_MapList.clear();
	m_MapList.shrink_to_fit();

	if (!m_vecGreed.empty())
	{
		for_each(m_vecGreed.begin(), m_vecGreed.end(), SafeDelete<GREED*>);
		m_vecGreed.clear();
		m_vecGreed.shrink_to_fit();
	}
}

void CTerrainMgr::LoadMapData()
{
	if (m_MapList.empty())
	{
		HANDLE hFile = CreateFile(L"../Data/MapData/Map.dat", GENERIC_READ, 0, 0,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			ERR_MSG(L"Load Failed!!!");
			return;
		}

		DWORD dwByte = 0;

		int MapSize = 0, Size = 0;
		int MapTileX = 0, MapTileY = 0;
		TILE pTileData = {};
		RECT pRc = {};
		TCHAR* szMapLink = nullptr;

		ReadFile(hFile, &MapSize, sizeof(int), &dwByte, nullptr);

		for (int index = 0; index < MapSize; ++index)
		{
			m_MapList.push_back(new CTerrain);

			// 맵 X크기 로드 & 대입
			ReadFile(hFile, &MapTileX, sizeof(int), &dwByte, nullptr);
			m_MapList[index]->SetTileX(MapTileX);
			// 맵 Y크기 로드 & 대입
			ReadFile(hFile, &MapTileY, sizeof(int), &dwByte, nullptr);
			m_MapList[index]->SetTileY(MapTileY);

			// Tile 데이터 로드
			ReadFile(hFile, &Size, sizeof(int), &dwByte, nullptr);
			for (int i = 0; i < Size; ++i)
			{
				ReadFile(hFile, &pTileData, sizeof(TILE), &dwByte, nullptr);
				m_MapList[index]->GetTile().push_back(new TILE(pTileData));
			}

			// Front 비 상호작용 오브젝트 데이터 로드
			ReadFile(hFile, &Size, sizeof(int), &dwByte, nullptr);
			for (int i = 0; i < Size; ++i)
			{
				ReadFile(hFile, &pTileData, sizeof(TILE), &dwByte, nullptr);
				m_MapList[index]->GetFrontObj().push_back(new TILE(pTileData));
			}

			// Back 비 상호작용 오브젝트 데이터 로드
			ReadFile(hFile, &Size, sizeof(int), &dwByte, nullptr);
			for (int i = 0; i < Size; ++i)
			{
				ReadFile(hFile, &pTileData, sizeof(TILE), &dwByte, nullptr);
				m_MapList[index]->GetBackObj().push_back(new TILE(pTileData));
			}

			// 상호작용 오브젝트 데이터 로드
			ReadFile(hFile, &Size, sizeof(int), &dwByte, nullptr);
			for (int i = 0; i < Size; ++i)
			{
				ReadFile(hFile, &pTileData, sizeof(TILE), &dwByte, nullptr);
				m_MapList[index]->GetInteractObj().push_back(new TILE(pTileData));
			}

			// Map ID
			ReadFile(hFile, &Size, sizeof(int), &dwByte, nullptr);

			szMapLink = new TCHAR[Size];

			ReadFile(hFile, szMapLink, sizeof(TCHAR) * Size, &dwByte, nullptr);
			m_MapList[index]->GetMapLink().MyMapID = szMapLink;

			delete[] szMapLink;
			szMapLink = nullptr;

			// Map Up Link ID
			ReadFile(hFile, &Size, sizeof(int), &dwByte, nullptr);

			szMapLink = new TCHAR[Size];

			ReadFile(hFile, szMapLink, sizeof(TCHAR) * Size, &dwByte, nullptr);
			m_MapList[index]->GetMapLink().UpMapID = szMapLink;

			delete[] szMapLink;
			szMapLink = nullptr;

			// Map Right Link ID
			ReadFile(hFile, &Size, sizeof(int), &dwByte, nullptr);

			szMapLink = new TCHAR[Size];

			ReadFile(hFile, szMapLink, sizeof(TCHAR) * Size, &dwByte, nullptr);
			m_MapList[index]->GetMapLink().RightMapID = szMapLink;

			delete[] szMapLink;
			szMapLink = nullptr;

			// Map Down Link ID
			ReadFile(hFile, &Size, sizeof(int), &dwByte, nullptr);

			szMapLink = new TCHAR[Size];

			ReadFile(hFile, szMapLink, sizeof(TCHAR) * Size, &dwByte, nullptr);
			m_MapList[index]->GetMapLink().DownMapID = szMapLink;

			delete[] szMapLink;
			szMapLink = nullptr;

			// Map Left Link ID
			ReadFile(hFile, &Size, sizeof(int), &dwByte, nullptr);

			szMapLink = new TCHAR[Size];

			ReadFile(hFile, szMapLink, sizeof(TCHAR) * Size, &dwByte, nullptr);
			m_MapList[index]->GetMapLink().LeftMapID = szMapLink;

			delete[] szMapLink;
			szMapLink = nullptr;
		}

	
		// 몬스터 데이터 로드
		hFile = CreateFile(L"../Data/MapData/MapMonster.dat", GENERIC_READ, 0, 0,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		TCHAR* szID = nullptr; int length = 0;
		MONSTER* pMonster = nullptr;

		ReadFile(hFile, &MapSize, sizeof(int), &dwByte, nullptr);
		for (int index = 0; index < MapSize; ++index)
		{
			ReadFile(hFile, &Size, sizeof(int), &dwByte, nullptr);
			for (int i = 0; i < Size; ++i)
			{
				pMonster = new MONSTER;

				ReadFile(hFile, &length, sizeof(int), &dwByte, nullptr);
				szID = new TCHAR[length];
				ReadFile(hFile, szID, sizeof(TCHAR) * length, &dwByte, nullptr);

				pMonster->strID = szID;

				delete[] szID; szID = nullptr;

				ReadFile(hFile, &pMonster->vPos, sizeof(D3DXVECTOR3), &dwByte, nullptr);
				ReadFile(hFile, &pMonster->HP, sizeof(int), &dwByte, nullptr);
				ReadFile(hFile, &pMonster->byDrawID, sizeof(BYTE), &dwByte, nullptr);

				m_MapList[index]->GetMonsterData().push_back(pMonster);
			}
		}

		CloseHandle(hFile);
	}
}

void CTerrainMgr::GreedInit()
{
	if (!m_vecGreed.empty())
	{
		for_each(m_vecGreed.begin(), m_vecGreed.end(), SafeDelete<GREED*>);
		m_vecGreed.clear();
		m_vecGreed.shrink_to_fit();
	}

	m_iGreedX = (m_pTerrain->GetTileX() * TILESIZE) / GREEDSIZE;
	float SizeGap = GREEDSIZE * 0.5f;
	float Width = static_cast<float>(m_pTerrain->GetTileX() * TILESIZE) / GREEDSIZE;
	float Height = static_cast<float>(m_pTerrain->GetTileY() * TILESIZE) / GREEDSIZE;

	for (int i = 0; i < Height; ++i)
	{
		for (int j = 0; j < Width; ++j)
		{
			float x = j * GREEDSIZE + (SizeGap);
			float y = i * GREEDSIZE + (SizeGap);

			RECT rc = { LONG(x - SizeGap), LONG(y - SizeGap),
				LONG(x + SizeGap), LONG(y + SizeGap) };

			m_vecGreed.push_back(new GREED(rc));
		}
	}
}

void CTerrainMgr::RenderGreed()
{
	D3DXVECTOR2 m_Line[5];
	D3DXMATRIX matWorld, matTrans;
	int i = 0;
	for (auto*& pGreed : m_vecGreed)
	{
		m_Line[0] = { (float)pGreed->rect.left + CScrollMgr::GetScroll().x, (float)pGreed->rect.top + CScrollMgr::GetScroll().y };
		m_Line[1] = { (float)pGreed->rect.right + CScrollMgr::GetScroll().x, (float)pGreed->rect.top + CScrollMgr::GetScroll().y };
		m_Line[2] = { (float)pGreed->rect.right + CScrollMgr::GetScroll().x, (float)pGreed->rect.bottom + CScrollMgr::GetScroll().y };
		m_Line[3] = { (float)pGreed->rect.left + CScrollMgr::GetScroll().x, (float)pGreed->rect.bottom + CScrollMgr::GetScroll().y };
		m_Line[4] = { (float)pGreed->rect.left + CScrollMgr::GetScroll().x, (float)pGreed->rect.top + CScrollMgr::GetScroll().y };

		CDevice::GetInstance()->GetLine()->Begin();
		CDevice::GetInstance()->GetLine()->Draw(m_Line, 5, D3DCOLOR_XRGB(255, 0, 0));
		CDevice::GetInstance()->GetLine()->End();

		////////////////////// TEXT

		int ObjSize = 0;
		for (int i = 0; i < OBJECT::END; ++i)
			ObjSize += pGreed->ColliObjLst[(OBJECT::TYPE)i].size();

		TCHAR szIndex[MIN_STR] = L"";
		swprintf_s(szIndex, L"%d, TILE : %d", i++, pGreed->ColliTileLst.size());

		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixTranslation(&matTrans, (float)pGreed->rect.left + 10.f + CScrollMgr::GetScroll().x,
			(float)pGreed->rect.top + 20.f + CScrollMgr::GetScroll().y, 0.f);
		matWorld = matTrans;
		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
		CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite(),
			szIndex, lstrlen(szIndex), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

		swprintf_s(szIndex, L"OBJ : %d, Coll : %d", ObjSize, pGreed->bIsColli);

		D3DXMatrixTranslation(&matTrans, (float)pGreed->rect.left + 10.f + CScrollMgr::GetScroll().x,
			(float)pGreed->rect.top + 40.f + CScrollMgr::GetScroll().y, 0.f);
		matWorld = matTrans;
		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
		CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite(),
			szIndex, lstrlen(szIndex), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

}

void CTerrainMgr::GreedDataReset()
{
	for_each(m_vecGreed.begin(), m_vecGreed.end(),
		[](GREED*& pData)
	{
		pData->ListReset();
	});
}
