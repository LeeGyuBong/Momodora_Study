#pragma once
#include "afxwin.h"


// CPathFindDig ��ȭ �����Դϴ�.

class CPathFindDig : public CDialog
{
	DECLARE_DYNAMIC(CPathFindDig)

public:
	CPathFindDig(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPathFindDig();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATHFINDDIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedSavePath();
	afx_msg void OnBnClickedLoadPath();

public:
	CListBox m_ListBox;

	list<IMGPATH*>	m_PathInfoLst;
};
