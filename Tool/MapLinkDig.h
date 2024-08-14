#pragma once
#include "afxwin.h"


// CMapLinkDig 대화 상자입니다.

class CMapLinkDig : public CDialog
{
	DECLARE_DYNAMIC(CMapLinkDig)

public:
	CMapLinkDig(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMapLinkDig();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPLINK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeSelectMap();
	afx_msg void OnCbnSelchangeUpLink();
	afx_msg void OnCbnSelchangeRightLink();
	afx_msg void OnCbnSelchangeDownLink();
	afx_msg void OnCbnSelchangeLeftLink();

public:
	// 현재 선택된 맵
	CString m_strMapName;
	
	CListBox m_LinkListBox;

	CComboBox m_UpLinkList;
	CComboBox m_RightLinkList;
	CComboBox m_DownLinkList;
	CComboBox m_LeftLinkList;
	
	CString m_SelectUpLink;
	CString m_SelectRightLink;
	CString m_SelectDownLink;
	CString m_SelectLeftLink;
	
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};
