#include "stdafx.h"
#include "Dagger.h"


CDagger::CDagger()
	: m_fLifeTime(0.f), m_bIsBounce(0.f), m_fJumpTime(0.f), m_fPosY(0.f)
{
}


CDagger::~CDagger()
{
	Release();
}

void CDagger::Initialize()
{
	m_tInfo.vSize = { 22.f, 10.f, 0.f };
	
	m_wstrObjKey = L"Imp";
	m_wstrStateKey = L"ImpEffect";
	m_tFrame.fFrame = 0.f;

	m_fSpeed = 300.f;
	
	m_tStatus.Attack = 1;
}

void CDagger::LateInit()
{
}

int CDagger::Update()
{
	CObj::LateInit();

	if (m_bIsDead)
		return DEAD_OBJ;

	if (!m_bIsBounce)
		m_tInfo.vPos.x += m_tInfo.vDir.x * m_fSpeed * CTimeMgr::GetInstance()->GetTime();
	else
	{
		Bouncing();
		m_fDegree += 2.f;
	}

	m_tInfo.matWorld = CMath::CalculateMatrix(m_tInfo.vPos + CScrollMgr::GetScroll(), D3DXVECTOR3(m_tInfo.vDir.x * -1.f, 1.f, 0.f), ROTATION::Z, m_fDegree);

	m_fLifeTime += CTimeMgr::GetInstance()->GetTime();

	return NO_EVENT;
}

void CDagger::LateUpdate()
{
	if (m_fLifeTime > 5.f)
		m_bIsDead = true;

	if (m_bIsHit)
	{
		switch (m_eColliObjType)
		{
		case OBJECT::PLAYER:
			m_bIsDead = true;
			break;
		case OBJECT::PLAYER_ATTACK:
			m_bIsBounce = true;
			m_tInfo.vDir.x *= -1.f;
			m_fPosY = m_tInfo.vPos.y;
			NotHit();
			CSoundMgr::GetInstance()->PlaySound(L"sndNoDamage.wav", CSoundMgr::EFFECT);
			break;
		}
	}
}

void CDagger::Render()
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

void CDagger::RenderRect()
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

void CDagger::Release()
{
}

void CDagger::Bouncing()
{
	float Height = (powf(m_fJumpTime, 2.f) * -GRAVITY * 0.5f) + (m_fJumpTime * 30.f);

	m_tInfo.vPos.x += m_tInfo.vDir.x * 200.f * CTimeMgr::GetInstance()->GetTime();
	m_tInfo.vPos.y = m_fPosY - Height;
	m_fJumpTime += CTimeMgr::GetInstance()->GetTime() * 10.f;
}