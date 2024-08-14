#pragma once
#include "afxwin.h"


// CMapLinkDig ��ȭ �����Դϴ�.

class CMapLinkDig : public CDialog
{
	DECLARE_DYNAMIC(CMapLinkDig)

public:
	CMapLinkDig(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMapLinkDig();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPLINK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeSelectMap();
	afx_msg void OnCbnSelchangeUpLink();
	afx_msg void OnCbnSelchangeRightLink();
	afx_msg void OnCbnSelchangeDownLink();
	afx_msg void OnCbnSelchangeLeftLink();

public:
	// ���� ���õ� ��
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
