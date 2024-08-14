#include "stdafx.h"
#include "GameManager.h"

CGameManager::CGameManager()
{
	srand(unsigned(time(nullptr)));
}


CGameManager::~CGameManager()
{
	Release();
}

void CGameManager::Initialize()
{
	CTimeMgr::GetInstance()->InitTime();
	CSoundMgr::GetInstance()->Initialize();

	ShowCursor(false);

	if (FAILED(CDevice::GetInstance()->InitDevice()))
	{
		ERR_MSG(L"Device Init Failed!!!");
		return;
	}

	if (FAILED(CTextMgr::GetInstance()->LoadText(L"../Data/MapData/TextScripts.txt")))
	{
		ERR_MSG(L"Text Path Load Failed!!!");
		return;
	}
	if(FAILED(CDataMgr::GetInstance()->InsertItemData(L"../Data/MapData/ItemScripts.txt")))
	{
		ERR_MSG(L"ItemData Path Load Failed!!!");
		return;
	}
	// 이미지 경로를 받고 이미지 로드
	if (FAILED(CTextureMgr::GetInstance()->ReadImagePath(L"../Data/MapData/ImagePath.txt")))
	{
		ERR_MSG(L"Image Path Load Failed!!!");
		return;
	}
	CTextureMgr::GetInstance()->InsertTexture(L"../Data/Texture/ViewRect.png", L"Circle", TEX_SINGLE);

	CTextureMgr::GetInstance()->InitTexInfo();

	CSceneMgr::GetInstance()->SceneChange(CSceneMgr::LOGO);
}

void CGameManager::Update()
{
	CTimeMgr::GetInstance()->UpdateTime();
	CKeyMgr::GetInstance()->KeyCheck();

	CSceneMgr::GetInstance()->Update();
}

void CGameManager::LateUpdate()
{
	CSceneMgr::GetInstance()->LateUpdate();
	CSoundMgr::GetInstance()->UpdateSound();
}

void CGameManager::Render()
{
	CDevice::GetInstance()->Render_Begin();
	// 아래부터 코드 작성
	


	CSceneMgr::GetInstance()->Render();



	// 위로부터 코드 작성
	CDevice::GetInstance()->Render_End();
}

void CGameManager::Release()
{
	CTextureMgr::GetInstance()->DestroyInstance();
	CDevice::GetInstance()->DestroyInstance();

	CKeyMgr::GetInstance()->DestroyInstance();
	CTimeMgr::GetInstance()->DestroyInstance();
	CObjMgr::GetInstance()->DestroyInstance();
	CSoundMgr::GetInstance()->DestroyInstance();
	CUiMgr::GetInstance()->DestroyInstance();
	CSceneMgr::GetInstance()->DestroyInstance();
	CTerrainMgr::GetInstance()->DestroyInstance();
	CDataSubject::GetInstance()->DestroyInstance();
	CTextMgr::GetInstance()->DestroyInstance();
	CDataMgr::GetInstance()->DestroyInstance();
}