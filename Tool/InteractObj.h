#pragma once
#include "afxwin.h"


// CInteractObj 대화 상자입니다.

class CInteractObj : public CPropertyPage
{
	DECLARE_DYNAMIC(CInteractObj)

public:
	CInteractObj();
	virtual ~CInteractObj();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INTERACTOBJ };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeSelectList();
	afx_msg void OnBnClickedReversalX();
	afx_msg void OnBnClickedReversalY();
	afx_msg void OnDropFiles(HDROP hDropInfo);

	void Initialize();
	void Release();

public:
	// Control
	CListBox m_ListBox;
	CStatic m_Picture;

	// Value

	// User
	map <CString, CImage*> m_mapImg;
	BYTE m_iDrawID;
	float m_fReversalX;
	float m_fReversalY;
	virtual BOOL OnInitDialog();
};
