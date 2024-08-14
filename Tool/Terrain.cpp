#include "stdafx.h"
#include "Terrain.h"

#include "MainFrm.h"
#include "ToolView.h"


CTerrain::CTerrain()
	: m_iTileX(0), m_iTileY(0), m_bIsGreed(true), m_bIsTile(true), m_bIsObject(true),
	vPos(0.f, 0.f, 0.f)
{
	m_dwVisible |= VISIBLE_GREED;
	m_dwVisible |= VISIBLE_OBJECT;
	m_dwVisible |= VISIBLE_TILE;
}


CTerrain::~CTerrain()
{
	Release();
}

void CTerrain::Initialize()
{
	for (int i = 0; i < m_iTileY; ++i)
	{
		for (int j = 0; j < m_iTileX; ++j)
		{
			TILE* pTile = new TILE;

			pTile->vPos = { float(j * TILESIZE + SIZEGAP), float(i * TILESIZE + SIZEGAP), 0.f };
			pTile->vSize = { float(TILESIZE), float(TILESIZE), 0.f };
			pTile->byDrawID = -1;
			pTile->byOption = N_BLOCK;

			m_vecTile.push_back(pTile);
		}
	}
}

void CTerrain::Render()
{
	RECT rcWnd = {};
	::GetClientRect(m_pMainView->m_hWnd, &rcWnd);

	float fRatioX = WINCX / float(rcWnd.right - rcWnd.left);
	float fRatioY = WINCY / float(rcWnd.bottom - rcWnd.top);

	D3DXMATRIX matScale, matTrans, matWorld;

	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pView = dynamic_cast<CToolView*>(pMainFrame->m_MainSplit.GetPane(0, 1));
	
	//D3DXVECTOR3 vScroll = { (float)pView->GetScrollPos(0), (float)pView->GetScrollPos(1), 0.f };

	int CullX = pView->GetScrollPos(0) / TILESIZE;
	int CullY = pView->GetScrollPos(1) / TILESIZE;

	int CullEndX = CullX + WINCX / TILESIZE;
	int CullEndY = CullY + WINCY / TILESIZE;

	// --------------------- 타일 렌더링
	if (m_dwVisible & VISIBLE_TILE)
		for (size_t i = 0; i < m_vecTile.size(); ++i)
		{
			if (m_vecTile[i]->byOption == N_BLOCK)
				continue;

			D3DXMatrixIdentity(&matWorld);
			D3DXMatrixScaling(&matScale, m_vecTile[i]->ReversalX * fRatioX * 0.5f
				, m_vecTile[i]->ReversalY * fRatioY * 0.5f, 0.f);
			D3DXMatrixTranslation(&matTrans,
				(m_vecTile[i]->vPos.x - m_pMainView->GetScrollPos(0)) * fRatioX * 0.5f,
				(m_vecTile[i]->vPos.y - m_pMainView->GetScrollPos(1)) * fRatioY * 0.5f,
				m_vecTile[i]->vPos.z);

			matWorld = matScale * matTrans;

			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

			const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
				L"MapTool", L"Tile", m_vecTile[i]->byDrawID);

			if (pTexInfo == nullptr)
				continue;

			float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
			float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
				&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	
	if (m_dwVisible & VISIBLE_OBJECT)
	{
		// 비 상호작용 오브젝트 렌더링 // 0 : Front 비 상호작용 오브젝트, 1 : Back 비 상호작용 오브젝트
		for (int j = 1; j >= 0; --j)
			for (size_t i = 0; i < m_vecTileObj[j].size(); ++i)
			{
				D3DXMatrixIdentity(&matWorld);
				D3DXMatrixScaling(&matScale, m_vecTileObj[j][i]->ReversalX * fRatioX * 0.5f,
					m_vecTileObj[j][i]->ReversalY * fRatioY * 0.5f, 0.f);
				D3DXMatrixTranslation(&matTrans,
					(m_vecTileObj[j][i]->vPos.x - m_pMainView->GetScrollPos(0)) * fRatioX * 0.5f,
					(m_vecTileObj[j][i]->vPos.y - m_pMainView->GetScrollPos(1)) * fRatioY * 0.5f,
					m_vecTileObj[j][i]->vPos.z);

				matWorld = matScale * matTrans;

				CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

				const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
					L"MapTool", L"TileObj", m_vecTileObj[j][i]->byDrawID);

				if (pTexInfo == nullptr)
					continue;

				float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
				float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

				CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
					&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
					D3DCOLOR_ARGB(255, 255, 255, 255));
			}

		// 상호작용 오브젝트 렌더링
		for (size_t i = 0; i < m_vecInteractObj.size(); ++i)
		{
			D3DXMatrixIdentity(&matWorld);
			D3DXMatrixScaling(&matScale, m_vecInteractObj[i]->ReversalX * fRatioX * 0.5f,
				m_vecInteractObj[i]->ReversalY * fRatioY * 0.5f, 0.f);
			D3DXMatrixTranslation(&matTrans,
				(m_vecInteractObj[i]->vPos.x - m_pMainView->GetScrollPos(0)) * fRatioX * 0.5f,
				(m_vecInteractObj[i]->vPos.y - m_pMainView->GetScrollPos(1)) * fRatioY * 0.5f,
				m_vecInteractObj[i]->vPos.z);

			matWorld = matScale * matTrans;

			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

			const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
				L"MapTool", L"InteractObj", m_vecInteractObj[i]->byDrawID);

			if (pTexInfo == nullptr)
				continue;

			float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
			float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
				&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
	
	// 몬스터 렌더링
	for (size_t i = 0; i < m_vecMonster.size(); ++i)
	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f);
		D3DXMatrixTranslation(&matTrans,
			(m_vecMonster[i]->vPos.x - m_pMainView->GetScrollPos(0)) * fRatioX * 0.5f,
			(m_vecMonster[i]->vPos.y - m_pMainView->GetScrollPos(1)) * fRatioY * 0.5f,
			m_vecMonster[i]->vPos.z);

		matWorld = matScale * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

		const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
			L"MapTool", L"Monster", m_vecMonster[i]->byDrawID);

		if (pTexInfo == nullptr)
			continue;

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	// --------------------- 그리드 렌더링
	if (m_dwVisible & VISIBLE_GREED)
		for (size_t i = 0; i < m_vecTile.size(); ++i)
		{
			D3DXMatrixIdentity(&matWorld);
			D3DXMatrixScaling(&matScale, fRatioX * 0.5f, fRatioY * 0.5f, 0.f);
			D3DXMatrixTranslation(&matTrans,
				(m_vecTile[i]->vPos.x - m_pMainView->GetScrollPos(0)) * fRatioX * 0.5f,
				(m_vecTile[i]->vPos.y - m_pMainView->GetScrollPos(1)) * fRatioY * 0.5f,
				m_vecTile[i]->vPos.z);

			matWorld = matScale * matTrans;

			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

			const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
				L"MapTool", L"Greed", m_vecTile[i]->byOption);

			if (pTexInfo == nullptr)
				continue;

			float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
			float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
				&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
				D3DCOLOR_ARGB(255, 255, 255, 255));
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
}

