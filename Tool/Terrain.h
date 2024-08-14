#pragma once

class CToolView;
class CTerrain
{
public:
	CTerrain();
	~CTerrain();

public:
	void Initialize();
	void Render();
	void Release();

public:
	int GetTileIndex(const D3DXVECTOR3 & pos);
	void CheckNeighbor();
	void ChangeTileOption(const D3DXVECTOR3 & pos);
	void CheckRect(const RECT& SelectRC);

	void CheckSetLst(const RECT & SelectRC);

	void InitLinkViewPos();

private:
	bool IsPicking(const D3DXVECTOR3 & pos, const int & index);

public:
	void SetMainView(CToolView* pView) { m_pMainView = pView; }

public:
	// 전체 가로 타일 수
	int m_iTileX;
	// 전체 세로 타일 수
	int m_iTileY;

	// 타일 정보 저장 벡터
	vector<TILE*> m_vecTile;
	vector<TILE*> m_vecTileObj[2];
	vector<TILE*> m_vecInteractObj;
	vector<MONSTER*> m_vecMonster;

	// 맵 연결
	MAPLINK m_tLink;

	// ------- 비 저장 데이터
	CToolView* m_pMainView;

	DWORD m_dwVisible;
	bool m_bIsGreed;
	bool m_bIsTile;
	bool m_bIsObject;

	D3DXVECTOR3 vPos;
	D3DXVECTOR2 vLine[5];
};
