#include "stdafx.h"
#include "Money.h"

#include "Effect.h"
#include "AnimeEffect.h"

CMoney::CMoney()
	: m_fPosY(0.f), m_fJumpPower(0.f), m_fJumpTime(0.f),
	m_fLifeTime(0.f)
{
}

CMoney::~CMoney()
{
	Release();
}

void CMoney::Initialize()
{
	m_tInfo.vSize = { 14.f, 14.f, 0.f };
	m_vModify = { 0.f, 0.f };

	m_fJumpPower = float(rand() % 40 + 10);

	if ((rand() % 2 + 1) % 2 == 0)
		m_tInfo.vDir.x = 1.f;
	else
		m_tInfo.vDir.x = -1.f;

	m_fSpeed = float(rand() % 10 + 50);
}

void CMoney::LateInit()
{
	m_tInfo.vPos.y -= 50.f;
	m_fPosY = m_tInfo.vPos.y;
}

int CMoney::Update()
{
	CObj::LateInit();

	if (m_bIsDead)
		return DEAD_OBJ;

	m_fLifeTime += CTimeMgr::GetInstance()->GetTime();

	if (m_fJumpPower > 0)
	{
		float Height = (powf(m_fJumpTime, 2.f) * -GRAVITY * 0.5f) + (m_fJumpTime * m_fJumpPower);

		m_tInfo.vPos.y = m_fPosY - Height;
		m_tInfo.vPos.x += m_fSpeed * m_tInfo.vDir.x * CTimeMgr::GetInstance()->GetTime();

		m_fJumpTime += CTimeMgr::GetInstance()->GetTime() * 10.f;


		if (m_bIsFloorColli && m_fJumpTime > 0.2f)
		{
			m_fJumpTime = 0.f;
			m_fPosY = m_tInfo.vPos.y;
			m_fJumpPower -= rand() % 8 + 1;

			m_bIsFloorColli = false;
		}
	}

	++m_fDegree;

	m_tInfo.matWorld = CMath::CalculateMatrix(m_tInfo.vPos + CScrollMgr::GetScroll(), D3DXVECTOR3(1.f, 1.f, 0.f), ROTATION::Z, m_fDegree);

	return NO_EVENT;
}

void CMoney::LateUpdate()
{
	if (m_fLifeTime > 15.f)
		m_bIsDead = true;

	if (m_eColliObjType == OBJECT::PLAYER)
	{
		CObjMgr::GetInstance()->AddObject(CEffectFactory<CEffect, CAnimeEffect>::CreateEffect(L"Effect", L"MoneyEffect",
			D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y - 5.f, 0.f), FRAME(3.f)), OBJECT::EFFECT);

		m_bIsDead = true;
	}
}

void CMoney::Render()
{
	CObj::UpdateRect();

	const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
		L"Item", L"Money", 0);

	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
		D3DCOLOR_ARGB(255, 255,	255, 255));

	if (CObjMgr::GetInstance()->GetPlayer()->GetTestView())
		RenderRect();
}

void CMoney::RenderRect()
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

void CMoney::Release()
{
}
