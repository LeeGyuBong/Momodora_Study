#include "stdafx.h"
#include "Slot.h"


CSlot::CSlot()
	: m_pItem(nullptr), m_iAlpha(0), m_iCurAlpha(0), m_iCalcul(1), m_fAlphaTime(0.f)
{
}


CSlot::~CSlot()
{
	Release();
}

void CSlot::Initialize()
{
	m_wstrObjKey = L"UI";
}

void CSlot::LateInit()
{
}

int CSlot::Update()
{
	CObj::LateInit();

	if (m_iAlpha < 255)
		m_iAlpha += 5;
	if (m_iAlpha > 255)
		m_iAlpha = 255;

	if (m_bIsCur)
	{
		m_fAlphaTime += CTimeMgr::GetInstance()->GetTime();

		if (m_iCurAlpha >= 180)
			m_iCalcul = -1;
		else if (m_iCurAlpha <= 50)
			m_iCalcul = 1;

		if (m_fAlphaTime > 0.006f)
		{
			m_iCurAlpha += m_iCalcul;
			m_fAlphaTime = 0.f;
		}
	}

	m_tInfo.matWorld = CMath::CalculateMatrix(m_tInfo.vPos);

	return NO_EVENT;
}

void CSlot::LateUpdate()
{
}

void CSlot::Render()
{
	const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(m_wstrObjKey.c_str(), L"InventoryBox", 0);
	NULL_CHECK(pTexInfo);
	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

	if (m_pItem != nullptr)
	{
		pTexInfo = CTextureMgr::GetInstance()->GetTexture(m_wstrObjKey.c_str(), L"Item", m_pItem->ID - 1);
		NULL_CHECK(pTexInfo);
		fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
			nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (m_bIsCur)
	{
		pTexInfo = CTextureMgr::GetInstance()->GetTexture(m_wstrObjKey.c_str(), L"InventoryBox", 5);
		NULL_CHECK(pTexInfo);
		fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
			nullptr, D3DCOLOR_ARGB(m_iCurAlpha, 255, 255, 255));
	}
}

void CSlot::Release()
{
}
