// Form.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "Form.h"

#include "MainFrm.h"
#include "ToolView.h"
#include "Terrain.h"

// CForm

IMPLEMENT_DYNCREATE(CForm, CFormView)

CForm::CForm()
	: CFormView(IDD_FORM)
	, m_iTileX(0)
	, m_iTileY(0)
	, m_iVisibleTileW(0)
	, m_iVisibleMapH(0)
	, m_iMapTileX(0)
	, m_iMapTileY(0)
{

}

CForm::~CForm()
{
	for_each(m_vecCreateMap.begin(), m_vecCreateMap.end(), SafeDelete<CTerrain*>);
	m_vecCreateMap.clear();
}

void CForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT5, m_iTileX);
	DDX_Text(pDX, IDC_EDIT6, m_iTileY);
	DDX_Control(pDX, IDC_LIST1, m_MapListBox);
	DDX_Text(pDX, IDC_EDIT1, m_iVisibleTileW);
	DDX_Text(pDX, IDC_EDIT2, m_iVisibleMapH);
	DDX_Text(pDX, IDC_EDIT3, m_iMapTileX);
	DDX_Text(pDX, IDC_EDIT4, m_iMapTileY);
	DDX_Control(pDX, IDC_CHECK1, m_CheckBox[0]);
	DDX_Control(pDX, IDC_CHECK2, m_CheckBox[1]);
	DDX_Control(pDX, IDC_CHECK3, m_CheckBox[2]);
}

BEGIN_MESSAGE_MAP(CForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON5, &CForm::OnBnClickedCreateWorld)
	ON_LBN_SELCHANGE(IDC_LIST1, &CForm::OnLbnSelchangeMapListBox)
	ON_BN_CLICKED(IDC_BUTTON1, &CForm::OnBnClickedOpenTerrainEdit)
	ON_BN_CLICKED(IDC_BUTTON2, &CForm::OnBnClickedOpenMonsterEdit)
	ON_BN_CLICKED(IDC_BUTTON3, &CForm::OnBnClickedSaveData)
	ON_BN_CLICKED(IDC_BUTTON8, &CForm::OnBnClickedSetGround)
	ON_BN_CLICKED(IDC_BUTTON7, &CForm::OnBnClickedLoadData)
	ON_BN_CLICKED(IDC_BUTTON6, &CForm::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_CHECK1, &CForm::OnBnClickedGreedVisible)
	ON_BN_CLICKED(IDC_CHECK2, &CForm::OnBnClickedTileVisible)
	ON_BN_CLICKED(IDC_CHECK3, &CForm::OnBnClickedObjectVisible)
	ON_BN_CLICKED(IDC_BUTTON9, &CForm::OnBnClickedMapLink)
	ON_BN_CLICKED(IDC_BUTTON10, &CForm::OnBnClickedPathFind)
	ON_BN_CLICKED(IDC_CHECK4, &CForm::OnBnClickedSetOption)
	ON_BN_CLICKED(IDC_CHECK5, &CForm::OnBnClickedSetCollision)
END_MESSAGE_MAP()


// CForm 진단입니다.

#ifdef _DEBUG
void CForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CForm 메시지 처리기입니다.


void CForm::OnBnClickedCreateWorld()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	if (m_iTileX == 0 && m_iTileY == 0)
	{
		AfxMessageBox(L"맵 크기를 설정해 주세요.");
		return;
	}

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplit.GetPane(0, 1));

	CTerrain* pCreateTerrain = new CTerrain;

	pCreateTerrain->m_iTileX = m_iTileX;
	pCreateTerrain->m_iTileY = m_iTileY;

	pCreateTerrain->Initialize();
	pCreateTerrain->SetMainView(pMainView);

	// 맵 ID 지정
	TCHAR szIdNum[MIN_STR] = L"";
	TCHAR szIdName[MIN_STR] = L"Map";
	_itow_s(m_vecCreateMap.size(), szIdNum, 10);
	lstrcat(szIdName, szIdNum);

	pCreateTerrain->m_tLink.MyMapID = szIdName;

	// 리스트 박스에 목록 추가
	m_MapListBox.AddString(szIdName);

	// 맵 리스트에 push
	m_vecCreateMap.push_back(pCreateTerrain);

	pMainView->m_pTerrain = pCreateTerrain; 
	// 뷰영역 스크롤 리사이즈
	pMainView->SetScrollSizes(MM_TEXT, CSize(TILESIZE * pMainView->m_pTerrain->m_iTileX, TILESIZE * pMainView->m_pTerrain->m_iTileY));

	pMainView->Invalidate(FALSE);

	m_iVisibleTileW = m_iTileX * TILESIZE;
	m_iVisibleMapH = m_iTileY * TILESIZE;
	m_iMapTileX = m_iTileX;
	m_iMapTileY = m_iTileY;

	CheckDlgButton(IDC_CHECK1, TRUE);
	CheckDlgButton(IDC_CHECK2, TRUE);
	CheckDlgButton(IDC_CHECK3, TRUE);

	m_iTileX = 0;
	m_iTileY = 0;

	UpdateData(FALSE);
}


