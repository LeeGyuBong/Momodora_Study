#pragma once

class CTexture;
class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr)

private:
	CTextureMgr();
	~CTextureMgr();

public:
	const TEXINFO* GetTexture(const TCHAR* pObjKey, const TCHAR* pStateKey = L"", const int& iIndex = 0);

public:
	HRESULT ReadImagePath(const wstring& wstrPath);
	HRESULT InsertTexture(const TCHAR* pFilePath, const TCHAR* pObjKey, TEXTYPE eType, 
		const TCHAR* pStateKey = L"", const int& iCount = 0);
	void Release();

public:
	// 애니메이션 전체 프레임 반환 (0 ~ Size - 1)
	float GetTextureSize(const TCHAR* pObjKey, const TCHAR* pStateKey);

	void InitTexInfo();

	const vector<TEXINFO*>& GetTileInfo() { return m_TileInfo; }
	const vector<TEXINFO*>& GetObjInfo() { return m_ObjInfo; }
	const vector<TEXINFO*>& GetInteractInfo() { return m_InteractInfo; }
	const vector<TEXINFO*>& GetBackGroundInfo() { return m_BackGroundInfo; }

private:
	map<wstring, CTexture*> m_MapTexture;

	vector<TEXINFO*> m_TileInfo;
	vector<TEXINFO*> m_ObjInfo;
	vector<TEXINFO*> m_InteractInfo;
	vector<TEXINFO*> m_BackGroundInfo;
};

