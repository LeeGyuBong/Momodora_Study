#pragma once


// CTerrainDig ��ȭ �����Դϴ�.
#include "MySheet.h"
#include "afxcmn.h"

class CTerrainDig : public CDialog
{
	DECLARE_DYNAMIC(CTerrainDig)

public:
	CTerrainDig(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTerrainDig();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TERRAINDIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);

public:
	CMySheet* m_pMySheet;
};