void CForm::OnLbnSelchangeMapListBox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	int index = m_MapListBox.GetCurSel();

	if (LB_ERR == index)
		return;

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplit.GetPane(0, 1));

	pMainView->m_pTerrain = m_vecCreateMap[index];

	m_iMapTileX = pMainView->m_pTerrain->m_iTileX;
	m_iMapTileY = pMainView->m_pTerrain->m_iTileY;
	m_iVisibleTileW = m_iMapTileX * TILESIZE;
	m_iVisibleMapH = m_iMapTileY * TILESIZE;

	pMainView->SetScrollSizes(MM_TEXT, CSize(TILESIZE * m_iMapTileX, TILESIZE * m_iMapTileY));

	pMainView->Invalidate(FALSE);

	UpdateData(FALSE);
}


void CForm::OnBnClickedOpenTerrainEdit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (m_pTerrainDig.GetSafeHwnd() == nullptr)
		m_pTerrainDig.Create(IDD_TERRAINDIG);
	
	m_pTerrainDig.ShowWindow(SW_SHOW);

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplit.GetPane(0, 1));
	pMainView->m_bIsTerrainDigOn = ON;
}


void CForm::OnBnClickedOpenMonsterEdit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (m_pMonsterDig.GetSafeHwnd() == nullptr)
		m_pMonsterDig.Create(IDD_MONSTERDIG);

	m_pMonsterDig.ShowWindow(SW_SHOW);

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplit.GetPane(0, 1));
	pMainView->m_bIsMonsterDigOn = ON;
}



void CForm::OnBnClickedMapLink()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (m_pMapLinkDig.GetSafeHwnd() == nullptr)
		m_pMapLinkDig.Create(IDD_MAPLINK);

	m_pMapLinkDig.ShowWindow(SW_SHOW);

	m_pMapLinkDig.m_LinkListBox.ResetContent();

	// 폼의 맵 리스트를 다이얼로그에도 삽입 
	for (size_t i = 0; i < m_vecCreateMap.size(); ++i)
		m_pMapLinkDig.m_LinkListBox.AddString(m_vecCreateMap[i]->m_tLink.MyMapID);

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplit.GetPane(0, 1));
	pMainView->m_bIsMapLink = true;
	pMainView->Invalidate(FALSE);
}


