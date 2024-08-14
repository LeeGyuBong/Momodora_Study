#include "stdafx.h"
#include "ScrollMgr.h"
#include "Terrain.h"

D3DXVECTOR3 CScrollMgr::Scroll = { 0.f, 0.f, 0.f };
float CScrollMgr::m_fShakeTime = 0.f;
bool CScrollMgr::m_bIsShake = false;
bool CScrollMgr::m_bIsShakeSmall = false;
float CScrollMgr::m_fShakeTurn = 0.f;

bool CScrollMgr::m_bIsChangeInit = false;

CScrollMgr::CScrollMgr()
{
}


CScrollMgr::~CScrollMgr()
{
}

void CScrollMgr::ScrollLock()
{
	if (0.f < Scroll.x)
		Scroll.x = 0.f;
	if (0.f < Scroll.y)
		Scroll.y = 0.f;

	if (Scroll.x < CLIENTWINCX - CTerrainMgr::GetInstance()->GetCurTerrain()->GetTileX() * TILESIZE)
		Scroll.x = static_cast<float>(CLIENTWINCX - CTerrainMgr::GetInstance()->GetCurTerrain()->GetTileX() * TILESIZE);
	if (Scroll.y < CLIENTWINCY - CTerrainMgr::GetInstance()->GetCurTerrain()->GetTileY() * TILESIZE)
		Scroll.y = static_cast<float>(CLIENTWINCY - CTerrainMgr::GetInstance()->GetCurTerrain()->GetTileY() * TILESIZE);
}

void CScrollMgr::ScrollInit()
{
	// 카메라 중점에 대한 오프셋
	if (CObjMgr::GetInstance()->GetPlayer() != nullptr && !m_bIsChangeInit)
	{
		Scroll.x = (float)CLIENTWINCX * 0.5f - CObjMgr::GetInstance()->GetPlayer()->GetInfo().vPos.x;
		Scroll.y = (float)CLIENTWINCX * 0.5f - CObjMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y;
	}
}

void CScrollMgr::SetScroll(D3DXVECTOR3 vScroll)
{
	Scroll.x += vScroll.x;
	Scroll.y += vScroll.y;
}

void CScrollMgr::ScrollShake()
{
	if (m_bIsShake)
	{
		m_fShakeTime += CTimeMgr::GetInstance()->GetTime();
		m_fShakeTurn += CTimeMgr::GetInstance()->GetTime();

		if (m_fShakeTurn > 0.1f)
		{
			int randomX = rand() % 5 + 1;
			int randomY = rand() % 5 + 1;

			int randomDirX = rand() % 2 + 1;
			int randomDirY = rand() % 2 + 1;

			if (randomDirX % 2 == 0)
				randomX *= -1;
			if (randomDirY % 2 == 0)
				randomY *= -1;

			Scroll.x += randomX;
			Scroll.y += randomY;
		}

		if (m_fShakeTime > 0.5f)
		{
			m_bIsShake = false;
			m_fShakeTurn = 0.f;
			m_fShakeTime = 0.f;
		}
	}

	if (m_bIsShakeSmall)
	{
		m_fShakeTime += CTimeMgr::GetInstance()->GetTime();
		m_fShakeTurn += CTimeMgr::GetInstance()->GetTime();

		if (m_fShakeTurn > 0.1f)
		{
			int randomX = rand() % 3 + 1;
			int randomY = rand() % 3 + 1;

			int randomDirX = rand() % 2 + 1;
			int randomDirY = rand() % 2 + 1;

			if (randomDirX % 2 == 0)
				randomX *= -1;
			if (randomDirY % 2 == 0)
				randomY *= -1;

			Scroll.x += randomX;
			Scroll.y += randomY;
		}

		if (m_fShakeTime > 0.5f)
		{
			m_bIsShakeSmall = false;
			m_fShakeTurn = 0.f;
			m_fShakeTime = 0.f;
		}
	}
}

void CScrollMgr::ChangeScrollInit(D3DXVECTOR3 vPos)
{
	if (m_bIsChangeInit)
	{
		Scroll.x = (float)CLIENTWINCX * 0.5f - vPos.x;
		Scroll.y = (float)CLIENTWINCX * 0.5f - vPos.y;
	}
}

void CScrollMgr::Reset()
{
	Scroll = { 0.f, 0.f, 0.f };

	m_fShakeTurn = false;
	m_fShakeTime = 0.f;
	m_bIsShake = false;
	m_bIsShakeSmall = false;

	m_bIsChangeInit = false;
}