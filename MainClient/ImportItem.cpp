#include "stdafx.h"
#include "ImportItem.h"


CImportItem::CImportItem()
	: eType(SLOT), m_iAlpha(0)
{
}


CImportItem::~CImportItem()
{
	Release();
}

void CImportItem::Initialize()
{
	m_tInfo.vPos = { CLIENTWINCX * 0.5f, CLIENTWINCY * 0.5f, 0.f };

}

void CImportItem::LateInit()
{
}

int CImportItem::Update()
{
	CObj::LateInit();

	if (m_iAlpha < 255)
		m_iAlpha += 3;
	if (m_iAlpha > 255)
		m_iAlpha = 255;

	m_tInfo.matWorld = CMath::CalculateMatrix(m_tInfo.vPos);

	return NO_EVENT;
}

void CImportItem::LateUpdate()
{
	Control();
}

void CImportItem::Render()
{
	RenderBack();
}

void CImportItem::Release()
{
}

void CImportItem::RenderBack()
{
	const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"BackGround", L"Effect", 0);
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
}

void CImportItem::Control()
{
	if (CKeyMgr::GetInstance()->KeyDown(KEY_S))
	{
		if (eType == SLOT)
		{
			CUiMgr::GetInstance()->UiChanger(CUiMgr::MENU);
			return;
		}
		else if (eType == INVEN)
			eType = SLOT;
	}

}
