#pragma once
#include "afxwin.h"


// CMonsterDig 대화 상자입니다.

class CMonsterDig : public CDialog
{
	DECLARE_DYNAMIC(CMonsterDig)

public:
	CMonsterDig(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMonsterDig();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MONSTERDIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
	// 이미지 목록 리스트 박스
	CListBox m_ImageListBox;
	// 완성 리스트 박스
	CListBox m_CompleteListBox;
	CStatic m_MonsterPicture;

	// -------------- Value
	// 입력 아이디 변수
	CString m_strID;
	// 입력 체력 변수
	int m_iHp;
	// 아이디값 표시 변수
	CString m_strVisibleID;
	// 체력 표시 변수
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
