#pragma once
#include "afxwin.h"

#include "TerrainDig.h"
#include "MonsterDig.h"
#include "MapLinkDig.h"
#include "PathFindDig.h"

// CForm 폼 뷰입니다.

class CTerrain;
class CForm : public CFormView
{
	DECLARE_DYNCREATE(CForm)

protected:
	CForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	// 월드 생성
	afx_msg void OnBnClickedCreateWorld();
	// 맵 데이터 리스트
	afx_msg void OnLbnSelchangeMapListBox();
	// 지형 에디터 오픈
	afx_msg void OnBnClickedOpenTerrainEdit();
	// 몬스터 에디터 오픈
	afx_msg void OnBnClickedOpenMonsterEdit();
	// 맵 링크 에디터 오픈
	afx_msg void OnBnClickedMapLink();
	// 에디트 중인 해당 맵 저장
	afx_msg void OnBnClickedSaveData();
	// 저장된 맵 로드
	afx_msg void OnBnClickedLoadData();
	// 지형 자동 설정
	afx_msg void OnBnClickedSetGround();
	// 선택한 맵 삭제
	afx_msg void OnBnClickedDelete();
	// 그리드 뷰 스위치
	afx_msg void OnBnClickedGreedVisible();
	// 타일 뷰 스위치
	afx_msg void OnBnClickedTileVisible();
	// 오브젝트 뷰 스위치
	afx_msg void OnBnClickedObjectVisible();
	// 경로 다이얼로그
	afx_msg void OnBnClickedPathFind();
	// 타일 옵션 스위치
	afx_msg void OnBnClickedSetOption();

public:
	// ================= Controll
	CListBox m_MapListBox;

	// ================= Value
	// 생성할 타일 X
	int m_iTileX;
	// 생성할 타일 Y
	int m_iTileY;
	// 생성된 맵 너비
	int m_iVisibleTileW;
	// 생성된 맵 높이
	int m_iVisibleMapH;
	// 생성된 맵 타일 X 갯수
	int m_iMapTileX;
	// 생성된 맵 타일 Y 갯수
	int m_iMapTileY;

	// ================= User
	CButton m_CheckBox[3];
	
	vector<CTerrain*> m_vecCreateMap;

	// ================= Dialog
	CTerrainDig m_pTerrainDig;
	CMonsterDig m_pMonsterDig;
	CMapLinkDig m_pMapLinkDig;
	CPathFindDig m_pPathFindDig;
	afx_msg void OnBnClickedSetCollision();
};


