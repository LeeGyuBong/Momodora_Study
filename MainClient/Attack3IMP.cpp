#include "stdafx.h"
#include "Attack3IMP.h"
#include "AttackEffect.h"
#include "AnimeEffect.h"


CAttack3IMP::CAttack3IMP()
{
}


CAttack3IMP::~CAttack3IMP()
{
	Release();
}

void CAttack3IMP::Initialize()
{
	m_wstrStateKey = L"KahoAttack3";
	m_tFrame.fFrame = 0.f;
	m_tFrame.fMax = CTextureMgr::GetInstance()->GetTextureSize(
		m_pObj->GetObjKey().c_str(), m_wstrStateKey.c_str());
	m_tFrame.fRestartFrame = 0.f;
	m_tFrame.fFrameAccel = 2.f;
}

void CAttack3IMP::LateInit()
{
	CObjMgr::GetInstance()->AddObject(CEffectFactory<CAttackEffect, CAnimeEffect>::
		CreateEffect(m_pObj->GetObjKey().c_str(), L"Attack3Effect",
			D3DXVECTOR3(50.f, 0.f, 0.f), FRAME(m_tFrame.fFrameAccel)),
		OBJECT::PLAYER_ATTACK);
}

int CAttack3IMP::Update()
{
	CPlayerIMP::LateInit();

	if (m_tFrame.fFrame < m_tFrame.fMax)
		m_tFrame.fFrame += m_tFrame.fMax * CTimeMgr::GetInstance()->GetTime() * m_tFrame.fFrameAccel;
	else
	{
		m_tFrame.fFrame = m_tFrame.fMax;
		return ANIMATION_END;
	}

	return NO_EVENT;
}

void CAttack3IMP::LateUpdate()
{
}

void CAttack3IMP::Release()
{
}
