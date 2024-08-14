#include "stdafx.h"
#include "AnimeEffect.h"

CAnimeEffect::CAnimeEffect()
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}


CAnimeEffect::~CAnimeEffect()
{
	Release();
}

void CAnimeEffect::Initialize()
{
}

void CAnimeEffect::LateInit()
{	
	m_tFrame.fFrame = 0.f;
	m_tFrame.fMax = CTextureMgr::GetInstance()->GetTextureSize(m_pObj->GetObjKey().c_str(), m_wstrStateKey.c_str());
	
	if (m_tFrame.fFrameAccel <= 0.f)
		m_tFrame.fFrameAccel = 1.f;
}

int CAnimeEffect::Update()
{
	CEffectIMP::LateInit();

	m_tFrame.fFrame += m_tFrame.fMax * CTimeMgr::GetInstance()->GetTime() * m_tFrame.fFrameAccel;

	if (m_tFrame.fFrame > m_tFrame.fMax)
		return DEAD_OBJ;

	return NO_EVENT;
}

void CAnimeEffect::LateUpdate()
{
}

void CAnimeEffect::Render()
{
	const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
		m_pObj->GetObjKey().c_str(), m_wstrStateKey.c_str(), (int)m_tFrame.fFrame);

	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
}

void CAnimeEffect::Release()
{
}
