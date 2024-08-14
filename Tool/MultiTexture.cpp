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

		// D3DXGetImageInfoFromFile: 지정한 이미지 파일에 관한 정보를 D3DXIMAGE_INFO에게 기록해주는 함수.
		if (FAILED(D3DXGetImageInfoFromFile(szFullPath, &pTexInfo->tImgInfo)))
		{
			ERR_MSG(L"Image Load Failed!!!");
			SafeDelete(pTexInfo);
			return E_FAIL;
		}

		// D3DXIMAGE_INFO정보를 바탕으로 LPDIRECT3DTEXTURE9객체를 생성.
		if (FAILED(D3DXCreateTextureFromFileEx(CDevice::GetInstance()->GetDevice(), /*Texture Com객체 생성 주체*/
			szFullPath, /*이미지 경로*/
			pTexInfo->tImgInfo.Width, /*이미지의 가로 크기*/
			pTexInfo->tImgInfo.Height, /*이미지의 세로 크기*/
			pTexInfo->tImgInfo.MipLevels, /*mipmap을 사용하기 위한 가중치*/
			0, /*생성한 텍스쳐를 사용하는 방식*/
			pTexInfo->tImgInfo.Format, /*텍스쳐에 대한 픽셀 포맷*/
			D3DPOOL_MANAGED, /*Direct3D 자원들에 대해 어떤 메모리에 보관할 것인가에 대한 지정*/
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
