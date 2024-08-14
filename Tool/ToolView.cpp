
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"

#include "MainFrm.h"
#include "Terrain.h"
#include "TileObj.h"
#include "Form.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND g_hWnd;

// CToolView

IMPLEMENT_DYNCREATE(CToolView, CScrollView)

BEGIN_MESSAGE_MAP(CToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
ON_WM_MOUSEMOVE()
ON_WM_LBUTTONUP()
ON_WM_KEYDOWN()
ON_WM_CONTEXTMENU()
ON_COMMAND(ID_BLOCK, &CToolView::OnBlock)
ON_COMMAND(ID_LADDER, &CToolView::OnLadder)
ON_COMMAND(ID_TRAP, &CToolView::OnTrap)
ON_COMMAND(ID_SWITCH, &CToolView::OnSwitch)
ON_COMMAND(ID_OPTION_DELETE, &CToolView::OnOptionDelete)
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView()
	: m_pTerrain(nullptr), m_bIsRectVisible(false), m_bIsTerrainDigOn(false),
	m_bIsMonsterDigOn(false), m_fReversalX(1.f), m_fReversalY(1.f),
	m_eCurSheet(END), m_bIsMapLink(false), m_bIsSetOption(false),
	m_bIsCollision(false)
{
	// TODO: 여기에 생성 코드를 추가합니다.
	pObjKey = L"MapTool";
	pStateKey = L"Greed";
	m_iVDrawID = 0;
}

CToolView::~CToolView()
{
	m_SelectLst.clear();

	CTextureMgr::GetInstance()->DestroyInstance();
	CDevice::GetInstance()->DestroyInstance();
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CToolView 그리기

void CToolView::OnDraw(CDC* pDC)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다
	CDevice::GetInstance()->Render_Begin();

	if (m_pTerrain)
	{
		if (m_bIsMapLink)
		{
			CDevice::GetInstance()->GetLine()->Begin();
			DrawLine();
			DrawRectangle();
			CDevice::GetInstance()->GetLine()->End();
		}
		else
			m_pTerrain->Render();
			
	}

	// 마우스에 배치할 오브젝트 이미지를 표시
	if (m_bIsTerrainDigOn || m_bIsMonsterDigOn)
	{
		D3DXMATRIX matScale, matTrans, matWorld;

		RECT rcWnd = {};
		::GetClientRect(this->m_hWnd, &rcWnd);
		float fRatioX = WINCX / float(rcWnd.right - rcWnd.left);
		float fRatioY = WINCY / float(rcWnd.bottom - rcWnd.top);

		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, m_fReversalX * fRatioX * 0.5f, m_fReversalY * fRatioY * 0.5f, 0.f);
		D3DXMatrixTranslation(&matTrans, 
			float(MousePoint.x - GetScrollPos(0)) * fRatioX,
			float(MousePoint.y - GetScrollPos(1)) * fRatioY,
			0.f);
		
		matWorld = matScale * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

		const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
			pObjKey, pStateKey, m_iVDrawID);

		if (pTexInfo == nullptr)
			return;

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
			D3DCOLOR_ARGB(200 , 255, 255, 255));
	}

	CDevice::GetInstance()->Render_End();

	// 마우스 드래그하여 생긴 RECT 영역 렌더
	if (m_bIsRectVisible)
	{
		pDC->SelectStockObject(NULL_BRUSH);
		pDC->Rectangle(CRect(StartPoint.x / 2, StartPoint.y / 2, EndPoint.x / 2, EndPoint.y / 2));
	}
}


// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기


void CToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	SetScrollSizes(MM_TEXT, CSize(WINCX, WINCY));

	g_hWnd = m_hWnd;

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	RECT rcMainWnd = {};
	pMainFrm->GetWindowRect(&rcMainWnd);
	SetRect(&rcMainWnd, 0, 0, rcMainWnd.right - rcMainWnd.left, rcMainWnd.bottom - rcMainWnd.top);

	RECT rcView = {};
	GetClientRect(&rcView);

	int iRowFrm = rcMainWnd.right - rcView.right;
	int iColFrm = rcMainWnd.bottom - rcView.bottom;

	pMainFrm->SetWindowPos(nullptr, 0, 0, WINCX + iRowFrm, WINCY + iColFrm, SWP_NOZORDER);

	if (FAILED(CDevice::GetInstance()->InitDevice()))
	{
		AfxMessageBox(L"Device Init Failed!!!");
		return;
	}

	CTextureMgr::GetInstance()->ReadImagePath(L"../Data/MapData/ImagePath.txt");
}


