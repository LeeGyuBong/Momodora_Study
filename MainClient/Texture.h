#pragma once
class CTexture
{
public:
	CTexture();
	virtual ~CTexture();

public:
	virtual const TEXINFO* GetTexture(const TCHAR* pStateKey = L"", const int& iIndex = 0) PURE;
	virtual HRESULT InsertTexture(const TCHAR* pFilePath, const TCHAR* pStateKey = L"", const int& iCount = 0) PURE;
	virtual void Release() PURE;
};