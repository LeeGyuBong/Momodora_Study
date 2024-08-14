#include "stdafx.h"
#include "MainMenu.h"

int CMainMenu::m_iMenuIndex = 1;

CMainMenu::CMainMenu()
	: m_iTextAlpha(0)
{
}


CMainMenu::~CMainMenu()
{
	Release();
}

void CMainMenu::Initialize()
{
	m_tInfo.vPos = { CLIENTWINCX * 0.5f, CLIENTWINCY * 0.5f, 0.f };

	m_wstrObjKey = L"UI";		
	m_wstrStateKey = L"Menu";

	m_vMenuText.push_back(L"장비");
	m_vMenuText.push_back(L"주요 아이템");
	m_vMenuText.push_back(L"지도");
	m_vMenuText.push_back(L"설정");
	m_vMenuText.push_back(L"시작 화면으로 돌아가기");
}

void CMainMenu::LateInit()
{
}

int CMainMenu::Update()
{
	CObj::LateInit();

	if (m_iTextAlpha != 255)
		++m_iTextAlpha;

	m_tInfo.matWorld = CMath::CalculateMatrix(m_tInfo.vPos);

	return NO_EVENT;
}

void CMainMenu::LateUpdate()
{
	Control();
}

void CMainMenu::Render()
{
	RenderBack();
	RenderMenu();
	RenderText();
}

void CMainMenu::RenderBack()
{
	const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"BackGround", L"Effect", 0);
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(130, 255, 255, 255));
}

void CMainMenu::RenderMenu()
{
	const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), 0);
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(200, 255, 255, 255));

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	pTexInfo = CTextureMgr::GetInstance()->GetTexture(m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), 1);
	NULL_CHECK(pTexInfo);
	fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	fCenterY = pTexInfo->tImgInfo.Height * 0.5f;
	for (unsigned int i = 0; i < CLIENTWINCX / pTexInfo->tImgInfo.Width + 1; ++i)
	{
		D3DXMATRIX matWorld;
		D3DXVECTOR3 vPos = { m_tInfo.vPos.x + (i * pTexInfo->tImgInfo.Width) - (CLIENTWINCX * 0.5f) + fCenterX, m_tInfo.vPos.y - 27.f, 0.f };
		matWorld = CMath::CalculateMatrix(vPos);

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
			nullptr, D3DCOLOR_ARGB(200, 255, 255, 255));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 메뉴 아이콘
	for (int i = 0; i < 7; ++i)
	{
		pTexInfo = CTextureMgr::GetInstance()->GetTexture(m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), i + 2);

		NULL_CHECK(pTexInfo);

		fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		D3DXMATRIX matWorld;
		D3DXVECTOR3 vPos = { m_tInfo.vPos.x + (pTexInfo->tImgInfo.Width * i) - 70.f, m_tInfo.vPos.y - 27.f, 0.f };
		matWorld = CMath::CalculateMatrix(vPos);

		int R, G, B;
		if (i == m_iMenuIndex)
		{
			R = 255;
			G = 255;
			B = 255;
		}
		else
		{
			R = 150;
			G = 150;
			B = 150;
		}

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
			nullptr, D3DCOLOR_ARGB(255, R, G, B));
	}
}

void CMainMenu::RenderText()
{
	D3DXMATRIX matWorld;
	matWorld = CMath::CalculateMatrix(D3DXVECTOR3(40.f, m_tInfo.vPos.y - 35.f, 0.f));

	CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
	CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite(),
		m_vMenuText[m_iMenuIndex - 1].c_str(), lstrlen(m_vMenuText[m_iMenuIndex - 1].c_str()), nullptr, 0, D3DCOLOR_ARGB(m_iTextAlpha, 255, 255, 255));

	wstring strText[2];
	strText[0] = L"A: ";
	strText[1] = L"S: ";
	for (int i = 0; i < 2; ++i)
	{
		matWorld = CMath::CalculateMatrix(D3DXVECTOR3(40.f + 120.f * i, m_tInfo.vPos.y + 25.f, 0.f));
		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
		CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite(),
			strText[i].c_str(), lstrlen(strText[i].c_str()), nullptr, 0, D3DCOLOR_ARGB(255, 140, 140, 140));
	}

	strText[0] = L"확인";
	strText[1] = L"취소";
	for (int i = 0; i < 2; ++i)
	{
		matWorld = CMath::CalculateMatrix(D3DXVECTOR3(80.f + 120.f * i, m_tInfo.vPos.y + 25.f, 0.f));
		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
		CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite(),
			strText[i].c_str(), lstrlen(strText[i].c_str()), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CMainMenu::Release()
{
	m_vMenuText.clear();
	m_vMenuText.shrink_to_fit();
}

void CMainMenu::Control()
{
	if (CKeyMgr::GetInstance()->KeyDown(KEY_A))
	{
		CSoundMgr::GetInstance()->PlaySound(L"sndUIchoice1.wav", CSoundMgr::EFFECT);
		switch (m_iMenuIndex - 1)
		{
			// 장비
		case 0:
			CUiMgr::GetInstance()->UiChanger(CUiMgr::EQUIP);
			return;
			// 주요 아이템
		/*case 1:
			CUiMgr::GetInstance()->UiChanger(CUiMgr::IMPORTANT);
			return;*/
			 //시작 화면으로 돌아가기
		case 4:
			g_bIsStart = false;

			CSoundMgr::GetInstance()->StopSound(CSoundMgr::BGM);
			CSceneMgr::GetInstance()->SceneChange(CSceneMgr::LOGO);
			CUiMgr::GetInstance()->UI_OFF();
		
			m_iMenuIndex = 1;
			return;
		}
	}

	if (CKeyMgr::GetInstance()->KeyDown(KEY_SHIFT))
	{
		CUiMgr::GetInstance()->UI_OFF();
		CSoundMgr::GetInstance()->PlaySound(L"sndUIclose.wav", CSoundMgr::EFFECT);

		m_iMenuIndex = 1;
	}
	if (CKeyMgr::GetInstance()->KeyDown(KEY_S))
	{
		CUiMgr::GetInstance()->UI_OFF();
		CSoundMgr::GetInstance()->PlaySound(L"sndUIclose.wav", CSoundMgr::EFFECT);

		m_iMenuIndex = 1;
	}
	if (CKeyMgr::GetInstance()->KeyDown(KEY_LEFT))
	{
		if (m_iMenuIndex == 1)
			m_iMenuIndex = 5;
		else
			--m_iMenuIndex;

		m_iTextAlpha = 0;
	}
	if (CKeyMgr::GetInstance()->KeyDown(KEY_RIGHT))
	{
		if (m_iMenuIndex == 5)
			m_iMenuIndex = 1;
		else
			++m_iMenuIndex;

		m_iTextAlpha = 0;
	}
}