
// ToolView.h : CToolView 클래스의 인터페이스
//

#pragma once

class CTerrain;
class CToolDoc;
class CToolView : public CScrollView
{
protected: // serialization에서만 만들어집니다.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

public:
	// 0 : 맵타일, 1 : 비 상호작용 오브젝트, 2 : 상호작용 오브젝트
	enum SHEET { MAPTILE, UNOBJ, OBJ, END };

// 특성입니다.
public:
	CToolDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnInitialUpdate();

// 구현입니다.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
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

#ifndef _DEBUG  // ToolView.cpp의 디버그 버전
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

