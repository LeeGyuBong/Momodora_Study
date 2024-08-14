#pragma once
#include "afxwin.h"


// CMonsterDig ��ȭ �����Դϴ�.

class CMonsterDig : public CDialog
{
	DECLARE_DYNAMIC(CMonsterDig)

public:
	CMonsterDig(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMonsterDig();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MONSTERDIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnLbnSelchangeImageList();
	afx_msg void OnLbnSelchangeCompleteList();
	afx_msg void OnBnClickedCreateMonsterInfo();
	afx_msg void OnDropFiles(HDROP hDropInfo);

	void Initialize();

	void Release();

public:
	// -------------- Controll
	// �̹��� ��� ����Ʈ �ڽ�
	CListBox m_ImageListBox;
	// �ϼ� ����Ʈ �ڽ�
	CListBox m_CompleteListBox;
	CStatic m_MonsterPicture;

	// -------------- Value
	// �Է� ���̵� ����
	CString m_strID;
	// �Է� ü�� ����
	int m_iHp;
	// ���̵� ǥ�� ����
	CString m_strVisibleID;
	// ü�� ǥ�� ����
	int m_iVisibleHp;

	// -------------- User
	map<CString, CImage*> m_mapImg;
	list<MONSTER*> m_MonsterList;
	MONSTER* m_SelectMonster;

	BYTE m_iDrawID;
	BYTE m_iImgaeID;

	bool m_bISImageSelect;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
