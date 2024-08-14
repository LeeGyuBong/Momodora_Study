#pragma once
#include "Texture.h"
class CSingleTexture :
	public CTexture
{
public:
	CSingleTexture();
	virtual ~CSingleTexture();

	// CTexture��(��) ���� ��ӵ�
	virtual const TEXINFO * GetTexture(const TCHAR * pStateKey = L"", const int & iIndex = 0) { return m_pTexInfo; };
	virtual HRESULT InsertTexture(const TCHAR * pFilePath, const TCHAR * pStateKey = L"", const int & iCount = 0) override;
	virtual void Release() override;

private:
	TEXINFO* m_pTexInfo;
};