void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnLButtonDown(nFlags, point);

	point.x += GetScrollPos(0);
	point.y += GetScrollPos(1);

	StartPoint.x = point.x * 2;
	StartPoint.y = point.y * 2;

	Invalidate(FALSE);
}

void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnMouseMove(nFlags, point);

	point.x += GetScrollPos(0);
	point.y += GetScrollPos(1);

	if (nFlags & MK_LBUTTON)
	{
		if (!m_bIsTerrainDigOn && !m_bIsMonsterDigOn)
		{
			if (!m_bIsRectVisible)
				m_bIsRectVisible = true;

			EndPoint.x = point.x * 2;
			EndPoint.y = point.y * 2;
		}

		// 움직임 시 TILE, UNOBJ, OBJ 자동으로 배치
		if (m_bIsTerrainDigOn)
		{
			int index = m_pTerrain->GetTileIndex(D3DXVECTOR3((float)point.x * 2.f, (float)point.y * 2.f, 0.f));

			// 프로퍼티 시트를 구분해서 배치
			switch (m_eCurSheet)
			{
			case MAPTILE:
				AllocateMapTile(index);
				break;
			case UNOBJ:
				AllocateUnObj(index);
				break;
			case OBJ:
				AllocateObj(index);
				break;
			}
		}
	}

	MousePoint = point;

	Invalidate(FALSE);
}


void CToolView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnLButtonUp(nFlags, point);

	point.x += GetScrollPos(0);
	point.y += GetScrollPos(1);

	EndPoint.x = point.x * 2;
	EndPoint.y = point.y * 2;

	// 작업할 맵이 선택되어 있을 경우
	if (m_pTerrain)
	{
		if (!m_bIsMapLink)
		{
			// Terrain 다이얼로그와 Monster 다이얼로그가 꺼져있을 경우
			if (!m_bIsTerrainDigOn && !m_bIsMonsterDigOn)
			{
				// 옵션 부여가 켜져있을 경우
				if (m_bIsSetOption)
				{
					// 드래그 영역으로 작업시
					if (m_bIsRectVisible) 
					{
						RECT rc = { StartPoint.x, StartPoint.y, EndPoint.x, EndPoint.y };
						m_pTerrain->CheckSetLst(rc);
					}
					else
					{
						int index = m_pTerrain->GetTileIndex(D3DXVECTOR3((float)EndPoint.x, (float)EndPoint.y, 0.f));
						if (index != -1)
						{
							m_pTerrain->m_vecTile[index]->byOption = JUST_SELECT;
							m_SelectLst.push_back(m_pTerrain->m_vecTile[index]);
						}
					}
				}
				else
				{
					if (m_bIsRectVisible) // 드래그 영역으로 작업시
					{
						RECT rc = { StartPoint.x, StartPoint.y, EndPoint.x, EndPoint.y };
						m_pTerrain->CheckRect(rc);
					}
					else // 클릭으로 작업시
						m_pTerrain->ChangeTileOption(D3DXVECTOR3((float)EndPoint.x, (float)EndPoint.y, 0.f));
				}
			}
			// Terrain 다이얼로그가 켜져 있을 때
			if (m_bIsTerrainDigOn)
			{
				int index = m_pTerrain->GetTileIndex(D3DXVECTOR3((float)EndPoint.x , (float)EndPoint.y, 0.f));

				// 프로퍼티 시트를 구분해서 배치
				switch (m_eCurSheet)
				{
				case MAPTILE:
					AllocateMapTile(index);
					break;
				case UNOBJ:
					AllocateUnObj(index);
					break;
				case OBJ:
					AllocateObj(index);
					break;
				}
			}
			// Monster 다이얼로그가 켜져 있을 때
			if (m_bIsMonsterDigOn)
			{
				CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
				CForm* pFormView = dynamic_cast<CForm*>(pMainFrame->m_MainSplit.GetPane(0, 0));

				int index = m_pTerrain->GetTileIndex(D3DXVECTOR3((float)EndPoint.x, (float)EndPoint.y, 0.f));

				if (index != -1 && pFormView->m_pMonsterDig.m_SelectMonster != nullptr)
				{
					MONSTER* pMonster = new MONSTER(*pFormView->m_pMonsterDig.m_SelectMonster);
					pMonster->vPos = m_pTerrain->m_vecTile[index]->vPos;

					m_pTerrain->m_vecMonster.push_back(pMonster);
				}
			}
		}
	}

	m_bIsRectVisible = false;
	ZeroMemory(&StartPoint, sizeof(POINT));
	ZeroMemory(&EndPoint, sizeof(POINT));
		
	Invalidate(FALSE);
}

