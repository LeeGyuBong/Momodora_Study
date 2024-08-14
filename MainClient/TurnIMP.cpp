#include "stdafx.h"
#include "TurnIMP.h"


CTurnIMP::CTurnIMP()
{
}


CTurnIMP::~CTurnIMP()
{
	Release();
}

void CTurnIMP::Initialize()
{
	m_wstrStateKey = L"KahoTurn";
	m_tFrame.fFrame = 0.f;
	m_tFrame.fMax = CTextureMgr::GetInstance()->GetTextureSize(
		m_pObj->GetObjKey().c_str(), m_wstrStateKey.c_str());
	m_tFrame.fRestartFrame = 0.f;
	m_tFrame.fFrameAccel = 3.f;
}

void CTurnIMP::LateInit()
{
}

int CTurnIMP::Update()
{
	CPlayerIMP::LateInit();

	m_tFrame.fFrame += m_tFrame.fMax * CTimeMgr::GetInstance()->GetTime() * m_tFrame.fFrameAccel;

	if (m_tFrame.fFrame > m_tFrame.fMax)
		return ANIMATION_END;

	return NO_EVENT;
}

void CTurnIMP::LateUpdate()
{
}

void CTurnIMP::Release()
{
}
