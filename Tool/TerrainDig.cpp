// TerrainDig.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "TerrainDig.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "ToolView.h"

// CTerrainDig ��ȭ �����Դϴ�.

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


// CTerrainDig �޽��� ó�����Դϴ�.


BOOL CTerrainDig::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	CRect rc;
	this->GetWindowRect(&rc);

	m_pMySheet = new CMySheet;
	m_pMySheet->Create(this, WS_CHILD | WS_VISIBLE);
	m_pMySheet->MoveWindow(0, 0, rc.Width(), rc.Height());

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CTerrainDig::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

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