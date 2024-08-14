#include "stdafx.h"
#include "CrouchIMP.h"


CCrouchIMP::CCrouchIMP()
{
}


CCrouchIMP::~CCrouchIMP()
{
	Release();
}

void CCrouchIMP::Initialize()
{
	m_wstrStateKey = L"KahoCrouch";
	m_tFrame.fFrame = 0.f;
	m_tFrame.fMax = CTextureMgr::GetInstance()->GetTextureSize(
		m_pObj->GetObjKey().c_str(), m_wstrStateKey.c_str());
	m_tFrame.fRestartFrame = 3.f;
	m_tFrame.fFrameAccel = 4.f;
}

void CCrouchIMP::LateInit()
{
}

int CCrouchIMP::Update()
{
	CPlayerIMP::LateInit();

	m_tFrame.fFrame += m_tFrame.fMax * CTimeMgr::GetInstance()->GetTime() * m_tFrame.fFrameAccel;

	if (m_tFrame.fFrame > m_tFrame.fMax)
		m_tFrame.fFrame = m_tFrame.fRestartFrame;

	return NO_EVENT;
}

void CCrouchIMP::LateUpdate()
{
}

void CCrouchIMP::Release()
{
}