void CTerrain::CheckNeighbor()
{
	for (int TileY = 0; TileY < m_iTileY; ++TileY)
	{
		for (int TileX = 0; TileX < m_iTileX; ++TileX)
		{
			int index = TileY * m_iTileX + TileX;

			// 렌더 중인 타일들만 검사 ( byOption이 Y_Block인 것만 검사 )
			if (m_vecTile[index]->byOption != Y_BLOCK)
				continue;

			bool bIsBlock[4] = { false, false, false, false };
			//// 검사하는 타일의 4방향 체크
			//    0
			//  3 M 1
			//    2

			// 상 방향 블럭이 있는지 검사 // 방향블럭이 리스트 크기를 초과나 미만일 경우 확인 && 방향 블럭의 옵션이 막힌블럭인지 확인
			if (TileY != 0 && (m_vecTile[index - m_iTileX]->byOption == Y_BLOCK))
				bIsBlock[0] = true;
			// 우 방향 블럭이 있는지 검사
			if (TileX != (m_iTileX - 1) && (m_vecTile[index + 1]->byOption == Y_BLOCK))
				bIsBlock[1] = true;
			// 하 방향 블럭이 있는지 검사
			if (TileY != (m_iTileY - 1) && (m_vecTile[index + m_iTileX]->byOption == Y_BLOCK))
				bIsBlock[2] = true;
			// 좌 방향 블럭이 있는지 검사
			if (TileX != 0 && (m_vecTile[index - 1]->byOption == Y_BLOCK))
				bIsBlock[3] = true;

			// SetDrawID
			//	0 1 2
			//	3 4	5 // 4 = Default
			//	6 7 8

			if (bIsBlock[0] && bIsBlock[1] && bIsBlock[2] && bIsBlock[3]) // null
				m_vecTile[index]->byDrawID = 4;
			else if (!bIsBlock[0] && bIsBlock[1] && bIsBlock[2] && bIsBlock[3]) // 상
				m_vecTile[index]->byDrawID = 1;
			else if (bIsBlock[0] && bIsBlock[1] && bIsBlock[2] && !bIsBlock[3]) // 좌
				m_vecTile[index]->byDrawID = 3;
			else if (bIsBlock[0] && !bIsBlock[1] && bIsBlock[2] && bIsBlock[3]) // 우
				m_vecTile[index]->byDrawID = 5;
			else if (bIsBlock[0] && bIsBlock[1] && !bIsBlock[2] && bIsBlock[3]) // 하
				m_vecTile[index]->byDrawID = 7;
			else if (!bIsBlock[0] && bIsBlock[1] && bIsBlock[2] && !bIsBlock[3]) // 상-좌대각
				m_vecTile[index]->byDrawID = 0;
			else if (!bIsBlock[0] && !bIsBlock[1] && bIsBlock[2] && bIsBlock[3]) // 상-우대각
				m_vecTile[index]->byDrawID = 2;
			else if (bIsBlock[0] && bIsBlock[1] && !bIsBlock[2] && !bIsBlock[3]) // 하-좌대각
				m_vecTile[index]->byDrawID = 6;
			else if (bIsBlock[0] && !bIsBlock[1] && !bIsBlock[2] && bIsBlock[3]) // 하-우대각
				m_vecTile[index]->byDrawID = 8;
		}
	}
}

