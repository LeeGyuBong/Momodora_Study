#include "stdafx.h"
#include "RollIMP.h"


CRollIMP::CRollIMP()
{
}

CRollIMP::~CRollIMP()
{
	Release();
}

void CRollIMP::Initialize()
{
	m_wstrStateKey = L"KahoRoll";
	m_tFrame.fFrame = 0.f;
	m_tFrame.fMax = CTextureMgr::GetInstance()->GetTextureSize(
		m_pObj->GetObjKey().c_str(), m_wstrStateKey.c_str());
	m_tFrame.fRestartFrame = 0.f;
	m_tFrame.fFrameAccel = 2.f;
}

void CRollIMP::LateInit()
{
}

int CRollIMP::Update()
{
	CPlayerIMP::LateInit();

	m_tFrame.fFrame += m_tFrame.fMax * CTimeMgr::GetInstance()->GetTime() * m_tFrame.fFrameAccel;

	if (m_tFrame.fFrame > m_tFrame.fMax)
		return ANIMATION_END;

	return NO_EVENT;
}

void CRollIMP::LateUpdate()
{
}

void CRollIMP::Release()
{
}
