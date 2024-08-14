// TileObj.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "TileObj.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "ToolView.h"

// CTileObj ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CTileObj, CPropertyPage)

CTileObj::CTileObj()
	: CPropertyPage(IDD_TILEOBJ), m_fReversalX(1.f), m_fReversalY(1.f)
{
	
}

CTileObj::~CTileObj()
{
	Release();
}

void CTileObj::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_TilePicture, m_TilePicture);
}


BEGIN_MESSAGE_MAP(CTileObj, CPropertyPage)
	ON_LBN_SELCHANGE(IDC_LIST1, &CTileObj::OnLbnSelchangeSelectList)
	ON_BN_CLICKED(IDC_CHECK1, &CTileObj::OnBnClickedReversalX)
	ON_BN_CLICKED(IDC_CHECK2, &CTileObj::OnBnClickedReversalY)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CTileObj �޽��� ó�����Դϴ�.


void CTileObj::OnLbnSelchangeSelectList()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	int index = m_ListBox.GetCurSel();

	if (LB_ERR == index)
		return;

	CString strSelectName;
	m_ListBox.GetText(index, strSelectName);

	auto iter_find = m_mapImg.find(strSelectName);

	if (iter_find == m_mapImg.end())
		return;

	m_TilePicture.SetBitmap(*(iter_find->second));

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
	pMainView->pStateKey = L"Tile";
	pMainView->m_iVDrawID = m_iDrawID;
	pMainView->m_eCurSheet = CToolView::MAPTILE;

	pMainView->Invalidate(FALSE);

	UpdateData(FALSE);
}


void CTileObj::OnBnClickedReversalX()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	m_fReversalX *= -1.f;

	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrame->m_MainSplit.GetPane(0, 1));
	pMainView->m_fReversalX = m_fReversalX;

	pMainView->Invalidate(FALSE);

	UpdateData(FALSE);
}


void CTileObj::OnBnClickedReversalY()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	m_fReversalY *= -1.f;

	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrame->m_MainSplit.GetPane(0, 1));
	pMainView->m_fReversalY = m_fReversalY;

	pMainView->Invalidate(FALSE);

	UpdateData(FALSE);
}


void CTileObj::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CPropertyPage::OnDropFiles(hDropInfo);

	UpdateData(TRUE);

	// ���� ����Ʈ�ڽ� �ʱ�ȭ
	m_ListBox.ResetContent();

	// �巡�׵� ���� ������ �޾ƿ�
	int FileCount = DragQueryFile(hDropInfo, -1, nullptr, 0);

	TCHAR szFilePath[MAX_STR] = L"";
	TCHAR szFileName[MAX_STR] = L"";
	for (int i = 0; i < FileCount; ++i)
	{
		// �巡�׵� ������ ��θ� ����
		DragQueryFile(hDropInfo, i, szFilePath, MAX_STR);

		// ���� ��ο��� ���ϸ� ����
		CString strFileName = PathFindFileName(szFilePath);
		lstrcpy(szFileName, strFileName.GetString());
		// ���ϸ��� Ȯ���ڸ� ����
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

void CTileObj::Initialize()
{
	UpdateData(TRUE);

	CFileFind finder;
	int FileCount = 0;
	TCHAR szFilePath[MAX_STR] = L"../Data/Texture/MapTool/Tile/*.*";

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

void CTileObj::Release()
{
	for_each(m_mapImg.begin(), m_mapImg.end(),
		[](auto& MyPair)
	{
		MyPair.second->Destroy();
		SafeDelete(MyPair.second);
	});
	m_mapImg.clear();
}

BOOL CTileObj::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	Initialize();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
