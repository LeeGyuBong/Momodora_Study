#include "stdafx.h"
#include "DeadIMP.h"


CDeadIMP::CDeadIMP()
	: m_bIsSound(false)
{
}


CDeadIMP::~CDeadIMP()
{
	Release();
}

void CDeadIMP::Initialize()
{
	m_wstrStateKey = L"KahoDeath";
	m_tFrame.fFrame = -7.f;
	m_tFrame.fMax = CTextureMgr::GetInstance()->GetTextureSize(
		m_pObj->GetObjKey().c_str(), m_wstrStateKey.c_str());
	m_tFrame.fRestartFrame = m_tFrame.fMax;
	m_tFrame.fFrameAccel = 0.45f;
}

void CDeadIMP::LateInit()
{
}

int CDeadIMP::Update()
{
	CPlayerIMP::LateInit();

	m_tFrame.fFrame += m_tFrame.fMax * CTimeMgr::GetInstance()->GetTime() * m_tFrame.fFrameAccel;

	if (m_tFrame.fFrame > m_tFrame.fMax)
		m_tFrame.fFrame = m_tFrame.fRestartFrame;

	if (!m_bIsSound && (int)m_tFrame.fFrame == 3)
	{
		CSoundMgr::GetInstance()->PlaySound(L"sndKahoDeath.wav", CSoundMgr::PLAYER);
		m_bIsSound = false;
	}

	return NO_EVENT;
}

void CDeadIMP::LateUpdate()
{
}

void CDeadIMP::Release()
{
}
