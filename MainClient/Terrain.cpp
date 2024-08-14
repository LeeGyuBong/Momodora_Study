#include "stdafx.h"
#include "Terrain.h"

CTerrain::CTerrain()
	: m_bIsInit(false)
{

}

CTerrain::~CTerrain()
{
	Release();
}

void CTerrain::Initialize()
{
	
}

void CTerrain::LateInit()
{
	if (!m_bIsInit)
	{
		for (auto*& pInter : m_vecInteractObj)
		{
			INTERACT* pInteract = new INTERACT;
			pInteract->vPos = pInter->vPos;
			pInteract->vSize = { 60.f, 60.f, 0.f };
			pInteract->ID = Bellflower;

			m_vecInteract.push_back(pInteract);
		}

		m_bIsInit = true;
	}
}

void CTerrain::Update()
{
	LateInit();

	
	
}

void CTerrain::LateUpdate()
{
}

void CTerrain::RenderTile()
{
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);

	D3DXVECTOR3 vScroll = CScrollMgr::GetScroll();

	int iCullX = -(int)vScroll.x / TILESIZE;
	int iCullY = -(int)vScroll.y / TILESIZE;

	int iCullEndX = iCullX + CLIENTWINCX / TILESIZE;
	int iCullEndY = iCullY + CLIENTWINCY / TILESIZE;

	// 맵 타일
	for(int i = iCullY; i < iCullEndY + 2; ++i)
	{
		for (int j = iCullX; j < iCullEndX + 1; ++j)
		{
			int Index = j + (m_iTileX * i);

			if (0 > Index || m_vecTile.size() <= (size_t)Index)
				continue;

			if (m_vecTile[Index]->byOption == N_BLOCK)
				continue;

			matWorld = CMath::CalculateMatrix((m_vecTile[Index]->vPos + CScrollMgr::GetScroll()),
				D3DXVECTOR3(m_vecTile[Index]->ReversalX, m_vecTile[Index]->ReversalY, 0.f));

			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

			float fCenterX = CTextureMgr::GetInstance()->GetTileInfo()[m_vecTile[Index]->byDrawID]->tImgInfo.Width * 0.5f;
			float fCenterY = CTextureMgr::GetInstance()->GetTileInfo()[m_vecTile[Index]->byDrawID]->tImgInfo.Height * 0.5f;

			CDevice::GetInstance()->GetSprite()->Draw(CTextureMgr::GetInstance()->GetTileInfo()[m_vecTile[Index]->byDrawID]->pTexture, nullptr,
				&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
}

void CTerrain::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), SafeDelete<TILE*>);
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();

	for (auto& vecTile : m_vecTileObj)
	{
		for_each(vecTile.begin(), vecTile.end(), SafeDelete<TILE*>);
		vecTile.clear();
		vecTile.shrink_to_fit();
	}

	for_each(m_vecInteractObj.begin(), m_vecInteractObj.end(), SafeDelete<TILE*>);
	m_vecInteractObj.clear();
	m_vecInteractObj.shrink_to_fit();

	for_each(m_vecMonster.begin(), m_vecMonster.end(), SafeDelete<MONSTER*>);
	m_vecMonster.clear();
	m_vecMonster.shrink_to_fit();

	for_each(m_vecInteract.begin(), m_vecInteract.end(), SafeDelete<INTERACT*>);
	m_vecInteract.clear();
	m_vecInteract.shrink_to_fit();
}

