#include "stdafx.h"
#include "JumpEffect.h"


CJumpEffect::CJumpEffect()
	: m_iAlpha(200), m_fTime(0.f), m_fSpinDir(0.f), m_fAlphaTime(0.f)
{
}


CJumpEffect::~CJumpEffect()
{
	Release();
}

void CJumpEffect::Initialize()
{
	if ((rand() % 2 + 1) % 2 == 0)
		m_fSpinDir = 0.5f;
	else
		m_fSpinDir = -0.5f;

	m_fDegree = float(rand() % 360);
}

void CJumpEffect::LateInit()
{
}

int CJumpEffect::Update()
{
	CObj::LateInit();

	if (m_bIsDead)
		return DEAD_OBJ;
	
	m_fTime += CTimeMgr::GetInstance()->GetTime();

	if (m_fTime > 0.3f)
	{
		m_fDegree += m_fSpinDir;

		m_fAlphaTime += CTimeMgr::GetInstance()->GetTime();
		m_tInfo.vPos.y += 50.f * CTimeMgr::GetInstance()->GetTime();
	}

	if(m_fAlphaTime > 0.1f)
		--m_iAlpha;

	m_tInfo.matWorld = CMath::CalculateMatrix(m_tInfo.vPos + CScrollMgr::GetScroll(), D3DXVECTOR3(1.f, 1.f, 0.f), ROTATION::Z, m_fDegree);

	return NO_EVENT;
}

void CJumpEffect::LateUpdate()
{
	if (m_iAlpha <= 0)
		m_bIsDead = true;

}

void CJumpEffect::Render()
{
	if (m_iAlpha <= 0)
		return;

	const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
		L"Player", L"Effect", 7);

	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
		D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
}

void CJumpEffect::Release()
{
}
