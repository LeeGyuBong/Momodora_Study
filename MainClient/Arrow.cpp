#include "stdafx.h"
#include "Arrow.h"
#include "PlayerInfo.h"

#include "Effect.h"
#include "AnimeEffect.h"

CArrow::CArrow()
	: m_wstrStateKey(L""), m_fLifeTime(0.f), m_fDirX(0.f), m_bIsBounce(false),
	m_fJumpTime(0.f), m_fPosY(0.f)
{
}


CArrow::~CArrow()
{
	Release();
}

void CArrow::Initialize()
{
	m_pObserver = new CPlayerInfo;
	CDataSubject::GetInstance()->Subscribe(m_pObserver);

	m_tInfo.vSize = { 10.f, 6.f, 0.f };
	m_vModify = { 19.f, -1.f };

	m_wstrObjKey = L"Player";
	m_wstrStateKey = L"Effect";
	m_tFrame.fFrame = 5.f;

	m_fSpeed = 1300.f;
}

void CArrow::LateInit()
{
	m_tInfo.vPos += dynamic_cast<CPlayerInfo*>(m_pObserver)->GetInfo().vPos;
	m_tInfo.vDir = dynamic_cast<CPlayerInfo*>(m_pObserver)->GetInfo().vDir;

	m_fDirX = m_tInfo.vDir.x;
	m_vModify.x *= m_fDirX;
}

int CArrow::Update()
{
	CObj::LateInit();

	if (m_bIsDead)
		return DEAD_OBJ;

	if (m_bIsBounce)
	{
		Bouncing();
		++m_fDegree;
	}
	else
		m_tInfo.vPos += m_tInfo.vDir * m_fSpeed * CTimeMgr::GetInstance()->GetTime();
	
	m_fLifeTime += CTimeMgr::GetInstance()->GetTime();

	m_tInfo.matWorld = CMath::CalculateMatrix(m_tInfo.vPos + CScrollMgr::GetScroll(), D3DXVECTOR3(m_fDirX, 1.f, 0.f), Z, m_fDegree);
	D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vLook, &m_tInfo.matWorld);

	return NO_EVENT;
}

void CArrow::LateUpdate()
{
	if (m_fLifeTime > 5.f)
		m_bIsDead = true;

	if (m_bIsHit)
	{
		switch (m_eColliObjType)
		{
		case OBJECT::TILE:
			if(m_fSpeed > 0.f)
				m_fSpeed -= 100.f;
			break;
		case OBJECT::ENEMY_ATTACK:
			m_bIsBounce = true;
			m_fDirX *= -1.f;
			m_fPosY = m_tInfo.vPos.y;
			m_tInfo.vSize = { 0.f, 0.f, 0.f };
			NotHit();
			CObjMgr::GetInstance()->AddObject(CEffectFactory<CEffect, CAnimeEffect>::CreateEffect(L"HitEffect", L"HitArrow",
				D3DXVECTOR3(m_tInfo.vPos), FRAME(2.5f)), OBJECT::EFFECT);
			CSoundMgr::GetInstance()->PlaySound(L"sndNoDamage.wav", CSoundMgr::EFFECT);
			break;
		case OBJECT::ENEMY:
		case OBJECT::BOSS:
			m_bIsDead = true;
			CObjMgr::GetInstance()->AddObject(CEffectFactory<CEffect, CAnimeEffect>::CreateEffect(L"HitEffect", L"HitArrow",
				D3DXVECTOR3(m_tInfo.vPos), FRAME(2.5f)), OBJECT::EFFECT);
			break;
		}
	}
}

void CArrow::Render()
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

	if(CObjMgr::GetInstance()->GetPlayer()->GetTestView())
		RenderRect();
}

void CArrow::RenderRect()
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

void CArrow::Release()
{
	SafeDeleteObserver<CDataSubject>(m_pObserver);
}

void CArrow::Bouncing()
{
	float Height = (powf(m_fJumpTime, 2.f) * -GRAVITY * 0.5f) + (m_fJumpTime * 50.f);

	m_tInfo.vPos.x += m_fDirX * 200.f * CTimeMgr::GetInstance()->GetTime();
	m_tInfo.vPos.y = m_fPosY - Height;
	m_fJumpTime += CTimeMgr::GetInstance()->GetTime() * 10.f;
}