#include "stdafx.h"
#include "Attack1IMP.h"
#include "AttackEffect.h"
#include "AnimeEffect.h"

CAttack1IMP::CAttack1IMP()
{
}


CAttack1IMP::~CAttack1IMP()
{
	Release();
}

void CAttack1IMP::Initialize()
{
	m_wstrStateKey = L"KahoAttack1";
	m_tFrame.fFrame = 0.f;
	m_tFrame.fMax = CTextureMgr::GetInstance()->GetTextureSize(
		m_pObj->GetObjKey().c_str(), m_wstrStateKey.c_str());
	m_tFrame.fRestartFrame = 0.f;
	m_tFrame.fFrameAccel = 2.3f;
}

void CAttack1IMP::LateInit()
{
	CObjMgr::GetInstance()->AddObject(CEffectFactory<CAttackEffect, CAnimeEffect>::
		CreateEffect(m_pObj->GetObjKey().c_str(), L"Attack1Effect", 
			D3DXVECTOR3(50.f, 0.f, 0.f), FRAME(m_tFrame.fFrameAccel)), 
		OBJECT::PLAYER_ATTACK);
}

int CAttack1IMP::Update()
{
	CPlayerIMP::LateInit();
	
	m_tFrame.fFrame += m_tFrame.fMax * CTimeMgr::GetInstance()->GetTime() * m_tFrame.fFrameAccel;

	if (m_tFrame.fFrame > m_tFrame.fMax)
	{
		m_tFrame.fFrame = m_tFrame.fMax;
		return ATTACK_END;
	}

	return NO_EVENT;
}

void CAttack1IMP::LateUpdate()
{
}

void CAttack1IMP::Release()
{
}