void CForm::OnBnClickedSaveData()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	CFileDialog Dlg(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat||", this);

	TCHAR szPath[MAX_STR] = L"";

	GetCurrentDirectory(MAX_STR, szPath);
	PathRemoveFileSpec(szPath);
	lstrcat(szPath, L"\\Data\\MapData");

	Dlg.m_ofn.lpstrInitialDir = szPath;

	if (IDOK == Dlg.DoModal())
	{
		CString strGetPath = Dlg.GetPathName();
		const TCHAR* pGetPath = strGetPath.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			AfxMessageBox(L"Save Failed!!");
			return;
		}

		DWORD dwByte = 0;

		// 맵 갯수 저장
		int size = m_vecCreateMap.size();
		WriteFile(hFile, &size, sizeof(int), &dwByte, nullptr);

		for (CTerrain*& pTerrain : m_vecCreateMap)
		{
			// 맵 X, Y 크기
			WriteFile(hFile, &pTerrain->m_iTileX, sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, &pTerrain->m_iTileY, sizeof(int), &dwByte, nullptr);

			// 타일 정보
			size = pTerrain->m_vecTile.size();
			WriteFile(hFile, &size, sizeof(int), &dwByte, nullptr);
			for (TILE*& pTile : pTerrain->m_vecTile)
				WriteFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);

			// Front 비 상호작용 오브젝트 정보
			size = pTerrain->m_vecTileObj[0].size();
			WriteFile(hFile, &size, sizeof(int), &dwByte, nullptr);
			for (TILE*& pTile : pTerrain->m_vecTileObj[0])
				WriteFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);

			// Back 비 상호작용 오브젝트 정보
			size = pTerrain->m_vecTileObj[1].size();
			WriteFile(hFile, &size, sizeof(int), &dwByte, nullptr);
			for (TILE*& pTile : pTerrain->m_vecTileObj[1])
				WriteFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);

			// 상호작용 오브젝트 정보
			size = pTerrain->m_vecInteractObj.size();
			WriteFile(hFile, &size, sizeof(int), &dwByte, nullptr);
			for (TILE*& pTile : pTerrain->m_vecInteractObj)
				WriteFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);

			// ID
			size = pTerrain->m_tLink.MyMapID.GetLength() + 1;
			WriteFile(hFile, &size, sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, pTerrain->m_tLink.MyMapID, sizeof(TCHAR) * size, &dwByte, nullptr);
			// UpLinkID
			size = pTerrain->m_tLink.UpMapID.GetLength() + 1;
			WriteFile(hFile, &size, sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, pTerrain->m_tLink.UpMapID, sizeof(TCHAR) * size, &dwByte, nullptr);
			// RightLinkID
			size = pTerrain->m_tLink.RightMapID.GetLength() + 1;
			WriteFile(hFile, &size, sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, pTerrain->m_tLink.RightMapID, sizeof(TCHAR) * size, &dwByte, nullptr);
			// DownLinkID
			size = pTerrain->m_tLink.DownMapID.GetLength() + 1;
			WriteFile(hFile, &size, sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, pTerrain->m_tLink.DownMapID, sizeof(TCHAR) * size, &dwByte, nullptr);
			// LeftLinkID
			size = pTerrain->m_tLink.LeftMapID.GetLength() + 1;
			WriteFile(hFile, &size, sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, pTerrain->m_tLink.LeftMapID, sizeof(TCHAR) * size, &dwByte, nullptr);
		}

		// 몬스터 데이터 저장
		TCHAR szPath[MAX_STR] = L"";
		lstrcat(szPath, Dlg.GetPathName().GetString());
		PathRemoveExtension(szPath);
		lstrcat(szPath, L"Monster.dat");
		hFile = CreateFile(szPath, GENERIC_WRITE, 0, 0,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			AfxMessageBox(L"Monster Data Save Failed!!");
			return;
		}

		size = m_vecCreateMap.size();
		WriteFile(hFile, &size, sizeof(int), &dwByte, nullptr);
		for (CTerrain*& pTerrain : m_vecCreateMap)
		{
			size = pTerrain->m_vecMonster.size();
			WriteFile(hFile, &size, sizeof(int), &dwByte, nullptr);
			for (MONSTER*& pData : pTerrain->m_vecMonster)
			{
				size = pData->strID.GetLength() + 1;
				WriteFile(hFile, &size, sizeof(int), &dwByte, nullptr);
				WriteFile(hFile, pData->strID, sizeof(TCHAR) * size, &dwByte, nullptr);

				WriteFile(hFile, &pData->vPos, sizeof(D3DXVECTOR3), &dwByte, nullptr);
				WriteFile(hFile, &pData->HP, sizeof(int), &dwByte, nullptr);
				WriteFile(hFile, &pData->byDrawID, sizeof(BYTE), &dwByte, nullptr);
			}
			
		}

		CloseHandle(hFile);
	}

	UpdateData(FALSE);
}

