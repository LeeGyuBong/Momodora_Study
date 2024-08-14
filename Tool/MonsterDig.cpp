// MonsterDig.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MonsterDig.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "ToolView.h"

// CMonsterDig ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMonsterDig, CDialog)

CMonsterDig::CMonsterDig(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MONSTERDIG, pParent)
	, m_strID(_T(""))
	, m_iHp(0)
	, m_strVisibleID(_T(""))
	, m_iVisibleHp(0)
	, m_bISImageSelect(false)
	, m_SelectMonster(nullptr)
{
}

CMonsterDig::~CMonsterDig()
{
	Release();
}

void CMonsterDig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ImageListBox);
	DDX_Control(pDX, IDC_LIST2, m_CompleteListBox);
	DDX_Control(pDX, IDC_MonsterPicture, m_MonsterPicture);
	DDX_Text(pDX, IDC_EDIT1, m_strID);
	DDX_Text(pDX, IDC_EDIT2, m_iHp);
	DDX_Text(pDX, IDC_EDIT7, m_strVisibleID);
	DDX_Text(pDX, IDC_EDIT8, m_iVisibleHp);
}


BEGIN_MESSAGE_MAP(CMonsterDig, CDialog)
	ON_WM_SYSCOMMAND()
	ON_LBN_SELCHANGE(IDC_LIST1, &CMonsterDig::OnLbnSelchangeImageList)
	ON_LBN_SELCHANGE(IDC_LIST2, &CMonsterDig::OnLbnSelchangeCompleteList)
	ON_BN_CLICKED(IDC_BUTTON1, &CMonsterDig::OnBnClickedCreateMonsterInfo)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CMonsterDig �޽��� ó�����Դϴ�.


void CMonsterDig::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialog::OnSysCommand(nID, lParam);

	if ((nID & 0xFFF0) == SC_CLOSE)
	{
		EndDialog(IDCANCEL);

		CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplit.GetPane(0, 1));
		pMainView->m_bIsMonsterDigOn = false;

		pMainView->pObjKey = L"MapTool";
		pMainView->pStateKey = L"Greed";
		pMainView->m_iVDrawID = 0;

		return;
	}
}


void CMonsterDig::OnLbnSelchangeImageList()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	int index = m_ImageListBox.GetCurSel();

	if (LB_ERR == index)
		return;

	CString strSelectName;
	m_ImageListBox.GetText(index, strSelectName);

	auto iter_find = m_mapImg.find(strSelectName);

	if (iter_find == m_mapImg.end())
		return;

	m_MonsterPicture.SetBitmap(*(iter_find->second));

	int i = 0;
	for (; i < strSelectName.GetLength(); ++i)
	{
		if (isdigit(strSelectName[i]) != 0)
			break;
	}
	strSelectName.Delete(0, i);

	m_iImgaeID = _ttoi(strSelectName);

	m_bISImageSelect = true;

	UpdateData(FALSE);
}


void CMonsterDig::OnLbnSelchangeCompleteList()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	int index = m_CompleteListBox.GetCurSel();

	if (LB_ERR == index)
		return;

	list<MONSTER*>::iterator iter_begin = m_MonsterList.begin();

	for (int i = 0; i < index; ++i)
		++iter_begin;

	m_SelectMonster = (*iter_begin);

	m_strVisibleID = m_SelectMonster->strID;
	m_iVisibleHp = m_SelectMonster->HP;

	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrame->m_MainSplit.GetPane(0, 1));
	pMainView->pObjKey = L"MapTool";
	pMainView->pStateKey = L"Monster";
	pMainView->m_iVDrawID = m_SelectMonster->byDrawID;

	pMainView->Invalidate(FALSE);

	UpdateData(FALSE);
}


void CMonsterDig::OnBnClickedCreateMonsterInfo()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	if (!m_bISImageSelect)
	{
		AfxMessageBox(L"�̹����� ������ �ּ���.");
		return;
	}
	if (m_strID == L"")
	{
		AfxMessageBox(L"���̵� �Է��� �ּ���.");
		return;
	}
	if (m_iHp == 0)
	{
		AfxMessageBox(L"ü���� �Է��� �ּ���.");
		return;
	}

	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrame->m_MainSplit.GetPane(0, 1));

	MONSTER* pMonster = new MONSTER;

	pMonster->strID = m_strID;
	pMonster->HP = m_iHp;
	pMonster->byDrawID = m_iImgaeID;
	
	m_MonsterList.push_back(pMonster);

	m_CompleteListBox.AddString(pMonster->strID);

	m_bISImageSelect = false;

	m_strID = L"";
	m_iHp = 0;

	UpdateData(FALSE);
}

void CMonsterDig::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialog::OnDropFiles(hDropInfo);

	UpdateData(TRUE);

	// ���� ����Ʈ�ڽ� �ʱ�ȭ
	m_ImageListBox.ResetContent();

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
			m_ImageListBox.AddString(strFileName);
		}
	}

	UpdateData(FALSE);
}

void CMonsterDig::Initialize()
{
	UpdateData(TRUE);

	CFileFind finder;
	int FileCount = 0;
	
	// ��� �� ��� ����
	TCHAR szFilePath[MAX_STR] = L"../Data/Texture/MapTool/Monster/*.*";

	BOOL working = finder.FindFile(szFilePath);

	while (working)
	{
		working = finder.FindNextFile();
		if (finder.IsDots())
			continue;

		// ��γ� ���ϸ�鸸 �޾ƿ�
		CString strFileName = finder.GetFileTitle();
		
		CString strFilePath = szFilePath;

		// ��ο��� *.*�� �����ϰ� ���ϸ�� Ȯ���ڸ� ����
		strFilePath.Delete(lstrlen(szFilePath)-3, 3);
		strFilePath += strFileName;
		strFilePath += L".png";

		auto iter_find = m_mapImg.find(strFileName);
		if (iter_find == m_mapImg.end())
		{
			CImage* pImage = new CImage;

			pImage->Load(strFilePath.GetString());

			m_mapImg.insert({ strFileName, pImage });
			m_ImageListBox.AddString(strFileName);
		}

	}
	finder.Close();

	UpdateData(FALSE);
}



void CMonsterDig::Release()
{
	for_each(m_mapImg.begin(), m_mapImg.end(),
		[](auto& MyPair)
	{
		MyPair.second->Destroy();
		SafeDelete(MyPair.second);
	});
	m_mapImg.clear();

	for_each(m_MonsterList.begin(), m_MonsterList.end(), SafeDelete<MONSTER*>);
	m_MonsterList.clear();
}

BOOL CMonsterDig::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	Initialize();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


BOOL CMonsterDig::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			OnBnClickedCreateMonsterInfo();
			return true;
		}
		break;
	}

	return CDialog::PreTranslateMessage(pMsg);
}
