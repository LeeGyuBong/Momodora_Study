#include "stdafx.h"
#include "RiesIMP.h"


CRiesIMP::CRiesIMP()
{
}


CRiesIMP::~CRiesIMP()
{
	Release();
}

void CRiesIMP::Initialize()
{
	m_wstrStateKey = L"KahoRies";
	m_tFrame.fFrame = 0.f;
	m_tFrame.fMax = CTextureMgr::GetInstance()->GetTextureSize(
		m_pObj->GetObjKey().c_str(), m_wstrStateKey.c_str());
	m_tFrame.fRestartFrame = 0.f;
	m_tFrame.fFrameAccel = 15.f;
}

void CRiesIMP::LateInit()
{
	
}

int CRiesIMP::Update()
{
	CPlayerIMP::LateInit();

	m_tFrame.fFrame += m_tFrame.fMax * CTimeMgr::GetInstance()->GetTime() * m_tFrame.fFrameAccel;

	if (m_tFrame.fFrame > m_tFrame.fMax)
		return ANIMATION_END;

	return NO_EVENT;
}

void CRiesIMP::LateUpdate()
{
}

void CRiesIMP::Release()
{
}