void CToolView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);

	switch (nChar)
	{
	case VK_LEFT:
		this->SetScrollPos(0, GetScrollPos(0) - 15);
		break;
	case VK_RIGHT:
		this->SetScrollPos(0, GetScrollPos(0) + 15);
		break;
	case VK_UP:
		this->SetScrollPos(1, GetScrollPos(1) - 15);
		break;
	case VK_DOWN:
		this->SetScrollPos(1, GetScrollPos(1) + 15);
		break;
	case VK_CONTROL:
		if (GetAsyncKeyState('Z') & 0x8000)
		{
			if (m_bIsTerrainDigOn)
			{
				CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
				CForm* pFormView = dynamic_cast<CForm*>(pMainFrame->m_MainSplit.GetPane(0, 0));
				int Type = pFormView->m_pTerrainDig.m_pMySheet->m_BackObj.m_iUnObjType;

				switch (m_eCurSheet)
				{
				case UNOBJ:
					if (!(m_pTerrain->m_vecTileObj[Type].empty()))
					{
						SafeDelete(m_pTerrain->m_vecTileObj[Type].back());
						m_pTerrain->m_vecTileObj[Type].pop_back();
					}
					break;
				case OBJ:
					if (!(m_pTerrain->m_vecInteractObj.empty()))
					{
						SafeDelete(m_pTerrain->m_vecInteractObj.back());
						m_pTerrain->m_vecInteractObj.pop_back();
					}
					break;
				}
			}
			else if (m_bIsMonsterDigOn)
			{
				if (!m_pTerrain->m_vecMonster.empty())
				{
					SafeDelete(m_pTerrain->m_vecMonster.back());
					m_pTerrain->m_vecMonster.pop_back();
				}
			}
		}
		break;
	}
	
	Invalidate(FALSE);
}

// 맵 타일 배치
void CToolView::AllocateMapTile(int index)
{
	// 인덱스 값이 없거나 해당 타일이 맵타일이 아닐 경우
	if (index == -1 || m_pTerrain->m_vecTile[index]->byOption == N_BLOCK)
		return;

	m_pTerrain->m_vecTile[index]->byDrawID = m_iVDrawID;
	m_pTerrain->m_vecTile[index]->ReversalX = m_fReversalX;
	m_pTerrain->m_vecTile[index]->ReversalY = m_fReversalY;
}
// 비 상호작용 오브젝트 배치
void CToolView::AllocateUnObj(int index)
{
	// 인덱스 값이 없거나 해당 타일이 맵타일일 경우
	if (index == -1 || m_pTerrain->m_vecTile[index]->byOption == Y_BLOCK)
		return;

	float pointX = m_pTerrain->m_vecTile[index]->vPos.x;
	float pointY = m_pTerrain->m_vecTile[index]->vPos.y;
	/*float pointX = (float)EndPoint.x;
	float pointY = (float)EndPoint.y;*/

	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CForm* pFormView = dynamic_cast<CForm*>(pMainFrame->m_MainSplit.GetPane(0, 0));
	int Type = pFormView->m_pTerrainDig.m_pMySheet->m_BackObj.m_iUnObjType;

	// 선택 영역에 이미 비 상호작용 오브젝트가 배치되어있는지 확인
	auto iter_find = find_if(m_pTerrain->m_vecTileObj[Type].begin(), m_pTerrain->m_vecTileObj[Type].end(),
		[&pointX, &pointY](auto*& pTile)
	{
		return (pTile->vPos.x == pointX) && (pTile->vPos.y == pointY);
	});

	if (iter_find == m_pTerrain->m_vecTileObj[Type].end()) // 빈 타일을 선택시 객체 생성
	{
		TILE* pTile = new TILE;
		pTile->vPos = { pointX, pointY, 0.f };
		pTile->byDrawID = m_iVDrawID;
		pTile->ReversalX = m_fReversalX;
		pTile->ReversalY = m_fReversalY;
		m_pTerrain->m_vecTileObj[Type].push_back(pTile);
	}
	else // 이미 배치된 타일을 선택 시 정보만 변경
	{
		(*iter_find)->byDrawID = m_iVDrawID;
		(*iter_find)->ReversalX = m_fReversalX;
		(*iter_find)->ReversalY = m_fReversalY;
	}
}
// 상호작용 오브젝트 배치
void CToolView::AllocateObj(int index)
{
	// 인덱스 값이 없거나 해당 타일이 맵타일일 경우
	if (index == -1 || m_pTerrain->m_vecTile[index]->byOption == Y_BLOCK)
		return;

	float pointX = m_pTerrain->m_vecTile[index]->vPos.x;
	float pointY = m_pTerrain->m_vecTile[index]->vPos.y;

	TILE* pTile = new TILE;

	pTile->vPos = D3DXVECTOR3(pointX, pointY, 0.f);
	pTile->byDrawID = m_iVDrawID;
	pTile->ReversalX = m_fReversalX;
	pTile->ReversalY = m_fReversalY;
	m_pTerrain->m_vecInteractObj.push_back(pTile);
}

