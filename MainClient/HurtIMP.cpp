#include "stdafx.h"
#include "HurtIMP.h"


CHurtIMP::CHurtIMP()
{
}


CHurtIMP::~CHurtIMP()
{
	Release();
}

void CHurtIMP::Initialize()
{
	m_wstrStateKey = L"KahoHurt";
	m_tFrame.fFrame = 0.f;
	m_tFrame.fMax = CTextureMgr::GetInstance()->GetTextureSize(
		m_pObj->GetObjKey().c_str(), m_wstrStateKey.c_str());
	m_tFrame.fRestartFrame = 0.f;
	m_tFrame.fFrameAccel = 1.f;
}

void CHurtIMP::LateInit()
{
	
}

int CHurtIMP::Update()
{
	CPlayerIMP::LateInit();

	return NO_EVENT;
}

void CHurtIMP::LateUpdate()
{
}

void CHurtIMP::Release()
{
}
