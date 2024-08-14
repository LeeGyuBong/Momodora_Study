#include "stdafx.h"
#include "SceneMgr.h"

#include "Terrain.h"

#include "Logo.h"
#include "Stage.h"

IMPLEMENT_SINGLETON(CSceneMgr)

CSceneMgr::CSceneMgr()
	: m_pScene(nullptr), m_eCurScene(END), m_ePreScene(END), m_bIsBoss(false), m_bIsBossFight(false)
{
}


CSceneMgr::~CSceneMgr()
{
	Release();
}

void CSceneMgr::SceneChange(SCENE eScene)
{
	m_eCurScene = eScene;

	if (m_ePreScene != m_eCurScene)
	{
		SafeDelete(m_pScene);

		switch (eScene)
		{
		case LOGO:
			m_pScene = new CLogo;
			break;
		case STAGE:
			m_pScene = new CStage;
			break;
		}
		m_pScene->Initialize();

		m_ePreScene = m_eCurScene;
	}
}

void CSceneMgr::Update()
{
	if(!CUiMgr::GetInstance()->GetUIState())
		m_pScene->Update();

	CUiMgr::GetInstance()->Update();
}

void CSceneMgr::LateUpdate()
{
	if (!CUiMgr::GetInstance()->GetUIState())
		m_pScene->LateUpdate();

	CUiMgr::GetInstance()->LateUpdate();
}

void CSceneMgr::Render()
{
	m_pScene->Render();

	CUiMgr::GetInstance()->Render();
}

void CSceneMgr::Release()
{
	SafeDelete(m_pScene);
}
