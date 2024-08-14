#include "stdafx.h"
#include "PoisionBomb.h"
#include "PoisionCloud.h"


CPoisionBomb::CPoisionBomb()
	: m_fJumpTime(0.f), m_fPosY(0.f), m_fSpawnTime(0.f), m_fIntervalTime(0.f)
{
}


CPoisionBomb::~CPoisionBomb()
{
	Release();
}

void CPoisionBomb::Initialize()
{
	m_tInfo.vSize = { 22.f, 20.f, 0.f };

	m_wstrObjKey = L"Imp";
	m_wstrStateKey = L"ImpEffect";
	m_tFrame.fFrame = 1.f;

	m_fSpeed = 300.f;

	m_tStatus.Attack = 0;
}

void CPoisionBomb::LateInit()
{
	m_fPosY = m_tInfo.vPos.y;
}

int CPoisionBomb::Update()
{
	CObj::LateInit();

	if (m_bIsDead)
		return DEAD_OBJ;

	if (!m_bIsHit)
	{
		float Height = (powf(m_fJumpTime, 2.f) * -GRAVITY * 0.5f) + (m_fJumpTime * 45.f);

		m_tInfo.vPos.x += m_tInfo.vDir.x * 200.f * CTimeMgr::GetInstance()->GetTime();
		m_tInfo.vPos.y = m_fPosY - Height;
		m_fJumpTime += CTimeMgr::GetInstance()->GetTime() * 10.f;

		m_fDegree -= 2.f;
	}

	m_tInfo.matWorld = CMath::CalculateMatrix(m_tInfo.vPos + CScrollMgr::GetScroll(), D3DXVECTOR3(m_tInfo.vDir.x * -1.f, 1.f, 0.f), ROTATION::Z, m_fDegree);

	return NO_EVENT;
}

void CPoisionBomb::LateUpdate()
{
	if (m_bIsHit)
	{
		m_fSpawnTime += CTimeMgr::GetInstance()->GetTime();
		m_fIntervalTime += CTimeMgr::GetInstance()->GetTime();

		if (m_fSpawnTime > 1.f)
			m_bIsDead = true;

		if (m_fIntervalTime > 0.1f)
		{
			int x = rand() % 15;
			int y = rand() % 15;

			if ((rand() % 4 + 1) % 2 == 0)
				x *= -1;
			else if ((rand() % 4 + 1) % 2 == 1)
				y *= -1;

			CObjMgr::GetInstance()->AddObject(CAbstractFactory<CPoisionCloud>::CreateObject(D3DXVECTOR3(m_tInfo.vPos.x + x, m_tInfo.vPos.y + y, 0.f)), OBJECT::POISIONBOMB);

			CSoundMgr::GetInstance()->PlaySound(L"sndGeyser.wav", CSoundMgr::EFFECT);

			m_fIntervalTime = 0.f;
		}
	}
}

void CPoisionBomb::Render()
{
	if (!m_bIsHit)
	{
		CObj::UpdateRect();

		const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
			m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), (int)m_tFrame.fFrame);

		NULL_CHECK(pTexInfo);

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
			nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		if (CObjMgr::GetInstance()->GetPlayer()->GetTestView())
			RenderRect();
	}
}

void CPoisionBomb::RenderRect()
{
	D3DXVECTOR2 m_Line[5];
	m_Line[0] = { (float)m_tRect.left + CScrollMgr::GetScroll().x, (float)m_tRect.top + CScrollMgr::GetScroll().y };
	m_Line[1] = { (float)m_tRect.right + CScrollMgr::GetScroll().x, (float)m_tRect.top + CScrollMgr::GetScroll().y };
	m_Line[2] = { (float)m_tRect.right + CScrollMgr::GetScroll().x, (float)m_tRect.bottom + CScrollMgr::GetScroll().y };
	m_Line[3] = { (float)m_tRect.left + CScrollMgr::GetScroll().x, (float)m_tRect.bottom + CScrollMgr::GetScroll().y };
	m_Line[4] = { (float)m_tRect.left + CScrollMgr::GetScroll().x, (float)m_tRect.top + CScrollMgr::GetScroll().y };

	CDevice::GetInstance()->GetSprite()->End();
	CDevice::GetInstance()->GetLine()->Begin();
	CDevice::GetInstance()->GetLine()->Draw(m_Line, 5, D3DCOLOR_XRGB(0, 0, 255));
	CDevice::GetInstance()->GetLine()->End();
	CDevice::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
}

void CPoisionBomb::Release()
{
}