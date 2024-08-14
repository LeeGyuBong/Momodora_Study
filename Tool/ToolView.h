
// ToolView.h : CToolView Ŭ������ �������̽�
//

#pragma once

class CTerrain;
class CToolDoc;
class CToolView : public CScrollView
{
protected: // serialization������ ��������ϴ�.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

public:
	// 0 : ��Ÿ��, 1 : �� ��ȣ�ۿ� ������Ʈ, 2 : ��ȣ�ۿ� ������Ʈ
	enum SHEET { MAPTILE, UNOBJ, OBJ, END };

// Ư���Դϴ�.
public:
	CToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnInitialUpdate();

// �����Դϴ�.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint point);
	afx_msg void OnBlock();
	afx_msg void OnLadder();
	afx_msg void OnTrap();
	afx_msg void OnSwitch();
	afx_msg void OnOptionDelete();
	
	void AllocateMapTile(int index);
	void AllocateUnObj(int index);
	void AllocateObj(int index);

	void InitLinkViewPos();

	void DrawLine();

	void DrawRectangle();

public:
	CTerrain* m_pTerrain;

	CPoint MousePoint;

	POINT StartPoint;
	POINT EndPoint;
	bool m_bIsRectVisible;

	bool m_bIsTerrainDigOn;
	bool m_bIsMonsterDigOn;

	TCHAR* pObjKey;
	TCHAR* pStateKey;
	int m_iVDrawID;
	float m_fReversalX;
	float m_fReversalY;

	SHEET m_eCurSheet;

	bool m_bIsMapLink;

	bool m_bIsSetOption;
	list<TILE*> m_SelectLst;
	
	bool m_bIsCollision;
	
};

#ifndef _DEBUG  // ToolView.cpp�� ����� ����
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

