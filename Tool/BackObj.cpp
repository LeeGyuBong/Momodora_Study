// BackObj.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "BackObj.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "ToolView.h"

// CBackObj 대화 상자입니다.

IMPLEMENT_DYNAMIC(CBackObj, CPropertyPage)

CBackObj::CBackObj()
	: CPropertyPage(IDD_BACKOBJ), m_fReversalX(1.f), m_fReversalY(1.f), m_iUnObjType(0)
{
	
}

CBackObj::~CBackObj()
{
	Release();
}

void CBackObj::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_BackPicture, m_BackPicture);
}


BEGIN_MESSAGE_MAP(CBackObj, CPropertyPage)
	ON_LBN_SELCHANGE(IDC_LIST1, &CBackObj::OnLbnSelchangeSelectList)
	ON_BN_CLICKED(IDC_CHECK1, &CBackObj::OnBnClickedReversalX)
	ON_BN_CLICKED(IDC_CHECK2, &CBackObj::OnBnClickedReversalY)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_CHECK3, &CBackObj::OnBnClickedChangeType)
END_MESSAGE_MAP()


// CBackObj 메시지 처리기입니다.


void CBackObj::OnLbnSelchangeSelectList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	int index = m_ListBox.GetCurSel();

	if (LB_ERR == index)
		return;

	CString strSelectName;
	m_ListBox.GetText(index, strSelectName);

	auto iter_find = m_mapImg.find(strSelectName);

	if (iter_find == m_mapImg.end())
		return;

	m_BackPicture.SetBitmap(*(iter_find->second));


	int i = 0;
	for (; i < strSelectName.GetLength(); ++i)
	{
		if (isdigit(strSelectName[i]) != 0)
			break;
	}
	strSelectName.Delete(0, i);

	m_iDrawID = _ttoi(strSelectName);

	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrame->m_MainSplit.GetPane(0, 1));
	pMainView->pObjKey = L"MapTool";
	pMainView->pStateKey = L"TileObj";
	pMainView->m_iVDrawID = m_iDrawID;
	pMainView->m_eCurSheet = CToolView::UNOBJ;

	pMainView->Invalidate(FALSE);

	UpdateData(FALSE);
}


void CBackObj::OnBnClickedReversalX()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	m_fReversalX *= -1.f;

	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrame->m_MainSplit.GetPane(0, 1));
	pMainView->m_fReversalX = m_fReversalX;

	pMainView->Invalidate(FALSE);

	UpdateData(FALSE);
}


void CBackObj::OnBnClickedReversalY()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	m_fReversalY *= -1.f;

	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrame->m_MainSplit.GetPane(0, 1));
	pMainView->m_fReversalY = m_fReversalY;

	pMainView->Invalidate(FALSE);

	UpdateData(FALSE);
}

void CBackObj::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CPropertyPage::OnDropFiles(hDropInfo);

	UpdateData(TRUE);

	// 현재 리스트박스 초기화
	m_ListBox.ResetContent();

	// 드래그된 파일 갯수를 받아옴
	int FileCount = DragQueryFile(hDropInfo, -1, nullptr, 0);

	TCHAR szFilePath[MAX_STR] = L"";
	TCHAR szFileName[MAX_STR] = L"";
	for (int i = 0; i < FileCount; ++i)
	{
		// 드래그된 파일의 경로를 받음
		DragQueryFile(hDropInfo, i, szFilePath, MAX_STR);

		// 받은 경로에서 파일명만 추출
		CString strFileName = PathFindFileName(szFilePath);
		lstrcpy(szFileName, strFileName.GetString());
		// 파일명에서 확장자를 제거
		PathRemoveExtension(szFileName);

		strFileName = szFileName;

		auto iter_find = m_mapImg.find(strFileName);

		if (iter_find == m_mapImg.end())
		{
			CImage* pImage = new CImage;

			pImage->Load(szFilePath);

			m_mapImg.insert({ strFileName, pImage });
			m_ListBox.AddString(strFileName);
		}
	}

	UpdateData(FALSE);
}

void CBackObj::Initialize()
{
	UpdateData(TRUE);

	CFileFind finder;
	int FileCount = 0;
	TCHAR szFilePath[MAX_STR] = L"../Data/Texture/MapTool/TileObj/*.*";

	BOOL working = finder.FindFile(szFilePath);

	while (working)
	{
		working = finder.FindNextFile();
		if (finder.IsDots())
			continue;

		CString strFileName = finder.GetFileTitle();

		CString strFilePath = szFilePath;

		strFilePath.Delete(lstrlen(szFilePath) - 3, 3);
		strFilePath += strFileName;
		strFilePath += L".png";

		auto iter_find = m_mapImg.find(strFileName);
		if (iter_find == m_mapImg.end())
		{
			CImage* pImage = new CImage;

			pImage->Load(strFilePath.GetString());

			m_mapImg.insert({ strFileName, pImage });
			m_ListBox.AddString(strFileName);
		}

	}
	finder.Close();

	UpdateData(FALSE);
}

void CBackObj::Release()
{
	for_each(m_mapImg.begin(), m_mapImg.end(),
		[](auto& MyPair)
	{
		MyPair.second->Destroy();
		SafeDelete(MyPair.second);
	});
	m_mapImg.clear();
}

BOOL CBackObj::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	Initialize();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CBackObj::OnBnClickedChangeType()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (m_iUnObjType)
		m_iUnObjType = 0;
	else
		m_iUnObjType = 1;
}
