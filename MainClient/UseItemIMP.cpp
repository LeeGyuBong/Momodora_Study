#include "stdafx.h"
#include "UseItemIMP.h"

#include "Effect.h"
#include "AnimeEffect.h"


CUseItemIMP::CUseItemIMP()
	: m_bIsEffect(false)
{
}


CUseItemIMP::~CUseItemIMP()
{
	Release();
}

void CUseItemIMP::Initialize()
{
	m_wstrStateKey = L"KahoItem";
	m_tFrame.fFrame = 0.f;
	m_tFrame.fMax = CTextureMgr::GetInstance()->GetTextureSize(
		m_pObj->GetObjKey().c_str(), m_wstrStateKey.c_str());
	m_tFrame.fRestartFrame = 0.f;
	m_tFrame.fFrameAccel = 1.5f;
}

void CUseItemIMP::LateInit()
{
}

int CUseItemIMP::Update()
{
	CPlayerIMP::LateInit();

	if (!m_bIsEffect)
	{
		if ((int)m_tFrame.fFrame == 2)
		{
			CObjMgr::GetInstance()->AddObject(CEffectFactory<CEffect, CAnimeEffect>::CreateEffect(L"Player", L"UseItem", 
				D3DXVECTOR3(m_pObj->GetInfo().vPos.x, m_pObj->GetInfo().vPos.y + 10.f, 0.f), 
				FRAME(m_tFrame.fFrameAccel), 200), OBJECT::EFFECT);
			m_bIsEffect = true;
		}
	}
	
	m_tFrame.fFrame += m_tFrame.fMax * CTimeMgr::GetInstance()->GetTime() * m_tFrame.fFrameAccel;

	if (m_tFrame.fFrame > m_tFrame.fMax)
		return ANIMATION_END;

	return NO_EVENT;
}

void CUseItemIMP::LateUpdate()
{
}

void CUseItemIMP::Release()
{
}
