#include "stdafx.h"
#include "MoveIMP.h"


CMoveIMP::CMoveIMP()
{
}


CMoveIMP::~CMoveIMP()
{
	Release();
}

void CMoveIMP::Initialize()
{
	m_wstrStateKey = L"KahoWalk";
	m_tFrame.fFrame = 0.f;
	m_tFrame.fMax = CTextureMgr::GetInstance()->GetTextureSize(
		m_pObj->GetObjKey().c_str(), m_wstrStateKey.c_str());
	m_tFrame.fRestartFrame = 2.f;
	m_tFrame.fFrameAccel = 1.f;
}

void CMoveIMP::LateInit()
{
}

int CMoveIMP::Update()
{
	CPlayerIMP::LateInit();

	m_tFrame.fFrame += m_tFrame.fMax * CTimeMgr::GetInstance()->GetTime() * m_tFrame.fFrameAccel;

	if (m_tFrame.fFrame > m_tFrame.fMax)
		m_tFrame.fFrame = m_tFrame.fRestartFrame;

	return NO_EVENT;
}

void CMoveIMP::LateUpdate()
{
}

void CMoveIMP::Release()
{
}
