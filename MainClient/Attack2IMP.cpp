#include "stdafx.h"
#include "Attack2IMP.h"
#include "AttackEffect.h"
#include "AnimeEffect.h"

CAttack2IMP::CAttack2IMP()
{
}


CAttack2IMP::~CAttack2IMP()
{
	Release();
}

void CAttack2IMP::Initialize()
{
	m_wstrStateKey = L"KahoAttack2";
	m_tFrame.fFrame = 0.f;
	m_tFrame.fMax = CTextureMgr::GetInstance()->GetTextureSize(
		m_pObj->GetObjKey().c_str(), m_wstrStateKey.c_str());
	m_tFrame.fRestartFrame = 0.f;
	m_tFrame.fFrameAccel = 2.3f;

}

void CAttack2IMP::LateInit()
{
	CObjMgr::GetInstance()->AddObject(CEffectFactory<CAttackEffect, CAnimeEffect>::
		CreateEffect(m_pObj->GetObjKey().c_str(), L"Attack2Effect",
			D3DXVECTOR3(50.f, 0.f, 0.f), FRAME(m_tFrame.fFrameAccel)), 
		OBJECT::PLAYER_ATTACK);
}

int CAttack2IMP::Update()
{
	CPlayerIMP::LateInit();

	if (m_tFrame.fFrame < m_tFrame.fMax)
		m_tFrame.fFrame += m_tFrame.fMax * CTimeMgr::GetInstance()->GetTime() * m_tFrame.fFrameAccel;
	else
	{
		m_tFrame.fFrame = m_tFrame.fMax;
		return ATTACK_END;
	}

	return NO_EVENT;
}

void CAttack2IMP::LateUpdate()
{
}

void CAttack2IMP::Release()
{
}
