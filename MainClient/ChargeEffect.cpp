#include "stdafx.h"
#include "ChargeEffect.h"


CChargeEffect::CChargeEffect()
	: m_fTime(0.f)
{
}


CChargeEffect::~CChargeEffect()
{
	Release();
}

void CChargeEffect::Initialize()
{
	CEffect::Initialize();
}

void CChargeEffect::LateInit()
{
	CEffect::LateInit();
}

int CChargeEffect::Update()
{
	CObj::LateInit();

	if (m_fLifeTime < m_fTime)
		return DEAD_OBJ;

	D3DXVECTOR3 pPos = dynamic_cast<CPlayerInfo*>(m_pObserver)->GetInfo().vPos;
	D3DXVECTOR3 Dir = pPos - m_tInfo.vPos;
	
	m_tInfo.vPos += 5.f * Dir * CTimeMgr::GetInstance()->GetTime();

	m_fTime += CTimeMgr::GetInstance()->GetTime();

	m_tInfo.matWorld = CMath::CalculateMatrix(m_tInfo.vPos + CScrollMgr::GetScroll());
	m_pBridge->SetInfo(m_tInfo);

	return m_pBridge->Update();
}

void CChargeEffect::LateUpdate()
{
	CEffect::LateUpdate();
	
}

void CChargeEffect::Render()
{
	CEffect::Render();
}

void CChargeEffect::Release()
{
}
