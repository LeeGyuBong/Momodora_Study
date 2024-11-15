#include "stdafx.h"
#include "TextureMgr.h"
#include "SingleTexture.h"
#include "MultiTexture.h"

IMPLEMENT_SINGLETON(CTextureMgr)

CTextureMgr::CTextureMgr()
{
}


CTextureMgr::~CTextureMgr()
{
	Release();
}

const TEXINFO * CTextureMgr::GetTexture(const TCHAR * pObjKey, const TCHAR * pStateKey, const int & iIndex)
{
	auto iter_find = find_if(m_MapTexture.begin(), m_MapTexture.end(), 
		[&pObjKey](auto& MyPair)
	{
		return pObjKey == MyPair.first;
	});

	if (iter_find == m_MapTexture.end())
		return nullptr;

	return iter_find->second->GetTexture(pStateKey, iIndex);
}

HRESULT CTextureMgr::InsertTexture(const TCHAR * pFilePath, const TCHAR * pObjKey, TEXTYPE eType, const TCHAR * pStateKey, const int & iCount)
{
	auto iter_find = find_if(m_MapTexture.begin(), m_MapTexture.end(), 
		[&pObjKey](auto& MyPair)
	{
		return MyPair.first == pObjKey;
	});

	// pObjKey값이 없을시 
	if (iter_find == m_MapTexture.end())
	{
		CTexture* pTexture = nullptr;

		switch (eType)
		{
		case TEX_SINGLE:
			pTexture = new CSingleTexture;
			break;
		case TEX_MULTI:
			pTexture = new CMultiTexture;
			break;
		}

		if (FAILED(pTexture->InsertTexture(pFilePath, pStateKey, iCount)))
		{
			ERR_MSG(pFilePath);
			SafeDelete(pTexture);
			return E_FAIL;
		}

		m_MapTexture.insert({ pObjKey, pTexture });
	}
	else
	{
		switch (eType)
		{
		case TEX_SINGLE:
			if (iter_find->second == nullptr)
			{
				if (FAILED(iter_find->second->InsertTexture(pFilePath)))
				{
					ERR_MSG(pFilePath);
					return E_FAIL;
				}
			}
			break;
		case TEX_MULTI:
			if (iter_find->second->GetTexture(pStateKey) == nullptr)
			{
				if (FAILED(iter_find->second->InsertTexture(pFilePath, pStateKey, iCount)))
				{
					ERR_MSG(pFilePath);
					return E_FAIL;
				}
			}
			break;
		}
	}

	return S_OK;
}

void CTextureMgr::Release()
{
	for_each(m_MapTexture.begin(), m_MapTexture.end(), 
		[](auto& MyPair)
	{
		SafeDelete(MyPair.second);
	});

	m_MapTexture.clear();
}

HRESULT CTextureMgr::ReadImagePath(const wstring & wstrPath)
{
	wifstream fin;

	fin.open(wstrPath);

	if (fin.fail())
	{
		ERR_MSG(L"ImgPath Load Failed!!!");
		return E_FAIL;
	}

	TCHAR szObjKey[MAX_STR] = L"";
	TCHAR szStateKey[MAX_STR] = L"";
	TCHAR szCount[MIN_STR] = L"";
	TCHAR szPath[MAX_STR] = L"";

	while (true)
	{
		fin.getline(szObjKey, MAX_STR, '|');
		fin.getline(szStateKey, MAX_STR, '|');
		fin.getline(szCount, MAX_STR, '|');
		fin.getline(szPath, MAX_STR);

		if (fin.eof())
			break;

		int Count = _ttoi(szCount);

		if (FAILED(InsertTexture(szPath, szObjKey, TEX_MULTI, szStateKey, Count)))
		{
			fin.close();

			ERR_MSG(L"Multi Texture Load Failed");
			return E_FAIL;
		}
	}

	fin.close();

	return S_OK;
}