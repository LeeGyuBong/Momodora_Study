#include "stdafx.h"
#include "PlayerIMP.h"

CPlayerIMP::CPlayerIMP()
	: m_pObj(nullptr), m_bIsInit(false), m_wstrStateKey(L"")
{
	m_pObserver = new CPlayerInfo;
	CDataSubject::GetInstance()->Subscribe(m_pObserver);

	ZeroMemory(&m_tFrame, sizeof(FRAME));

	m_pObj = CObjMgr::GetInstance()->GetPlayer();

	m_wstrStateKey = L"KahoIdle";
}


CPlayerIMP::~CPlayerIMP()
{
	SafeDeleteObserver<CDataSubject>(m_pObserver);
}

void CPlayerIMP::LateInit()
{
	if (!m_bIsInit)
	{
		this->LateInit();
		m_bIsInit = true;
	}
}

void CPlayerIMP::Render()
{
	const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
		m_pObj->GetObjKey().c_str(), m_wstrStateKey.c_str(), (int)m_tFrame.fFrame);
	
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_pObj->GetInfo().matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, 
		D3DCOLOR_ARGB(255 + dynamic_cast<CPlayer*>(m_pObj)->GetInviAlpha(),
			255, 
			255 + dynamic_cast<CPlayer*>(m_pObj)->GetChargeBlend(), 
			255 + dynamic_cast<CPlayer*>(m_pObj)->GetChargeBlend())
	);
}
