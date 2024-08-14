// MapLinkDig.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapLinkDig.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "Form.h"
#include "ToolView.h"
#include "Terrain.h"

// CMapLinkDig 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMapLinkDig, CDialog)

CMapLinkDig::CMapLinkDig(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAPLINK, pParent)
	, m_strMapName(_T(""))
	, m_SelectUpLink(_T(""))
	, m_SelectRightLink(_T(""))
	, m_SelectDownLink(_T(""))
	, m_SelectLeftLink(_T(""))
{

}

CMapLinkDig::~CMapLinkDig()
{
}

void CMapLinkDig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strMapName);
	DDX_Control(pDX, IDC_COMBO1, m_UpLinkList);
	DDX_Control(pDX, IDC_COMBO2, m_RightLinkList);
	DDX_Control(pDX, IDC_COMBO3, m_DownLinkList);
	DDX_Control(pDX, IDC_COMBO4, m_LeftLinkList);
	DDX_Control(pDX, IDC_LIST1, m_LinkListBox);
	DDX_CBString(pDX, IDC_COMBO1, m_SelectUpLink);
	DDX_CBString(pDX, IDC_COMBO2, m_SelectRightLink);
	DDX_CBString(pDX, IDC_COMBO3, m_SelectDownLink);
	DDX_CBString(pDX, IDC_COMBO4, m_SelectLeftLink);
}


BEGIN_MESSAGE_MAP(CMapLinkDig, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapLinkDig::OnLbnSelchangeSelectMap)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CMapLinkDig::OnCbnSelchangeUpLink)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CMapLinkDig::OnCbnSelchangeRightLink)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CMapLinkDig::OnCbnSelchangeDownLink)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CMapLinkDig::OnCbnSelchangeLeftLink)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// CMapLinkDig 메시지 처리기입니다.

void CMapLinkDig::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnSysCommand(nID, lParam);

	if ((nID & 0xFFF0) == SC_CLOSE)
	{
		EndDialog(IDCANCEL);

		CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplit.GetPane(0, 1));
		pMainView->m_bIsMapLink = false;

		pMainView->Invalidate(FALSE);

		return;
	}
}

void CMapLinkDig::OnLbnSelchangeSelectMap()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	m_LeftLinkList.ResetContent();
	m_RightLinkList.ResetContent();
	m_UpLinkList.ResetContent();
	m_DownLinkList.ResetContent();

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CForm* pForm = dynamic_cast<CForm*>(pMainFrm->m_MainSplit.GetPane(0, 0));

	int index = m_LinkListBox.GetCurSel();

	if (LB_ERR == index)
		return;

	CString m_strSelectID;
	m_LinkListBox.GetText(index, m_strSelectID);

	// 선택한 리스트값의 ID를 비교 검사
	auto iter_find = find_if(pForm->m_vecCreateMap.begin(), pForm->m_vecCreateMap.end(),
		[&m_strSelectID](CTerrain*& pTerrain)
	{
		return pTerrain->m_tLink.MyMapID == m_strSelectID;
	});

	if (iter_find == pForm->m_vecCreateMap.end())
		return;

	m_strMapName = (*iter_find)->m_tLink.MyMapID;

	m_SelectUpLink = (*iter_find)->m_tLink.UpMapID;
	m_SelectLeftLink = (*iter_find)->m_tLink.LeftMapID;
	m_SelectRightLink = (*iter_find)->m_tLink.RightMapID;
	m_SelectDownLink = (*iter_find)->m_tLink.DownMapID;

	// 콤보박스에 선택할 수 있는 데이터 목록 추가
	m_UpLinkList.AddString(L"");
	m_LeftLinkList.AddString(L"");
	m_RightLinkList.AddString(L"");
	m_DownLinkList.AddString(L"");
	for (size_t i = 0; i < pForm->m_vecCreateMap.size(); ++i)
	{
		if (pForm->m_vecCreateMap[i]->m_tLink.MyMapID == m_strMapName)
			continue;

		m_UpLinkList.AddString(pForm->m_vecCreateMap[i]->m_tLink.MyMapID);
		m_LeftLinkList.AddString(pForm->m_vecCreateMap[i]->m_tLink.MyMapID);
		m_RightLinkList.AddString(pForm->m_vecCreateMap[i]->m_tLink.MyMapID);
		m_DownLinkList.AddString(pForm->m_vecCreateMap[i]->m_tLink.MyMapID);
	}

	UpdateData(FALSE);
}

