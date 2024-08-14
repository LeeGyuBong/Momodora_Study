#include "stdafx.h"
#include "IdleIMP.h"

CIdleIMP::CIdleIMP()
	: m_iRandomIdle(0), m_fTime(0.f)
{
}


CIdleIMP::~CIdleIMP()
{
	Release();
}

void CIdleIMP::Initialize()
{
	m_wstrStateKey = L"KahoIdle";
	m_tFrame.fFrame = 0.f;
	m_tFrame.fMax = CTextureMgr::GetInstance()->GetTextureSize(
		m_pObj->GetObjKey().c_str(), m_wstrStateKey.c_str());
	m_tFrame.fRestartFrame = 0.f;
	m_tFrame.fFrameAccel = 1.f;
}

void CIdleIMP::LateInit()
{
	
}

int CIdleIMP::Update()
{
	CPlayerIMP::LateInit();

	m_tFrame.fFrame += m_tFrame.fMax * CTimeMgr::GetInstance()->GetTime() * m_tFrame.fFrameAccel;
	m_fTime += CTimeMgr::GetInstance()->GetTime() * m_tFrame.fFrameAccel; // 대기 상태 시간 계산
	 
	if (m_tFrame.fFrame > m_tFrame.fMax)
	{
		// 대기상태가 일정 시간이 지나면
		if (m_fTime > 7.f)
		{
			// 특정 IDLE 모션으로 전환된다 
			m_iRandomIdle = rand() % 20;
			if (m_iRandomIdle % 2 == 0)
			{
				m_wstrStateKey = L"KahoYawn";
				m_tFrame.fFrame = 0.f;
				m_tFrame.fMax = CTextureMgr::GetInstance()->GetTextureSize(
					m_pObj->GetObjKey().c_str(), m_wstrStateKey.c_str());
				m_tFrame.fRestartFrame = 0.f;
				m_tFrame.fFrameAccel = 0.4f;
			}
			else if (m_iRandomIdle % 2 == 1)
			{
				m_wstrStateKey = L"KahoKick";
				m_tFrame.fFrame = 0.f;
				m_tFrame.fMax = CTextureMgr::GetInstance()->GetTextureSize(
					m_pObj->GetObjKey().c_str(), m_wstrStateKey.c_str());
				m_tFrame.fRestartFrame = 0.f;
				m_tFrame.fFrameAccel = 0.4f;
			}

			m_fTime = 0.f;
		}
		else
		{
			m_wstrStateKey = L"KahoIdle";
			m_tFrame.fFrame = 0.f;
			m_tFrame.fMax = CTextureMgr::GetInstance()->GetTextureSize(
				m_pObj->GetObjKey().c_str(), m_wstrStateKey.c_str());
			m_tFrame.fRestartFrame = 0.f;
			m_tFrame.fFrameAccel = 1.f;
		}
	}

	return NO_EVENT;
}

void CIdleIMP::LateUpdate()
{
}

void CIdleIMP::Release()
{
}
