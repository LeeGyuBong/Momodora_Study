#pragma once
#include "afxwin.h"


// CTileObj ��ȭ �����Դϴ�.

class CTileObj : public CPropertyPage
{
	DECLARE_DYNAMIC(CTileObj)

public:
	CTileObj();
	virtual ~CTileObj();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TILEOBJ };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeSelectList();
	afx_msg void OnBnClickedReversalX();
	afx_msg void OnBnClickedReversalY();
	afx_msg void OnDropFiles(HDROP hDropInfo);

	void Initialize();

	void Release();

public:
	// controll
	CListBox m_ListBox;
	CStatic m_TilePicture;

	// value

	// user
	map <CString, CImage*> m_mapImg;
	BYTE m_iDrawID;
	float m_fReversalX;
	float m_fReversalY;
	virtual BOOL OnInitDialog();
};
