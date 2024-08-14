#include "stdafx.h"
#include "BrakeIMP.h"


CBrakeIMP::CBrakeIMP()
{
}


CBrakeIMP::~CBrakeIMP()
{
	Release();
}

void CBrakeIMP::Initialize()
{
	m_wstrStateKey = L"KahoBrake";
	m_tFrame.fFrame = 0.f;
	m_tFrame.fMax = CTextureMgr::GetInstance()->GetTextureSize(
		m_pObj->GetObjKey().c_str(), m_wstrStateKey.c_str());
	m_tFrame.fRestartFrame = 0.f;
	m_tFrame.fFrameAccel = 2.f;
}

void CBrakeIMP::LateInit()
{
}

int CBrakeIMP::Update()
{
	CPlayerIMP::LateInit();

	m_tFrame.fFrame += m_tFrame.fMax * CTimeMgr::GetInstance()->GetTime() * m_tFrame.fFrameAccel;

	if (m_tFrame.fFrame > m_tFrame.fMax)
		return ANIMATION_END;

	return NO_EVENT;
}

void CBrakeIMP::LateUpdate()
{
}

void CBrakeIMP::Release()
{
}
