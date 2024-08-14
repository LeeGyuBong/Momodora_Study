#pragma once
#include "afxwin.h"


// CTileObj 대화 상자입니다.

class CTileObj : public CPropertyPage
{
	DECLARE_DYNAMIC(CTileObj)

public:
	CTileObj();
	virtual ~CTileObj();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TILEOBJ };
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
