// TerrainDig.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "TerrainDig.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "ToolView.h"

// CTerrainDig 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTerrainDig, CDialog)

CTerrainDig::CTerrainDig(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_TERRAINDIG, pParent)
{

}

CTerrainDig::~CTerrainDig()
{
	SafeDelete(m_pMySheet);
}

void CTerrainDig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTerrainDig, CDialog)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// CTerrainDig 메시지 처리기입니다.


BOOL CTerrainDig::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	CRect rc;
	this->GetWindowRect(&rc);

	m_pMySheet = new CMySheet;
	m_pMySheet->Create(this, WS_CHILD | WS_VISIBLE);
	m_pMySheet->MoveWindow(0, 0, rc.Width(), rc.Height());

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CTerrainDig::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnSysCommand(nID, lParam);

	if ((nID & 0xFFF0) == SC_CLOSE)
	{
		EndDialog(IDCANCEL);

		CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplit.GetPane(0, 1));
		pMainView->m_bIsTerrainDigOn = OFF;

		pMainView->pObjKey = L"MapTool";
		pMainView->pStateKey = L"Greed";
		pMainView->m_iVDrawID = 0;

		pMainView->m_eCurSheet = CToolView::END;

		return;
	}
}