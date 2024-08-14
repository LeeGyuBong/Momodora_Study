#include "stdafx.h"
#include "NonAnimeEffect.h"


CNonAnimeEffect::CNonAnimeEffect()
	: m_fTime(0.f)
{
}


CNonAnimeEffect::~CNonAnimeEffect()
{
	Release();
}

void CNonAnimeEffect::Initialize()
{
}

void CNonAnimeEffect::LateInit()
{
}

int CNonAnimeEffect::Update()
{
	CEffectIMP::LateInit();

	return NO_EVENT;
}

void CNonAnimeEffect::LateUpdate()
{
}

void CNonAnimeEffect::Render()
{

	const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
		m_pObj->GetObjKey().c_str(), m_wstrStateKey.c_str(), m_iDrawID);

	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
}

void CNonAnimeEffect::Release()
{
}
