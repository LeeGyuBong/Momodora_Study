#pragma once

class CTerrain;
class CTerrainMgr
{
	DECLARE_SINGLETON(CTerrainMgr)

private:
	CTerrainMgr();
	~CTerrainMgr();

public:
	void Update();
	void LateUpdate();
	void Release();

private:
	void LoadMapData();

public:
	vector<CTerrain*>& GetMapList() { return m_MapList; }
	CTerrain*& GetCurTerrain() { return m_pTerrain; }

	vector<GREED*>& GetGreed() { return m_vecGreed; }
	const int& GetGreedX() { return m_iGreedX; }
public:
	void ChangeTerrain(CTerrain*& pTerrain) { m_pTerrain = pTerrain; }

	void GreedInit();
	void RenderGreed();
	void GreedDataReset();

private:
	CTerrain* m_pTerrain;
	vector<CTerrain*> m_MapList;

	// �浹 �׸���
	vector<GREED*> m_vecGreed;
	int m_iGreedX;
};