void CTerrain::RenderBackGround()
{
	// 1스테이지 배경
	if (m_tLink.MyMapID == L"Map0" || m_tLink.MyMapID == L"Map1" || m_tLink.MyMapID == L"Map2")
	{
		D3DXMATRIX matWorld;
		D3DXMatrixIdentity(&matWorld);

		const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetBackGroundInfo()[0];
		for (int i = 0; i <= (m_iTileX * TILESIZE / (pTexInfo->tImgInfo.Width * 2.f)); ++i)
		{
			D3DXVECTOR3 vPos = { (float)pTexInfo->tImgInfo.Width * 0.5f + float(i * pTexInfo->tImgInfo.Width * 2.f),
				float(m_iTileY * TILESIZE) * 0.5f - 235.f,
				0.f };
			matWorld = CMath::CalculateMatrix(vPos + CScrollMgr::GetScroll(), D3DXVECTOR3(2.f, 2.f, 0.f));
			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
			float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
			float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;
			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
				&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		pTexInfo = CTextureMgr::GetInstance()->GetBackGroundInfo()[1];
		for (int i = 0; i <= (m_iTileX * TILESIZE / (pTexInfo->tImgInfo.Width * 2.f)); ++i)
		{
			D3DXVECTOR3 vPos = { (float)pTexInfo->tImgInfo.Width * 0.5f + float(i * pTexInfo->tImgInfo.Width * 2.f),
				float(m_iTileY * TILESIZE) * 0.5f,
				0.f };
			matWorld = CMath::CalculateMatrix(vPos + CScrollMgr::GetScroll(), D3DXVECTOR3(2.f, 2.f, 0.f));
			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
			float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
			float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;
			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
				&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		pTexInfo = CTextureMgr::GetInstance()->GetBackGroundInfo()[2];
		for (int i = 0; i <= (m_iTileX * TILESIZE / (pTexInfo->tImgInfo.Width * 2.f)); ++i)
		{
			D3DXVECTOR3 vPos = { (float)pTexInfo->tImgInfo.Width * 0.5f + float(i * pTexInfo->tImgInfo.Width * 2.f),
				float(m_iTileY * TILESIZE) * 0.5f - 115.f,
				0.f };
			matWorld = CMath::CalculateMatrix(vPos + CScrollMgr::GetScroll(), D3DXVECTOR3(2.f, 3.f, 0.f));
			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
			float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
			float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;
			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
				&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		pTexInfo = CTextureMgr::GetInstance()->GetBackGroundInfo()[3];
		for (int i = 0; i <= (m_iTileX * TILESIZE / (pTexInfo->tImgInfo.Width * 2.f)); ++i)
		{
			D3DXVECTOR3 vPos = { (float)pTexInfo->tImgInfo.Width * 0.5f + float(i * pTexInfo->tImgInfo.Width * 2.f),
				float(m_iTileY * TILESIZE) * 0.5f + 130.f,
				0.f };
			matWorld = CMath::CalculateMatrix(vPos + CScrollMgr::GetScroll(), D3DXVECTOR3(2.f, 2.f, 0.f));
			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
			float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
			float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;
			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
				&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
	if (m_tLink.MyMapID == L"Map3")
	{
		D3DXMATRIX matWorld;
		D3DXMatrixIdentity(&matWorld);

		const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"BackGround", L"2Stage", 2);
		for (int i = 0; i <= (m_iTileX * TILESIZE / (pTexInfo->tImgInfo.Width * 2.f)); ++i)
		{
			if (i == 0)
			{
				pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"BackGround", L"2Stage", 0);
				D3DXVECTOR3 vPos = { (float)pTexInfo->tImgInfo.Width * 0.5f + float(i * pTexInfo->tImgInfo.Width * 2.f),
					float(m_iTileY * TILESIZE) * 0.5f - 130.f,
					0.f };
				matWorld = CMath::CalculateMatrix(vPos + CScrollMgr::GetScroll(), D3DXVECTOR3(2.f, 2.f, 0.f));
				CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
				float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
				float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;
				CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
					&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
					D3DCOLOR_ARGB(255, 255, 255, 255));
			}
			else
			{
				pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"BackGround", L"2Stage", 2);
				D3DXVECTOR3 vPos = { (float)pTexInfo->tImgInfo.Width * 0.5f + float(i * pTexInfo->tImgInfo.Width * 2.f),
					float(m_iTileY * TILESIZE) * 0.5f - 130.f,
					0.f };
				matWorld = CMath::CalculateMatrix(vPos + CScrollMgr::GetScroll(), D3DXVECTOR3(2.f, 2.f, 0.f));
				CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
				float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
				float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;
				CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
					&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
					D3DCOLOR_ARGB(255, 255, 255, 255));
			}
		}

		pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"BackGround", L"2Stage", 1);
		for (int i = 0; i <= (m_iTileX * TILESIZE / (pTexInfo->tImgInfo.Width * 2.f)); ++i)
		{
			D3DXVECTOR3 vPos = { (float)pTexInfo->tImgInfo.Width * 0.5f + float(i * pTexInfo->tImgInfo.Width * 2.f),
				float(m_iTileY * TILESIZE) * 0.5f,
				0.f };
			matWorld = CMath::CalculateMatrix(vPos + CScrollMgr::GetScroll(), D3DXVECTOR3(2.f, 2.f, 0.f));
			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
			float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
			float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;
			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
				&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
	if (m_tLink.MyMapID == L"Map4")
	{
		D3DXMATRIX matWorld;
		D3DXMatrixIdentity(&matWorld);

		const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"BackGround", L"3Stage", 0);
		int max = (m_iTileX * TILESIZE) / (pTexInfo->tImgInfo.Width * 2);
		for (int i = 0; i <= max; ++i)
		{
			D3DXVECTOR3 vPos = { (float)pTexInfo->tImgInfo.Width * 0.5f + float(i * pTexInfo->tImgInfo.Width * 2.f),
				float(m_iTileY * TILESIZE) * 0.5f - 100.f,
				0.f };
			matWorld = CMath::CalculateMatrix(vPos + CScrollMgr::GetScroll(), D3DXVECTOR3(2.f, 2.f, 0.f));
			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
			float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
			float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;
			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
				&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
}

void CTerrain::RenderFrontObj()
{
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	//// 전방 비 상호작용 오브젝트
	for (auto*& pTile : m_vecTileObj[0])
	{
		matWorld = CMath::CalculateMatrix((pTile->vPos + CScrollMgr::GetScroll()),
			D3DXVECTOR3(pTile->ReversalX, pTile->ReversalY, 0.f));

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
		
		float fCenterX = CTextureMgr::GetInstance()->GetObjInfo()[pTile->byDrawID]->tImgInfo.Width * 0.5f;
		float fCenterY = CTextureMgr::GetInstance()->GetObjInfo()[pTile->byDrawID]->tImgInfo.Height * 0.5f;

		CDevice::GetInstance()->GetSprite()->Draw(CTextureMgr::GetInstance()->GetObjInfo()[pTile->byDrawID]->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CTerrain::RenderBackObj()
{
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	//// 후방 비 상호작용 오브젝트
	for (auto*& pTile : m_vecTileObj[1])
	{
		matWorld = CMath::CalculateMatrix((pTile->vPos + CScrollMgr::GetScroll()),
			D3DXVECTOR3(pTile->ReversalX, pTile->ReversalY, 0.f));

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

		float fCenterX = CTextureMgr::GetInstance()->GetObjInfo()[pTile->byDrawID]->tImgInfo.Width * 0.5f;
		float fCenterY = CTextureMgr::GetInstance()->GetObjInfo()[pTile->byDrawID]->tImgInfo.Height * 0.5f;
		
		CDevice::GetInstance()->GetSprite()->Draw(CTextureMgr::GetInstance()->GetObjInfo()[pTile->byDrawID]->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CTerrain::RenderInteractObj()
{
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	//// 상호작용 오브젝트
	for (auto*& pTile : m_vecInteractObj)
	{
		matWorld = CMath::CalculateMatrix((pTile->vPos + CScrollMgr::GetScroll()),
			D3DXVECTOR3(pTile->ReversalX, pTile->ReversalY, 0.f));

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
		
		float fCenterX = CTextureMgr::GetInstance()->GetInteractInfo()[pTile->byDrawID]->tImgInfo.Width * 0.5f;
		float fCenterY = CTextureMgr::GetInstance()->GetInteractInfo()[pTile->byDrawID]->tImgInfo.Height * 0.5f;

		CDevice::GetInstance()->GetSprite()->Draw(CTextureMgr::GetInstance()->GetInteractInfo()[pTile->byDrawID]->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	for (auto*& pInteract : m_vecInteract)
	{
		if (pInteract->CurInteract)
		{
			const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"UI", L"Menu", 9);
			NULL_CHECK(pTexInfo);

			float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
			float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

			matWorld = CMath::CalculateMatrix(D3DXVECTOR3(pInteract->vPos.x + CScrollMgr::GetScroll().x + 20.f, pInteract->vPos.y + CScrollMgr::GetScroll().y - 60.f, 0.f));
			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
				&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
}

void CTerrain::InteractDataSync(INTERACT* pInter)
{
	// 상호작용 아이템과 상호작용 렌더링용 타일 정보 동기화
	
	auto iter_find = find_if(m_vecInteractObj.begin(), m_vecInteractObj.end(),
		[&pInter](auto*& pInterTile)
	{
		return (pInter->vPos == pInterTile->vPos);
	});

	if (iter_find == m_vecInteractObj.end())
		return;

	vector<TILE*>::iterator iter_begin = m_vecInteractObj.begin();
	vector<TILE*>::iterator iter_end = m_vecInteractObj.end();
	for (; iter_begin != iter_end;)
	{
		if ((*iter_begin)->vPos == (*iter_find)->vPos)
		{
			TILE* pTile = *iter_begin;
			iter_begin = m_vecInteractObj.erase(iter_begin);
			SafeDelete(pTile);
			break;
		}
		else
			++iter_begin;
	}
}