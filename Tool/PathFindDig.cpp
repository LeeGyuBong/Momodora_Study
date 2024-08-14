// PathFindDig.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "PathFindDig.h"
#include "afxdialogex.h"


// CPathFindDig 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPathFindDig, CDialog)

CPathFindDig::CPathFindDig(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PATHFINDDIG, pParent)
{

}

CPathFindDig::~CPathFindDig()
{
}

void CPathFindDig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
}


BEGIN_MESSAGE_MAP(CPathFindDig, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CPathFindDig::OnBnClickedSavePath)
	ON_BN_CLICKED(IDC_BUTTON2, &CPathFindDig::OnBnClickedLoadPath)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CPathFindDig 메시지 처리기입니다.

void CPathFindDig::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnDropFiles(hDropInfo);

	UpdateData(TRUE);

	for_each(m_PathInfoLst.begin(), m_PathInfoLst.end(), SafeDelete<IMGPATH*>);
	m_PathInfoLst.clear();

	m_ListBox.ResetContent();


	int count = DragQueryFile(hDropInfo, -1, nullptr, 0);

	TCHAR szFullPath[MAX_STR] = L"";

	for (int i = 0; i < count; ++i)
	{
		DragQueryFile(hDropInfo, i, szFullPath, MAX_STR);
		CFileInfo::DirInfoExtraction(szFullPath, m_PathInfoLst);
	}

	wstring wstrCombine = L"";
	TCHAR szCount[MIN_STR] = L"";

	for (auto& pImgPath : m_PathInfoLst)
	{
		_itow_s(pImgPath->iCount, szCount, 10);

		wstrCombine = pImgPath->wstrObjKey + L"|" + pImgPath->wstrStateKey +
			L"|" + szCount + L"|" + pImgPath->wstrPath;

		m_ListBox.AddString(wstrCombine.c_str());
	}


	UpdateData(FALSE);
}

void CPathFindDig::OnBnClickedSavePath()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	wofstream fout;

	fout.open(L"../Data/MapData/ImagePath.txt");

	if (fout.fail())
	{
		AfxMessageBox(L"ImagePath Save Failed!");
		return;
	}

	for (auto& pImgPath : m_PathInfoLst)
	{
		fout << pImgPath->wstrObjKey << L"|" << pImgPath->wstrStateKey << L"|" <<
			pImgPath->iCount << L"|" << pImgPath->wstrPath << endl;
	}

	fout.close();

	WinExec("notepad.exe ../Data/MapData/ImagePath.txt", SW_SHOW);
}


void CPathFindDig::OnBnClickedLoadPath()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	wifstream fin;

	fin.open(L"../Data/MapData/ImagePath.txt");

	if (fin.fail())
	{
		AfxMessageBox(L"ImagePath Load Failed!");
		return;
	}

	for_each(m_PathInfoLst.begin(), m_PathInfoLst.end(), SafeDelete<IMGPATH*>);
	m_PathInfoLst.clear();

	m_ListBox.ResetContent();

	TCHAR szObjKey[MAX_STR] = L"";
	TCHAR szStateKey[MAX_STR] = L"";
	TCHAR szCount[MIN_STR] = L"";
	TCHAR szPath[MAX_STR] = L"";

	wstring strCombine = L"";

	while (true)
	{
		fin.getline(szObjKey, MAX_STR, '|');
		fin.getline(szStateKey, MAX_STR, '|');
		fin.getline(szCount, MIN_STR, '|');
		fin.getline(szPath, MAX_STR);

		if (fin.eof())
			break;

		strCombine = wstring(szObjKey) + L"|" + szStateKey + L"|" + szCount + L"|"
			+ szPath;

		m_ListBox.AddString(strCombine.c_str());

		IMGPATH* pImgPath = new IMGPATH;

		pImgPath->wstrObjKey = szObjKey;
		pImgPath->wstrStateKey = szStateKey;
		pImgPath->wstrPath = szPath;

		// _ttoi: 문자열 -> 정수로 변환.
		pImgPath->iCount = _ttoi(szCount);

		m_PathInfoLst.push_back(pImgPath);
	}

	fin.close();

	UpdateData(FALSE);
}