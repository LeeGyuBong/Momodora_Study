#pragma once


// CTerrainDig 대화 상자입니다.
#include "MySheet.h"
#include "afxcmn.h"

class CTerrainDig : public CDialog
{
	DECLARE_DYNAMIC(CTerrainDig)

public:
	CTerrainDig(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTerrainDig();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TERRAINDIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);

public:
	CMySheet* m_pMySheet;
};