void CToolView::InitLinkViewPos()
{
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CForm* pFormView = dynamic_cast<CForm*>(pMainFrame->m_MainSplit.GetPane(0, 0));

	/*for (auto*& pTerrain : pFormView->m_vecCreateMap)
		pTerrain->vPos = { 0.f, 0.f, 0.f };*/

	// 첫 시작 맵의 위치 좌표
	pFormView->m_vecCreateMap.front()->vPos = { 150.f, WINCY * 0.5f, 0.f };

	for (auto*& pTerrain : pFormView->m_vecCreateMap)
	{
		// 사각형 각 점의 좌표 설정
		pTerrain->InitLinkViewPos();

		// UpLink // UpMapID값이 있을 때
		if (lstrlen(pTerrain->m_tLink.UpMapID) != 0)
		{
			// UpMapID를 가진 맵을 검색
			auto iter_find = find_if(pFormView->m_vecCreateMap.begin(), pFormView->m_vecCreateMap.end(),
				[&pTerrain](CTerrain*& m_pTerrain)
			{
				return pTerrain->m_tLink.UpMapID == m_pTerrain->m_tLink.MyMapID;
			});

			// 발견했고 좌표가 초기화값으로 되있을 경우
			if ((*iter_find)->vPos.x == 0 || (*iter_find)->vPos.y == 0)
			{
				// 좌표값 설정
				(*iter_find)->vPos = pTerrain->vPos;
				(*iter_find)->vPos.y -= 100.f;
			}
			
		}
		else // UpMapID값이 없을 때
		{
			// 자기 자신의 ID값이 전체 맵의 DownMapID에 들어있는지의 여부 검사
			auto iter_find = find_if(pFormView->m_vecCreateMap.begin(), pFormView->m_vecCreateMap.end(),
				[&pTerrain](CTerrain*& m_pTerrain)
			{
				return pTerrain->m_tLink.MyMapID == m_pTerrain->m_tLink.DownMapID;
			});

			// DownMapID에 자기 자신의 ID 값이 있을 때
			if (iter_find != pFormView->m_vecCreateMap.end())
			{
				//(*iter_find)->vPos = { 0.f, 0.f, 0.f };

				// 연결된 맵이 없다고 설정
				(*iter_find)->m_tLink.DownMapID = L"";
			}
		}

		// RightLink
		if (lstrlen(pTerrain->m_tLink.RightMapID) != 0)
		{
			auto iter_find = find_if(pFormView->m_vecCreateMap.begin(), pFormView->m_vecCreateMap.end(),
				[&pTerrain](CTerrain*& m_pTerrain)
			{
				return pTerrain->m_tLink.RightMapID == m_pTerrain->m_tLink.MyMapID;
			});

			if ((*iter_find)->vPos.x == 0 || (*iter_find)->vPos.y == 0)
			{
				(*iter_find)->vPos = pTerrain->vPos;
				(*iter_find)->vPos.x += 100.f;
			}
		}
		else
		{
			auto iter_find = find_if(pFormView->m_vecCreateMap.begin(), pFormView->m_vecCreateMap.end(),
				[&pTerrain](CTerrain*& m_pTerrain)
			{
				return pTerrain->m_tLink.MyMapID == m_pTerrain->m_tLink.LeftMapID;
			});

			if (iter_find != pFormView->m_vecCreateMap.end())
			{
				//(*iter_find)->vPos = { 0.f, 0.f, 0.f };
				(*iter_find)->m_tLink.LeftMapID = L"";
			}
		}

		// DownLink
		if (lstrlen(pTerrain->m_tLink.DownMapID) != 0)
		{
			auto iter_find = find_if(pFormView->m_vecCreateMap.begin(), pFormView->m_vecCreateMap.end(),
				[&pTerrain](CTerrain*& m_pTerrain)
			{
				return pTerrain->m_tLink.DownMapID == m_pTerrain->m_tLink.MyMapID;
			});

			if ((*iter_find)->vPos.x == 0 || (*iter_find)->vPos.y == 0)
			{
				(*iter_find)->vPos = pTerrain->vPos;
				(*iter_find)->vPos.y += 100.f;
			}
		}
		else
		{
			auto iter_find = find_if(pFormView->m_vecCreateMap.begin(), pFormView->m_vecCreateMap.end(),
				[&pTerrain](CTerrain*& m_pTerrain)
			{
				return pTerrain->m_tLink.MyMapID == m_pTerrain->m_tLink.UpMapID;
			});

			if (iter_find != pFormView->m_vecCreateMap.end())
			{
				//(*iter_find)->vPos = { 0.f, 0.f, 0.f };
				(*iter_find)->m_tLink.UpMapID = L"";
			}
		}

		// LeftLink
		if (lstrlen(pTerrain->m_tLink.LeftMapID) != 0)
		{
			auto iter_find = find_if(pFormView->m_vecCreateMap.begin(), pFormView->m_vecCreateMap.end(),
				[&pTerrain](CTerrain*& m_pTerrain)
			{
				return pTerrain->m_tLink.LeftMapID == m_pTerrain->m_tLink.MyMapID;
			});

			if ((*iter_find)->vPos.x == 0 || (*iter_find)->vPos.y == 0)
			{
				(*iter_find)->vPos = pTerrain->vPos;
				(*iter_find)->vPos.x -= 100.f;
			}
		}
		else
		{
			auto iter_find = find_if(pFormView->m_vecCreateMap.begin(), pFormView->m_vecCreateMap.end(),
				[&pTerrain](CTerrain*& m_pTerrain)
			{
				return pTerrain->m_tLink.MyMapID == m_pTerrain->m_tLink.RightMapID;
			});

			if (iter_find != pFormView->m_vecCreateMap.end())
			{
				//(*iter_find)->vPos = { 0.f, 0.f, 0.f };
				(*iter_find)->m_tLink.RightMapID = L"";
			}
		}

		
	}
}