void CTerrain::ChangeTileOption(const D3DXVECTOR3 & pos)
{
	int index = GetTileIndex(pos);

	if (index == -1)
		return;

	if (m_vecTile[index]->byOption == Y_BLOCK)
		m_vecTile[index]->byOption = N_BLOCK;
	else
		m_vecTile[index]->byOption = Y_BLOCK;
}

int CTerrain::GetTileIndex(const D3DXVECTOR3& pos)
{
	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		if (IsPicking(pos, i))
			return i;
	}

	return -1;
}

bool CTerrain::IsPicking(const D3DXVECTOR3& pos, const int& index)
{
	float gap = static_cast<float>(SIZEGAP);
	RECT rc = { LONG(m_vecTile[index]->vPos.x - gap), LONG(m_vecTile[index]->vPos.y - gap),
		LONG(m_vecTile[index]->vPos.x + gap), LONG(m_vecTile[index]->vPos.y + gap) };

	if (pos.x > rc.left && pos.x < rc.right && pos.y > rc.top && pos.y < rc.bottom)
		return true;

	return false;
}

void CTerrain::CheckRect(const RECT& SelectRC)
{
	int LeftTopIndex = GetTileIndex(D3DXVECTOR3((float)SelectRC.left, (float)SelectRC.top, 0.f));
	int RightBottomIndex = GetTileIndex(D3DXVECTOR3((float)SelectRC.right, (float)SelectRC.bottom, 0.f));

	if (LeftTopIndex == -1 || RightBottomIndex == -1)
		return;

	int LTx = static_cast<int>(m_vecTile[LeftTopIndex]->vPos.x / TILESIZE);
	int LTy = static_cast<int>(m_vecTile[LeftTopIndex]->vPos.y / TILESIZE);
							  
	int RBx = static_cast<int>(m_vecTile[RightBottomIndex]->vPos.x / TILESIZE);
	int RBy = static_cast<int>(m_vecTile[RightBottomIndex]->vPos.y / TILESIZE);

	for (int i = LTy; i<= RBy; ++i)
	{
		for (int j = LTx; j <= RBx; ++j)
		{
			int index = j + m_iTileX * i;

			m_vecTile[index]->byDrawID = 4;
			if (m_vecTile[index]->byOption == Y_BLOCK)
				m_vecTile[index]->byOption = N_BLOCK;
			else
				m_vecTile[index]->byOption = Y_BLOCK;
		}
	}

	/*RECT TileRc = {};
	RECT rc = {};

	for (auto*& pTile : m_vecTile)
	{
		TileRc.left = LONG(pTile->vPos.x - SIZEGAP);
		TileRc.top = LONG(pTile->vPos.y - SIZEGAP);
		TileRc.right = LONG(pTile->vPos.x + SIZEGAP);
		TileRc.bottom = LONG(pTile->vPos.y + SIZEGAP);

		if (IntersectRect(&rc, &TileRc, &SelectRC))
		{
			if (pTile->byOption == Y_BLOCK)
				pTile->byOption = N_BLOCK;
			else
				pTile->byOption = Y_BLOCK;
		}
	}*/
}

