#include "stdafx.h"
#include "FallIMP.h"


CFallIMP::CFallIMP()
{
}


CFallIMP::~CFallIMP()
{
	Release();
}

void CFallIMP::Initialize()
{
	m_wstrStateKey = L"KahoFall";
	m_tFrame.fFrame = 0.f;
	m_tFrame.fMax = CTextureMgr::GetInstance()->GetTextureSize(
		m_pObj->GetObjKey().c_str(), m_wstrStateKey.c_str());
	m_tFrame.fRestartFrame = 2.f;
	m_tFrame.fFrameAccel = 3.5f;
}

void CFallIMP::LateInit()
{
}

int CFallIMP::Update()
{
	CPlayerIMP::LateInit();

	m_tFrame.fFrame += m_tFrame.fMax * CTimeMgr::GetInstance()->GetTime() * m_tFrame.fFrameAccel;

	if (m_tFrame.fFrame > m_tFrame.fMax)
		m_tFrame.fFrame = m_tFrame.fRestartFrame;

	return NO_EVENT;
}

void CFallIMP::LateUpdate()
{
}

void CFallIMP::Release()
{
}
