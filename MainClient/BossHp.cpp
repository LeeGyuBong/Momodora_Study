#include "stdafx.h"
#include "BossHp.h"

#include "BossInfo.h"

CBossHp::CBossHp()
	: m_iHp(0), m_iPreHp(0), m_iDamage(0), m_fTime(0.f), m_fDamageTime(0.f)
{
}


CBossHp::~CBossHp()
{
	Release();
}

void CBossHp::Initialize()
{
	m_tInfo.vPos = { CLIENTWINCX * 0.5f, CLIENTWINCY * 0.5f + 200.f, 0.f };

	m_wstrObjKey = L"UI";
	m_wstrStateKey = L"BossHpBar";

	m_pObserver = new CBossInfo;
	CDataSubject::GetInstance()->Subscribe(m_pObserver);
}

void CBossHp::LateInit()
{
	m_iHp = dynamic_cast<CBossInfo*>(m_pObserver)->GetStatus().HP;
	m_iPreHp = m_iHp;
}

int CBossHp::Update()
{
	CObj::LateInit();

	if (m_bIsDead)
		return DEAD_OBJ;

	m_iPreHp = m_iHp;
	m_iHp = dynamic_cast<CBossInfo*>(m_pObserver)->GetStatus().HP;

	if (m_iPreHp != m_iHp)
		m_iDamage += m_iPreHp - m_iHp;

	if (m_iDamage)
		m_fDamageTime += CTimeMgr::GetInstance()->GetTime();
	if (m_fDamageTime > 0.2f)
	{
		--m_iDamage;
		m_fDamageTime = 0.f;
	}

	m_tInfo.matWorld = CMath::CalculateMatrix(m_tInfo.vPos);

	return NO_EVENT;
}

void CBossHp::LateUpdate()
{
	if (m_iHp <= 0)
		m_bIsDead = true;

	m_fTime += CTimeMgr::GetInstance()->GetTime();
}

void CBossHp::Render()
{
	RenderSurface();
	RenderInside();

	CDevice::GetInstance()->DoubleFontText(L"¼ÓÁËÀÚ Æä³Ú", D3DXVECTOR3(100.f, 450.f, 0.f),
		D3DCOLOR_XRGB(255, 0, 0), D3DCOLOR_XRGB(255, 255, 255));
}

void CBossHp::RenderSurface()
{
	const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
		m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), 0);

	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CBossHp::RenderInside()
{
	const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
		m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), 4);
	NULL_CHECK(pTexInfo);
	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;
	for (int i = 0; i < m_iDamage; ++i)
	{
		D3DXMATRIX matWorld;
		matWorld = CMath::CalculateMatrix(D3DXVECTOR3(m_tInfo.vPos.x + (pTexInfo->tImgInfo.Width * m_iHp) - 222.f + (i * pTexInfo->tImgInfo.Width), m_tInfo.vPos.y, 0.f));

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	for (int i = 1; i <= m_iHp; ++i)
	{
		if (i == m_iHp)
		{
			if (m_fTime > 0.4f)
				m_fTime = 0.f;

			if(m_fTime < 0.2f)
				m_tFrame.fFrame = 2.f;

			if(m_fTime > 0.2f)
				m_tFrame.fFrame = 3.f;
		}
		else
			m_tFrame.fFrame = 1.f;

		const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
			m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), (int)m_tFrame.fFrame);

		NULL_CHECK(pTexInfo);

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		D3DXMATRIX matWorld;
		matWorld = CMath::CalculateMatrix(D3DXVECTOR3(m_tInfo.vPos.x + (pTexInfo->tImgInfo.Width * i) - 224.f, m_tInfo.vPos.y, 0.f));

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CBossHp::Release()
{
	SafeDeleteObserver<CDataSubject>(m_pObserver);
}
