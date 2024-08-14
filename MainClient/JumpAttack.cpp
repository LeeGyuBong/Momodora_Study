#include "stdafx.h"
#include "JumpAttack.h"
#include "AttackEffect.h"
#include "AnimeEffect.h"

CJumpAttack::CJumpAttack()
{
}


CJumpAttack::~CJumpAttack()
{
	Release();
}

void CJumpAttack::Initialize()
{
	m_wstrStateKey = L"KahoAttackAir";
	m_tFrame.fFrame = 0.f;
	m_tFrame.fMax = CTextureMgr::GetInstance()->GetTextureSize(
		m_pObj->GetObjKey().c_str(), m_wstrStateKey.c_str());
	m_tFrame.fRestartFrame = m_tFrame.fMax;
	m_tFrame.fFrameAccel = 2.f;
}

void CJumpAttack::LateInit()
{
	CObjMgr::GetInstance()->AddObject(CEffectFactory<CAttackEffect, CAnimeEffect>::
		CreateEffect(m_pObj->GetObjKey().c_str(), L"AttackAirEffect",
			D3DXVECTOR3(50.f, 10.f, 0.f), FRAME(m_tFrame.fFrameAccel)),
		OBJECT::PLAYER_ATTACK);
}

int CJumpAttack::Update()
{
	CPlayerIMP::LateInit();

	m_tFrame.fFrame += m_tFrame.fMax * CTimeMgr::GetInstance()->GetTime() * m_tFrame.fFrameAccel;

	if (m_tFrame.fFrame > m_tFrame.fMax)
		m_tFrame.fFrame = m_tFrame.fRestartFrame;

	return NO_EVENT;
}

void CJumpAttack::LateUpdate()
{
}

void CJumpAttack::Release()
{
}
