#pragma once
#include "afxwin.h"


// CBackObj 대화 상자입니다.

class CBackObj : public CPropertyPage
{
	DECLARE_DYNAMIC(CBackObj)

public:
	CBackObj();
	virtual ~CBackObj();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BACKOBJ };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeSelectList();
	afx_msg void OnBnClickedReversalX();
	afx_msg void OnBnClickedReversalY();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedChangeType();

	void Initialize();

	void Release();

public:
	// Controll
	CListBox m_ListBox;
	CStatic m_BackPicture;

	// Value

	// User
	map <CString, CImage*> m_mapImg;
	BYTE m_iDrawID;
	float m_fReversalX;
	float m_fReversalY;

	int m_iUnObjType;
};