void CForm::OnBnClickedLoadData()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	CFileDialog Dlg(TRUE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat||", this);

	TCHAR szPath[MAX_STR] = L"";

	GetCurrentDirectory(MAX_STR, szPath);
	PathRemoveFileSpec(szPath);
	lstrcat(szPath, L"\\Data\\MapData");

	Dlg.m_ofn.lpstrInitialDir = szPath;

	if (IDOK == Dlg.DoModal())
	{
		HANDLE hFile = CreateFile(Dlg.GetPathName().GetString(), GENERIC_READ, 0, 0,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			AfxMessageBox(L"Load Failed!!!");
			return;
		}

		CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplit.GetPane(0, 1));

		DWORD dwByte = 0;
		
		int MapSize = 0, Size = 0;
		int MapTileX = 0, MapTileY = 0;
		TILE pTileData = {};
		RECT pRc = {};
		TCHAR* szMapLink = nullptr;

		ReadFile(hFile, &MapSize, sizeof(int), &dwByte, nullptr);

		for (int index = 0; index < MapSize; ++index)
		{
			// 지형 동적할당 생성
			m_vecCreateMap.push_back(new CTerrain);

			// 맵 X크기 로드 & 대입
			ReadFile(hFile, &MapTileX, sizeof(int), &dwByte, nullptr);
			m_vecCreateMap[index]->m_iTileX = MapTileX;
			// 맵 Y크기 로드 & 대입
			ReadFile(hFile, &MapTileY, sizeof(int), &dwByte, nullptr);
			m_vecCreateMap[index]->m_iTileY = MapTileY;

			// Tile 데이터 로드
			ReadFile(hFile, &Size, sizeof(int), &dwByte, nullptr);
			for (int i = 0; i < Size; ++i)
			{
				ReadFile(hFile, &pTileData, sizeof(TILE), &dwByte, nullptr);
				m_vecCreateMap[index]->m_vecTile.push_back(new TILE(pTileData));
			}

			// Front 비 상호작용 오브젝트 데이터 로드
			ReadFile(hFile, &Size, sizeof(int), &dwByte, nullptr);
			for (int i = 0; i < Size; ++i)
			{
				ReadFile(hFile, &pTileData, sizeof(TILE), &dwByte, nullptr);
				m_vecCreateMap[index]->m_vecTileObj[0].push_back(new TILE(pTileData));
			}

			// Back 비 상호작용 오브젝트 데이터 로드
			ReadFile(hFile, &Size, sizeof(int), &dwByte, nullptr);
			for (int i = 0; i < Size; ++i)
			{
				ReadFile(hFile, &pTileData, sizeof(TILE), &dwByte, nullptr);
				m_vecCreateMap[index]->m_vecTileObj[1].push_back(new TILE(pTileData));
			}

			// 상호작용 오브젝트 데이터 로드
			ReadFile(hFile, &Size, sizeof(int), &dwByte, nullptr);
			for (int i = 0; i < Size; ++i)
			{
				ReadFile(hFile, &pTileData, sizeof(TILE), &dwByte, nullptr);
				m_vecCreateMap[index]->m_vecInteractObj.push_back(new TILE(pTileData));
			}

			// Map ID
			ReadFile(hFile, &Size, sizeof(int), &dwByte, nullptr);

			szMapLink = new TCHAR[Size];

			ReadFile(hFile, szMapLink, sizeof(TCHAR) * Size, &dwByte, nullptr);
			m_vecCreateMap[index]->m_tLink.MyMapID = szMapLink;

			delete[] szMapLink;
			szMapLink = nullptr;

			// Map Up Link ID
			ReadFile(hFile, &Size, sizeof(int), &dwByte, nullptr);

			szMapLink = new TCHAR[Size];

			ReadFile(hFile, szMapLink, sizeof(TCHAR) * Size, &dwByte, nullptr);
			m_vecCreateMap[index]->m_tLink.UpMapID = szMapLink;

			delete[] szMapLink;
			szMapLink = nullptr;

			// Map Right Link ID
			ReadFile(hFile, &Size, sizeof(int), &dwByte, nullptr);

			szMapLink = new TCHAR[Size];

			ReadFile(hFile, szMapLink, sizeof(TCHAR) * Size, &dwByte, nullptr);
			m_vecCreateMap[index]->m_tLink.RightMapID = szMapLink;

			delete[] szMapLink;
			szMapLink = nullptr;

			// Map Down Link ID
			ReadFile(hFile, &Size, sizeof(int), &dwByte, nullptr);

			szMapLink = new TCHAR[Size];

			ReadFile(hFile, szMapLink, sizeof(TCHAR) * Size, &dwByte, nullptr);
			m_vecCreateMap[index]->m_tLink.DownMapID = szMapLink;

			delete[] szMapLink;
			szMapLink = nullptr;

			// Map Left Link ID
			ReadFile(hFile, &Size, sizeof(int), &dwByte, nullptr);

			szMapLink = new TCHAR[Size];

			ReadFile(hFile, szMapLink, sizeof(TCHAR) * Size, &dwByte, nullptr);
			m_vecCreateMap[index]->m_tLink.LeftMapID = szMapLink;

			delete[] szMapLink;
			szMapLink = nullptr;

			// 리스트 박스에 추가
			m_MapListBox.AddString(m_vecCreateMap[index]->m_tLink.MyMapID);

			// 맵의 메인 뷰 설정
			m_vecCreateMap[index]->SetMainView(pMainView);

		}

		// 몬스터 데이터 로드
		TCHAR szPath[MAX_STR] = L"";
		lstrcat(szPath, Dlg.GetPathName().GetString());
		PathRemoveExtension(szPath); // 경로에서 확장자 제거
		lstrcat(szPath, L"Monster.dat");
		hFile = CreateFile(szPath, GENERIC_READ, 0, 0,
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

				m_vecCreateMap[index]->m_vecMonster.push_back(pMonster);
			}
		}

		CloseHandle(hFile);
	}

	CheckDlgButton(IDC_CHECK1, TRUE);
	CheckDlgButton(IDC_CHECK2, TRUE);
	CheckDlgButton(IDC_CHECK3, TRUE);

	UpdateData(FALSE);
}