void CToolView::DrawLine()
{
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CForm* pFormView = dynamic_cast<CForm*>(pMainFrame->m_MainSplit.GetPane(0, 0));

	// Up Link
	for (auto*& pTerrain : pFormView->m_vecCreateMap)
	{
		auto iter_find = find_if(pFormView->m_vecCreateMap.begin(), pFormView->m_vecCreateMap.end(),
			[&pTerrain](CTerrain*& m_pTerrain)
		{
			return pTerrain->m_tLink.UpMapID == m_pTerrain->m_tLink.MyMapID;
		});

		if (iter_find == pFormView->m_vecCreateMap.end())
			continue;

		D3DXVECTOR2 Line[2] = {
			{ pTerrain->vPos.x, pTerrain->vPos.y },{ (*iter_find)->vPos.x, (*iter_find)->vPos.y }
		};

		CDevice::GetInstance()->GetLine()->Draw(Line, 2, D3DCOLOR_XRGB(0, 0, 0));
	}
	// Right Link
	for (auto*& pTerrain : pFormView->m_vecCreateMap)
	{
		auto iter_find = find_if(pFormView->m_vecCreateMap.begin(), pFormView->m_vecCreateMap.end(),
			[&pTerrain](CTerrain*& m_pTerrain)
		{
			return pTerrain->m_tLink.RightMapID == m_pTerrain->m_tLink.MyMapID;
		});

		if (iter_find == pFormView->m_vecCreateMap.end())
			continue;

		D3DXVECTOR2 Line[2] = {
			{ pTerrain->vPos.x, pTerrain->vPos.y },{ (*iter_find)->vPos.x, (*iter_find)->vPos.y }
		};

		CDevice::GetInstance()->GetLine()->Draw(Line, 2, D3DCOLOR_XRGB(0, 0, 0));
	}
	// Down Link
	for (auto*& pTerrain : pFormView->m_vecCreateMap)
	{
		auto iter_find = find_if(pFormView->m_vecCreateMap.begin(), pFormView->m_vecCreateMap.end(),
			[&pTerrain](CTerrain*& m_pTerrain)
		{
			return pTerrain->m_tLink.DownMapID == m_pTerrain->m_tLink.MyMapID;
		});

		if (iter_find == pFormView->m_vecCreateMap.end())
			continue;

		D3DXVECTOR2 Line[2] = {
			{ pTerrain->vPos.x, pTerrain->vPos.y },{ (*iter_find)->vPos.x, (*iter_find)->vPos.y }
		};

		CDevice::GetInstance()->GetLine()->Draw(Line, 2, D3DCOLOR_XRGB(0, 0, 0));
	}
	// Left Link
	for (auto*& pTerrain : pFormView->m_vecCreateMap)
	{
		auto iter_find = find_if(pFormView->m_vecCreateMap.begin(), pFormView->m_vecCreateMap.end(),
			[&pTerrain](CTerrain*& m_pTerrain)
		{
			return pTerrain->m_tLink.LeftMapID == m_pTerrain->m_tLink.MyMapID;
		});

		if (iter_find == pFormView->m_vecCreateMap.end())
			continue;

		D3DXVECTOR2 Line[2] = {
			{ pTerrain->vPos.x, pTerrain->vPos.y },{ (*iter_find)->vPos.x, (*iter_find)->vPos.y }
		};

		CDevice::GetInstance()->GetLine()->Draw(Line, 2, D3DCOLOR_XRGB(0, 0, 0));
	}
}

