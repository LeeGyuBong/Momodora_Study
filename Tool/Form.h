#pragma once
#include "afxwin.h"

#include "TerrainDig.h"
#include "MonsterDig.h"
#include "MapLinkDig.h"
#include "PathFindDig.h"

// CForm �� ���Դϴ�.

class CTerrain;
class CForm : public CFormView
{
	DECLARE_DYNCREATE(CForm)

protected:
	CForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	// ���� ����
	afx_msg void OnBnClickedCreateWorld();
	// �� ������ ����Ʈ
	afx_msg void OnLbnSelchangeMapListBox();
	// ���� ������ ����
	afx_msg void OnBnClickedOpenTerrainEdit();
	// ���� ������ ����
	afx_msg void OnBnClickedOpenMonsterEdit();
	// �� ��ũ ������ ����
	afx_msg void OnBnClickedMapLink();
	// ����Ʈ ���� �ش� �� ����
	afx_msg void OnBnClickedSaveData();
	// ����� �� �ε�
	afx_msg void OnBnClickedLoadData();
	// ���� �ڵ� ����
	afx_msg void OnBnClickedSetGround();
	// ������ �� ����
	afx_msg void OnBnClickedDelete();
	// �׸��� �� ����ġ
	afx_msg void OnBnClickedGreedVisible();
	// Ÿ�� �� ����ġ
	afx_msg void OnBnClickedTileVisible();
	// ������Ʈ �� ����ġ
	afx_msg void OnBnClickedObjectVisible();
	// ��� ���̾�α�
	afx_msg void OnBnClickedPathFind();
	// Ÿ�� �ɼ� ����ġ
	afx_msg void OnBnClickedSetOption();

public:
	// ================= Controll
	CListBox m_MapListBox;

	// ================= Value
	// ������ Ÿ�� X
	int m_iTileX;
	// ������ Ÿ�� Y
	int m_iTileY;
	// ������ �� �ʺ�
	int m_iVisibleTileW;
	// ������ �� ����
	int m_iVisibleMapH;
	// ������ �� Ÿ�� X ����
	int m_iMapTileX;
	// ������ �� Ÿ�� Y ����
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