void CForm::OnBnClickedGreedVisible()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrame->m_MainSplit.GetPane(0, 1));

	for (size_t i = 0; i < m_vecCreateMap.size(); ++i)
	{
		// visible 스위치를 비트연산으로 체크
		// true이면 끄고 false면 킨다
		if (m_vecCreateMap[i]->m_dwVisible & VISIBLE_GREED)
			m_vecCreateMap[i]->m_dwVisible ^= VISIBLE_GREED;
		else
			m_vecCreateMap[i]->m_dwVisible |= VISIBLE_GREED;
	}

	pMainView->Invalidate(FALSE);

	UpdateData(FALSE);
}


void CForm::OnBnClickedTileVisible()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrame->m_MainSplit.GetPane(0, 1));

	for (size_t i = 0; i < m_vecCreateMap.size(); ++i)
	{
		if (m_vecCreateMap[i]->m_dwVisible & VISIBLE_TILE)
			m_vecCreateMap[i]->m_dwVisible ^= VISIBLE_TILE;
		else
			m_vecCreateMap[i]->m_dwVisible |= VISIBLE_TILE;
	}
	
	pMainView->Invalidate(FALSE);

	UpdateData(FALSE);
}


void CForm::OnBnClickedObjectVisible()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrame->m_MainSplit.GetPane(0, 1));

	for (size_t i = 0; i < m_vecCreateMap.size(); ++i)
	{
		if (m_vecCreateMap[i]->m_dwVisible & VISIBLE_OBJECT)
			m_vecCreateMap[i]->m_dwVisible ^= VISIBLE_OBJECT;
		else
			m_vecCreateMap[i]->m_dwVisible |= VISIBLE_OBJECT;
	}

	pMainView->Invalidate(FALSE);


	UpdateData(FALSE);
}



void CForm::OnBnClickedSetGround()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrame->m_MainSplit.GetPane(0, 1));

	if (pMainView->m_pTerrain)
	{
		pMainView->m_pTerrain->CheckNeighbor();

		pMainView->Invalidate(FALSE);
	}
	else
		AfxMessageBox(L"맵이 생성되어있지 않습니다!");

	UpdateData(FALSE);
}




void CForm::OnBnClickedDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrame->m_MainSplit.GetPane(0, 1));

	// 생성된 맵이 없을 경우
	if (m_vecCreateMap.empty())
	{
		AfxMessageBox(L"삭제할 데이터가 없습니다!");
		pMainView->m_pTerrain = nullptr;
		return;
	}

	int i = 0;
	// 현재 작업하는 맵을 검색
	auto iter_find = find_if(m_vecCreateMap.begin(), m_vecCreateMap.end(), 
		[&i, &pMainView](auto*& pTerrain)
	{
		++i;
		return pTerrain == pMainView->m_pTerrain;
	});

	// 예외처리
	if (iter_find == m_vecCreateMap.end())
		return;
	
	// 리스트 박스에서 문자열 제거
	m_MapListBox.DeleteString(i-1);

	// 맵 제거
	SafeDelete(*iter_find);
	iter_find = m_vecCreateMap.erase(iter_find);

	// 삭제 후 맵 목록이 빌 경우
	if (m_vecCreateMap.empty())
	{
		pMainView->m_pTerrain = nullptr;
		pMainView->Invalidate(FALSE);

		return;
	}

	pMainView->m_pTerrain = m_vecCreateMap.front();

	pMainView->Invalidate(FALSE);

	UpdateData(FALSE);
}


BOOL CForm::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			OnBnClickedCreateWorld();
			break;
		case VK_DELETE:
			OnBnClickedDelete();
			break;
		}
		break;
	}

	return CFormView::PreTranslateMessage(pMsg);
}


void CForm::OnBnClickedPathFind()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (m_pPathFindDig.GetSafeHwnd() == nullptr)
		m_pPathFindDig.Create(IDD_PATHFINDDIG);

	m_pPathFindDig.ShowWindow(SW_SHOW);
}


void CForm::OnBnClickedSetOption()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplit.GetPane(0, 1));

	if (pMainView->m_bIsSetOption)
		pMainView->m_bIsSetOption = false;
	else
		pMainView->m_bIsSetOption = true;

	UpdateData(FALSE);
}

void CForm::OnBnClickedSetCollision()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplit.GetPane(0, 1));

	if (pMainView->m_bIsCollision)
		pMainView->m_bIsCollision = false;
	else
		pMainView->m_bIsCollision = true;

	UpdateData(FALSE);
}
