#include "stdafx.h"
#include "Effect.h"

CEffect::CEffect()
	: m_pBridge(nullptr), m_fLifeTime(0.f), m_fTime(0.f), m_fDegree(0.f)
{
	m_tInfo.vDir = { 1.f, 0.f, 0.f };
}


CEffect::~CEffect()
{
	Release();
}

void CEffect::Initialize()
{
	m_pObserver = new CPlayerInfo;
	CDataSubject::GetInstance()->Subscribe(m_pObserver);
}

void CEffect::LateInit()
{
}

int CEffect::Update()
{
	CObj::LateInit();

	if (m_bIsDead)
		return DEAD_OBJ;

	if (m_fLifeTime != 0.f)
		m_fTime += CTimeMgr::GetInstance()->GetTime();

	D3DXVECTOR3 vPos = m_tInfo.vPos + CScrollMgr::GetScroll();

	m_tInfo.matWorld = CMath::CalculateMatrix(vPos, D3DXVECTOR3(m_tInfo.vDir.x, 1.f, 0.f), ROTATION::Z, m_fDegree);
	m_pBridge->SetInfo(m_tInfo);

	return m_pBridge->Update();
}

void CEffect::LateUpdate()
{
	if (m_fLifeTime != 0.f)
	{
		if (m_fTime > m_fLifeTime)
			m_bIsDead = true;
	}
	
	m_pBridge->LateUpdate();
}

void CEffect::Render()
{
	CObj::UpdateRect();

	m_pBridge->Render();
}

void CEffect::Release()
{
	SafeDelete(m_pBridge);

	SafeDeleteObserver<CDataSubject>(m_pObserver);
}