void CTerrain::CheckSetLst(const RECT& SelectRC)
{
	int LeftTopIndex = GetTileIndex(D3DXVECTOR3((float)SelectRC.left, (float)SelectRC.top, 0.f));
	int RightBottomIndex = GetTileIndex(D3DXVECTOR3((float)SelectRC.right, (float)SelectRC.bottom, 0.f));

	if (LeftTopIndex == -1 || RightBottomIndex == -1)
		return;

	int LTx = static_cast<int>(m_vecTile[LeftTopIndex]->vPos.x / TILESIZE);
	int LTy = static_cast<int>(m_vecTile[LeftTopIndex]->vPos.y / TILESIZE);

	int RBx = static_cast<int>(m_vecTile[RightBottomIndex]->vPos.x / TILESIZE);
	int RBy = static_cast<int>(m_vecTile[RightBottomIndex]->vPos.y / TILESIZE);

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplit.GetPane(0, 1));

	for_each(pMainView->m_SelectLst.begin(), pMainView->m_SelectLst.end(), 
		[](TILE*& pTile)
	{
		if(pTile->byOption == JUST_SELECT)
			pTile->byOption = N_BLOCK;
	});

	pMainView->m_SelectLst.clear();

	for (int i = LTy; i <= RBy; ++i)
	{
		for (int j = LTx; j <= RBx; ++j)
		{
			int index = j + m_iTileX * i;

			m_vecTile[index]->byOption = JUST_SELECT;
			pMainView->m_SelectLst.push_back(m_vecTile[index]);
		}
	}
}

void CTerrain::InitLinkViewPos()
{
	float Size = 25.f;

	vLine[0].x = vPos.x - Size;
	vLine[0].y = vPos.y - Size;

	vLine[1].x = vPos.x + Size;
	vLine[1].y = vPos.y - Size;

	vLine[2].x = vPos.x + Size;
	vLine[2].y = vPos.y + Size;

	vLine[3].x = vPos.x - Size;
	vLine[3].y = vPos.y + Size;

	vLine[4].x = vPos.x - Size;
	vLine[4].y = vPos.y - Size;
}