void CMapLinkDig::OnCbnSelchangeUpLink()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	UpdateData(TRUE);

	int index = m_UpLinkList.GetCurSel();

	m_UpLinkList.GetLBText(index, m_SelectUpLink);

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CForm* pForm = dynamic_cast<CForm*>(pMainFrm->m_MainSplit.GetPane(0, 0));
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplit.GetPane(0, 1));

	// -------- 현재 선택된 맵의 링크 설정
	CString strID = m_strMapName;
	// 선택된 맵의 ID를 받아와서 생성된 맵들의 ID를 비교 검사
	auto iter_find = find_if(pForm->m_vecCreateMap.begin(), pForm->m_vecCreateMap.end(),
		[&strID](CTerrain*& pTerrain)
	{
		return pTerrain->m_tLink.MyMapID == strID;
	});

	if (iter_find == pForm->m_vecCreateMap.end())
		return;

	// 찾아낸 맵에 링크할 맵을 대입
	(*iter_find)->m_tLink.UpMapID = m_SelectUpLink;

	// 링크할 맵도 자동으로 현재 선택된 값으로 바뀌게 // ex) UpLink를 해주면 UpLink되는 맵의 DownLink를 자동 설정
	iter_find = find_if(pForm->m_vecCreateMap.begin(), pForm->m_vecCreateMap.end(),
		[&iter_find](CTerrain*& pTerrain)
	{
		return pTerrain->m_tLink.MyMapID == (*iter_find)->m_tLink.UpMapID;
	});

	if (iter_find != pForm->m_vecCreateMap.end())
		(*iter_find)->m_tLink.DownMapID = m_strMapName;

	pMainView->InitLinkViewPos();
	pMainView->Invalidate(FALSE);

	UpdateData(FALSE);
}


void CMapLinkDig::OnCbnSelchangeRightLink()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	int index = m_RightLinkList.GetCurSel();

	m_RightLinkList.GetLBText(index, m_SelectRightLink);

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CForm* pForm = dynamic_cast<CForm*>(pMainFrm->m_MainSplit.GetPane(0, 0));
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplit.GetPane(0, 1));

	CString str = m_strMapName;
	auto iter_find = find_if(pForm->m_vecCreateMap.begin(), pForm->m_vecCreateMap.end(),
		[&str](CTerrain*& pTerrain)
	{
		return pTerrain->m_tLink.MyMapID == str;
	});

	if (iter_find == pForm->m_vecCreateMap.end())
		return;

	(*iter_find)->m_tLink.RightMapID = m_SelectRightLink;

	iter_find = find_if(pForm->m_vecCreateMap.begin(), pForm->m_vecCreateMap.end(),
		[&iter_find](CTerrain*& pTerrain)
	{
		return pTerrain->m_tLink.MyMapID == (*iter_find)->m_tLink.RightMapID;
	});

	if (iter_find != pForm->m_vecCreateMap.end())
		(*iter_find)->m_tLink.LeftMapID = m_strMapName;

	pMainView->InitLinkViewPos();
	pMainView->Invalidate(FALSE);

	UpdateData(FALSE);
}


void CMapLinkDig::OnCbnSelchangeDownLink()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	int index = m_DownLinkList.GetCurSel();

	m_DownLinkList.GetLBText(index, m_SelectDownLink);

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CForm* pForm = dynamic_cast<CForm*>(pMainFrm->m_MainSplit.GetPane(0, 0));
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplit.GetPane(0, 1));

	CString str = m_strMapName;
	auto iter_find = find_if(pForm->m_vecCreateMap.begin(), pForm->m_vecCreateMap.end(),
		[&str](CTerrain*& pTerrain)
	{
		return pTerrain->m_tLink.MyMapID == str;
	});

	if (iter_find == pForm->m_vecCreateMap.end())
		return;

	(*iter_find)->m_tLink.DownMapID = m_SelectDownLink;

	iter_find = find_if(pForm->m_vecCreateMap.begin(), pForm->m_vecCreateMap.end(),
		[&iter_find](CTerrain*& pTerrain)
	{
		return pTerrain->m_tLink.MyMapID == (*iter_find)->m_tLink.DownMapID;
	});

	if (iter_find != pForm->m_vecCreateMap.end())
		(*iter_find)->m_tLink.UpMapID = m_strMapName;

	pMainView->InitLinkViewPos();
	pMainView->Invalidate(FALSE);

	UpdateData(FALSE);
}


void CMapLinkDig::OnCbnSelchangeLeftLink()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	int index = m_LeftLinkList.GetCurSel();

	m_LeftLinkList.GetLBText(index, m_SelectLeftLink);

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CForm* pForm = dynamic_cast<CForm*>(pMainFrm->m_MainSplit.GetPane(0, 0));
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplit.GetPane(0, 1));

	CString str = m_strMapName;
	auto iter_find = find_if(pForm->m_vecCreateMap.begin(), pForm->m_vecCreateMap.end(),
		[&str](CTerrain*& pTerrain)
	{
		return pTerrain->m_tLink.MyMapID == str;
	});

	if (iter_find == pForm->m_vecCreateMap.end())
		return;

	(*iter_find)->m_tLink.LeftMapID = m_SelectLeftLink;

	iter_find = find_if(pForm->m_vecCreateMap.begin(), pForm->m_vecCreateMap.end(),
		[&iter_find](CTerrain*& pTerrain)
	{
		return pTerrain->m_tLink.MyMapID == (*iter_find)->m_tLink.LeftMapID;
	});

	if (iter_find != pForm->m_vecCreateMap.end())
		(*iter_find)->m_tLink.RightMapID = m_strMapName;

	pMainView->InitLinkViewPos();
	pMainView->Invalidate(FALSE);

	UpdateData(FALSE);
}