void CToolView::DrawRectangle()
{
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CForm* pFormView = dynamic_cast<CForm*>(pMainFrame->m_MainSplit.GetPane(0, 0));

	for (auto*& pTerrain : pFormView->m_vecCreateMap)
	{
		CDevice::GetInstance()->GetFont()->DrawTextW(nullptr, pTerrain->m_tLink.MyMapID, lstrlen(pTerrain->m_tLink.MyMapID),
			CRect(int(pTerrain->vPos.x), int(pTerrain->vPos.y), int(pTerrain->vPos.x + 100.f), int(pTerrain->vPos.y + 100.f)),
			0, D3DCOLOR_ARGB(255, 0, 0, 0));
		CDevice::GetInstance()->GetLine()->Draw(pTerrain->vLine, 5, D3DCOLOR_XRGB(0, 0, 0));
	}
		
}

void CToolView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	CMenu OptionMenu;
	OptionMenu.LoadMenuW(IDR_OPTIONMENU);

	CMenu* pMenu;
	pMenu = OptionMenu.GetSubMenu(0);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN || TPM_RIGHTBUTTON, point.x/* + GetScrollPos(0)*/, point.y/* + GetScrollPos(1)*/, this);
}

void CToolView::OnBlock()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplit.GetPane(0, 1));

	for_each(pMainView->m_SelectLst.begin(), pMainView->m_SelectLst.end(),
		[](TILE*& pTile)
	{
		pTile->byOption = Y_BLOCK;
	});

	pMainView->m_SelectLst.clear();

	pMainView->Invalidate(FALSE);

	UpdateData(FALSE);
}

void CToolView::OnLadder()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplit.GetPane(0, 1));

	for_each(pMainView->m_SelectLst.begin(), pMainView->m_SelectLst.end(),
		[](TILE*& pTile)
	{
		pTile->byOption = LADDER;
	});

	pMainView->m_SelectLst.clear();

	pMainView->Invalidate(FALSE);

	UpdateData(FALSE);
}

void CToolView::OnTrap()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplit.GetPane(0, 1));

	for_each(pMainView->m_SelectLst.begin(), pMainView->m_SelectLst.end(),
		[](TILE*& pTile)
	{
		pTile->byOption = TRAP;
	});

	pMainView->m_SelectLst.clear();

	pMainView->Invalidate(FALSE);

	UpdateData(FALSE);
}

void CToolView::OnSwitch()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplit.GetPane(0, 1));

	for_each(pMainView->m_SelectLst.begin(), pMainView->m_SelectLst.end(),
		[](TILE*& pTile)
	{
		pTile->byOption = SWITCH;
	});

	pMainView->m_SelectLst.clear();

	pMainView->Invalidate(FALSE);

	UpdateData(FALSE);
}

void CToolView::OnOptionDelete()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplit.GetPane(0, 1));

	for_each(pMainView->m_SelectLst.begin(), pMainView->m_SelectLst.end(),
		[](TILE*& pTile)
	{
		pTile->byOption = N_BLOCK;
	});

	pMainView->m_SelectLst.clear();

	pMainView->Invalidate(FALSE);

	UpdateData(FALSE);
}