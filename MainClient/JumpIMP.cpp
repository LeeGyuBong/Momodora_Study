#include "stdafx.h"
#include "JumpIMP.h"


CJumpIMP::CJumpIMP()
{
}


CJumpIMP::~CJumpIMP()
{
	Release();
}

void CJumpIMP::Initialize()
{
	m_wstrStateKey = L"KahoJump";
	m_tFrame.fFrame = 0.f;
	m_tFrame.fMax = CTextureMgr::GetInstance()->GetTextureSize(
		m_pObj->GetObjKey().c_str(), m_wstrStateKey.c_str());
	m_tFrame.fRestartFrame = 0.f;
	m_tFrame.fFrameAccel = 3.5f;
}

void CJumpIMP::LateInit()
{
}

int CJumpIMP::Update()
{
	CPlayerIMP::LateInit();

	m_tFrame.fFrame += m_tFrame.fMax * CTimeMgr::GetInstance()->GetTime() * m_tFrame.fFrameAccel;

	if (m_tFrame.fFrame > m_tFrame.fMax)
		m_tFrame.fFrame = m_tFrame.fRestartFrame;

	return NO_EVENT;
}

void CJumpIMP::LateUpdate()
{
}

void CJumpIMP::Release()
{
}
