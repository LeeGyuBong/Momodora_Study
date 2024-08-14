#include "stdafx.h"
#include "PlayerHp.h"

#include "PlayerInfo.h"

CPlayerHp::CPlayerHp()
	: m_iHp(0), m_iMaxHp(0), m_iPreHp(0), m_iDamage(0), m_fDamageTime(0.f)
{
}


CPlayerHp::~CPlayerHp()
{
	Release();
}

void CPlayerHp::Initialize()
{
	m_tInfo.vPos = { 100.f, 50.f, 0.f };

	m_wstrObjKey = L"UI";
	m_wstrStateKey = L"HpBar";

	m_pObserver = new CPlayerInfo;
	CDataSubject::GetInstance()->Subscribe(m_pObserver);
}

void CPlayerHp::LateInit()
{
	m_iHp = dynamic_cast<CPlayerInfo*>(m_pObserver)->GetStatus().HP;
	m_iPreHp = m_iHp;
}

int CPlayerHp::Update()
{
	CObj::LateInit();

	if (m_bIsDead)
		return DEAD_OBJ;

	m_iPreHp = m_iHp;
	m_iHp = dynamic_cast<CPlayerInfo*>(m_pObserver)->GetStatus().HP;
	m_iMaxHp = dynamic_cast<CPlayerInfo*>(m_pObserver)->GetStatus().MaxHP;

	if (m_iPreHp != m_iHp)
		m_iDamage += m_iPreHp - m_iHp;

	if (m_iDamage)
		m_fDamageTime += CTimeMgr::GetInstance()->GetTime();
	if (m_fDamageTime > 0.2f)
	{
		--m_iDamage;
		m_fDamageTime = 0.f;
	}

	return NO_EVENT;
}

void CPlayerHp::LateUpdate()
{
	if (m_iHp <= 0)
		m_bIsDead = true;
}

void CPlayerHp::Render()
{
	RenderSurface();
	RenderInside();
}

void CPlayerHp::Release()
{
	SafeDeleteObserver<CDataSubject>(m_pObserver);
}

void CPlayerHp::RenderSurface()
{
	for (int i = 1; i <= m_iMaxHp + 3; ++i)
	{
		if (i == 1 || i == m_iMaxHp + 2)
			m_tFrame.fFrame = 0.f;
		else if (i == m_iMaxHp + 3)
			m_tFrame.fFrame = 2.f;
		else
			m_tFrame.fFrame = 1.f;

		const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
			m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), (int)m_tFrame.fFrame);

		NULL_CHECK(pTexInfo);

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		D3DXVECTOR3 vPos = m_tInfo.vPos;
		if (i == m_iMaxHp + 3)
			vPos.x += static_cast<float>(i * (pTexInfo->tImgInfo.Width - 6));
		else
			vPos.x += static_cast<float>(i * pTexInfo->tImgInfo.Width);

		m_tInfo.matWorld = CMath::CalculateMatrix(vPos);

		CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CPlayerHp::RenderInside()
{
	const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
		m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), 7);
	NULL_CHECK(pTexInfo);
	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;
	for (int i = 0; i < m_iDamage; ++i)
	{
		D3DXMATRIX matWorld;
		matWorld = CMath::CalculateMatrix(D3DXVECTOR3(m_tInfo.vPos.x + (pTexInfo->tImgInfo.Width * (m_iHp + 2)) + (i * pTexInfo->tImgInfo.Width), m_tInfo.vPos.y, 0.f));

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	for (int i = 1; i <= m_iHp; ++i)
	{
		if (i == 1)
			m_tFrame.fFrame = 3.f;
		else if (i == 2)
			m_tFrame.fFrame = 4.f;
		else if (i == m_iHp)
			m_tFrame.fFrame = 6.f;
		else
			m_tFrame.fFrame = 5.f;

		const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
			m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), (int)m_tFrame.fFrame);

		NULL_CHECK(pTexInfo);

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		D3DXVECTOR3 vPos = m_tInfo.vPos;
			vPos.x += float(i * pTexInfo->tImgInfo.Width) + 4;

		m_tInfo.matWorld = CMath::CalculateMatrix(vPos);

		CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}