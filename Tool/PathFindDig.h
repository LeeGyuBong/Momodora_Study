#pragma once
#include "afxwin.h"


// CPathFindDig 대화 상자입니다.

class CPathFindDig : public CDialog
{
	DECLARE_DYNAMIC(CPathFindDig)

public:
	CPathFindDig(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPathFindDig();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATHFINDDIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedSavePath();
	afx_msg void OnBnClickedLoadPath();

public:
	CListBox m_ListBox;

	list<IMGPATH*>	m_PathInfoLst;
};
