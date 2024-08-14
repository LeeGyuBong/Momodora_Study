#include "stdafx.h"
#include "AttackEffect.h"

CAttackEffect::CAttackEffect()
{
}

CAttackEffect::~CAttackEffect()
{
	Release();
}

void CAttackEffect::Initialize()
{
	CEffect::Initialize();
}

void CAttackEffect::LateInit()
{
	CEffect::LateInit();
}

int CAttackEffect::Update()
{
	CObj::LateInit();

	m_tInfo.matWorld = CMath::CalculateMatrix(m_tInfo.vPos) * 
		dynamic_cast<CPlayerInfo*>(m_pObserver)->GetInfo().matWorld;
	m_pBridge->SetInfo(m_tInfo);

	return m_pBridge->Update();
}

void CAttackEffect::LateUpdate()
{
	CEffect::LateUpdate();
}

void CAttackEffect::Render()
{
	CEffect::Render();
}

void CAttackEffect::Release()
{
}
