#include "stdafx.h"
#include "SingleTexture.h"


CSingleTexture::CSingleTexture()
	: m_pTexInfo(nullptr)
{
}


CSingleTexture::~CSingleTexture()
{
	Release();
}

HRESULT CSingleTexture::InsertTexture(const TCHAR * pFilePath, const TCHAR * pStateKey, const int & iCount)
{
	m_pTexInfo = new TEXINFO;
	ZeroMemory(m_pTexInfo, sizeof(TEXINFO));

	// D3DXGetImageInfoFromFile: ������ �̹��� ���Ͽ� ���� ������ D3DXIMAGE_INFO���� ������ִ� �Լ�.
	if (FAILED(D3DXGetImageInfoFromFile(pFilePath, &m_pTexInfo->tImgInfo)))
	{
		ERR_MSG(L"Image Load Failed!!!");
		SafeDelete(m_pTexInfo);
		return E_FAIL;
	}

	// D3DXIMAGE_INFO������ �������� LPDIRECT3DTEXTURE9��ü�� ����.
	if (FAILED(D3DXCreateTextureFromFileEx(CDevice::GetInstance()->GetDevice(), /*Texture Com��ü ���� ��ü*/
		pFilePath, /*�̹��� ���*/
		m_pTexInfo->tImgInfo.Width, /*�̹����� ���� ũ��*/
		m_pTexInfo->tImgInfo.Height, /*�̹����� ���� ũ��*/
		m_pTexInfo->tImgInfo.MipLevels, /*mipmap�� ����ϱ� ���� ����ġ*/
		0, /*������ �ؽ��ĸ� ����ϴ� ���*/
		m_pTexInfo->tImgInfo.Format, /*�ؽ��Ŀ� ���� �ȼ� ����*/
		D3DPOOL_MANAGED, /*Direct3D �ڿ��鿡 ���� � �޸𸮿� ������ ���ΰ��� ���� ����*/
		D3DX_DEFAULT, D3DX_DEFAULT, 0, &m_pTexInfo->tImgInfo, nullptr, &m_pTexInfo->pTexture)))
	{
		ERR_MSG(L"Signle Texture Load Failed!!!");
		SafeDelete(m_pTexInfo);
		return E_FAIL;
	}

	return S_OK;
}

void CSingleTexture::Release()
{
	m_pTexInfo->pTexture->Release();
	SafeDelete(m_pTexInfo);
}
