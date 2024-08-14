#include "stdafx.h"
#include "Logo.h"


CLogo::CLogo()
	: time(0), frame(0), m_fAlphaTime(0.f), m_iAlpha(0)
{
}


CLogo::~CLogo()
{
	Release();
}

void CLogo::Initialize()
{
	CTextureMgr::GetInstance()->InsertTexture(L"../Data/Texture/Logo/sTitleScreenBG_0.png", L"Logo_Back", TEX_SINGLE);
	CTextureMgr::GetInstance()->InsertTexture(L"../Data/Texture/Logo/sTitleScreen_2.png", L"Logo_Text", TEX_SINGLE);
	CTextureMgr::GetInstance()->InsertTexture(L"../Data/Texture/Logo/sTitleScreen2_0.png", L"Logo_Cat", TEX_SINGLE);
	CTextureMgr::GetInstance()->InsertTexture(L"../Data/Texture/Logo/sTitleKaho_%d.png", L"Logo_Kaho", TEX_MULTI, L"IDLE", 5);

	CSoundMgr::GetInstance()->PlayBGM(L"Title.mp3");
}

void CLogo::Update()
{
	if ((time += CTimeMgr::GetInstance()->GetTime()) > 0.2f)
	{
		++frame;
		time = 0.f;
		if (frame >= CTextureMgr::GetInstance()->GetTextureSize(L"Logo_Kaho", L"IDLE"))
			frame = 0;
	}

	if (m_iAlpha < 190)
	{
		m_fAlphaTime += CTimeMgr::GetInstance()->GetTime();

		if (m_fAlphaTime > 0.01f)
		{
			++m_iAlpha;
			m_fAlphaTime = 0;
		}
	}
}

void CLogo::LateUpdate()
{
}

void CLogo::Render()
{
	D3DXMATRIX matWorld;
	ZeroMemory(matWorld, sizeof(D3DXMATRIX));
	matWorld = CMath::CalculateMatrix(D3DXVECTOR3(400.f, 300.f, 0.f));

	CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

	const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
		L"Logo_Back");

	if (pTexInfo == nullptr)
		return;

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));
	// --------------------------------------------------------
	ZeroMemory(matWorld, sizeof(D3DXMATRIX));
	matWorld = CMath::CalculateMatrix(D3DXVECTOR3(300.f, 170.f, 0.f));

	CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

	pTexInfo = CTextureMgr::GetInstance()->GetTexture(
		L"Logo_Cat");

	if (pTexInfo == nullptr)
		return;

	fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));
	// --------------------------------------------------------
	ZeroMemory(matWorld, sizeof(D3DXMATRIX));
	matWorld = CMath::CalculateMatrix(D3DXVECTOR3(400.f, 250.f, 0.f));

	CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

	pTexInfo = CTextureMgr::GetInstance()->GetTexture(
		L"Logo_Text");

	if (pTexInfo == nullptr)
		return;

	fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));
	//-----------------------------------------------------------
	ZeroMemory(matWorld, sizeof(D3DXMATRIX));
	matWorld = CMath::CalculateMatrix(D3DXVECTOR3(634.f, 538.f, 0.f));

	CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

	pTexInfo = CTextureMgr::GetInstance()->GetTexture(
		L"Logo_Kaho", L"IDLE", frame);

	if (pTexInfo == nullptr)
		return;

	fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	//////////////////////////////////////////////////////////////

	TCHAR szIndex[MIN_STR] = L"아무 버튼이나 누르세요";
	ZeroMemory(matWorld, sizeof(D3DXMATRIX));
	matWorld = CMath::CalculateMatrix(D3DXVECTOR3(230.f, 400.f, 0.f));
	
	CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
	CDevice::GetInstance()->GetBigFont()->DrawTextW(CDevice::GetInstance()->GetSprite(),
	szIndex, lstrlen(szIndex), nullptr, 0, D3DCOLOR_ARGB(m_iAlpha, 255, 200, 0));

	//////////////////////////////////////////////////////////////

	if (g_bIsStart)
	{
		CSceneMgr::GetInstance()->SceneChange(CSceneMgr::STAGE);
		CSoundMgr::GetInstance()->PlaySound(L"sndPressStart.wav", CSoundMgr::EFFECT);
		return;
	}
}

void CLogo::Release()
{
}
