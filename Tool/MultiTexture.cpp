#include "stdafx.h"
#include "MultiTexture.h"


CMultiTexture::CMultiTexture()
{
}


CMultiTexture::~CMultiTexture()
{
	Release();
}

const TEXINFO * CMultiTexture::GetTexture(const TCHAR * pStateKey, const int & iIndex)
{
	auto iter_find = find_if(m_MultiTex.begin(), m_MultiTex.end(), 
		[&pStateKey](auto& MyPair)
	{
		return pStateKey == MyPair.first;
	});

	if (iter_find == m_MultiTex.end())
		return nullptr;

	if (0 > iIndex || iter_find->second.size() <= (size_t)iIndex)
		return nullptr;

	return iter_find->second[iIndex];
}

HRESULT CMultiTexture::InsertTexture(const TCHAR * pFilePath, const TCHAR * pStateKey, const int & iCount)
{
	TCHAR szFullPath[MAX_STR] = L"";

	for (int i = 0; i < iCount; ++i)
	{
		swprintf_s(szFullPath, pFilePath, i);

		TEXINFO* pTexInfo = new TEXINFO;
		ZeroMemory(pTexInfo, sizeof(TEXINFO));

		// D3DXGetImageInfoFromFile: ������ �̹��� ���Ͽ� ���� ������ D3DXIMAGE_INFO���� ������ִ� �Լ�.
		if (FAILED(D3DXGetImageInfoFromFile(szFullPath, &pTexInfo->tImgInfo)))
		{
			ERR_MSG(L"Image Load Failed!!!");
			SafeDelete(pTexInfo);
			return E_FAIL;
		}

		// D3DXIMAGE_INFO������ �������� LPDIRECT3DTEXTURE9��ü�� ����.
		if (FAILED(D3DXCreateTextureFromFileEx(CDevice::GetInstance()->GetDevice(), /*Texture Com��ü ���� ��ü*/
			szFullPath, /*�̹��� ���*/
			pTexInfo->tImgInfo.Width, /*�̹����� ���� ũ��*/
			pTexInfo->tImgInfo.Height, /*�̹����� ���� ũ��*/
			pTexInfo->tImgInfo.MipLevels, /*mipmap�� ����ϱ� ���� ����ġ*/
			0, /*������ �ؽ��ĸ� ����ϴ� ���*/
			pTexInfo->tImgInfo.Format, /*�ؽ��Ŀ� ���� �ȼ� ����*/
			D3DPOOL_MANAGED, /*Direct3D �ڿ��鿡 ���� � �޸𸮿� ������ ���ΰ��� ���� ����*/
			D3DX_DEFAULT, D3DX_DEFAULT, 0, &pTexInfo->tImgInfo, nullptr, &pTexInfo->pTexture)))
		{
			ERR_MSG(L"Multi Texture Load Failed!!!");
			SafeDelete(pTexInfo);
			return E_FAIL;
		}

		m_MultiTex[pStateKey].push_back(pTexInfo);
	}

	return S_OK;
}

void CMultiTexture::Release()
{
	for (auto& MyPair : m_MultiTex)
	{
		for_each(MyPair.second.begin(), MyPair.second.end(),
			[](TEXINFO*& pTexInfo)
		{
			pTexInfo->pTexture->Release();
			SafeDelete(pTexInfo);
		});
		MyPair.second.clear();
		MyPair.second.shrink_to_fit();
	}
	m_MultiTex.clear();
}
