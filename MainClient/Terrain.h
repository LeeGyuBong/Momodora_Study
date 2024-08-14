#pragma once

class CTerrain
{
public:
	CTerrain();
	virtual ~CTerrain();

public:
	void Initialize();
	void LateInit();
	void Update();
	void LateUpdate();
	void Release();

	void RenderTile();
	void RenderBackGround();
	void RenderFrontObj();
	void RenderBackObj();
	void RenderInteractObj();

	void InteractDataSync(INTERACT * pInter);

public:
	void SetTileX(int x) { m_iTileX = x; }
	void SetTileY(int y) { m_iTileY = y; }

public:
	const int& GetTileX() const { return m_iTileX; }
	const int& GetTileY() const { return m_iTileY; }
	vector<TILE*>& GetTile() { return m_vecTile; }
	vector<TILE*>& GetFrontObj() { return m_vecTileObj[0]; }
	vector<TILE*>& GetBackObj() { return m_vecTileObj[1]; }
	vector<TILE*>& GetInteractObj() { return m_vecInteractObj; }
	MAPLINK& GetMapLink() { return m_tLink; }
	vector<MONSTER*>& GetMonsterData() { return m_vecMonster; }

	vector<INTERACT*>& GetInteractItem() { return m_vecInteract; }

private:
	int m_iTileX;
	int m_iTileY;

	vector<TILE*> m_vecTile;
	vector<TILE*> m_vecTileObj[2];
	vector<TILE*> m_vecInteractObj;

	MAPLINK m_tLink;

	vector<MONSTER*> m_vecMonster;

	bool m_bIsInit;

	vector<INTERACT*> m_vecInteract;
};