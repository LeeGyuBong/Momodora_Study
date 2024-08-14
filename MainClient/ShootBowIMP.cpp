#include "stdafx.h"
#include "ShootBowIMP.h"
#include "Arrow.h"

CShootBowIMP::CShootBowIMP()
{
}


CShootBowIMP::~CShootBowIMP()
{
	Release();
}

void CShootBowIMP::Initialize()
{
	m_wstrStateKey = L"KahoBow";
	m_tFrame.fFrame = 0.f;
	m_tFrame.fMax = CTextureMgr::GetInstance()->GetTextureSize(
		m_pObj->GetObjKey().c_str(), m_wstrStateKey.c_str());
	m_tFrame.fRestartFrame = 0.f;
	m_tFrame.fFrameAccel = 2.7f;
}

void CShootBowIMP::LateInit()
{
	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CArrow>::CreateObject(), OBJECT::ARROW);

	if (dynamic_cast<CPlayer*>(m_pObj)->GetBowCharge())
	{
		CObjMgr::GetInstance()->AddObject(CAbstractFactory<CArrow>::CreateObject(10.f), OBJECT::ARROW);
		CObjMgr::GetInstance()->AddObject(CAbstractFactory<CArrow>::CreateObject(350.f), OBJECT::ARROW);
		dynamic_cast<CPlayer*>(m_pObj)->ChargeAttackDone();
	}
}

int CShootBowIMP::Update()
{
	CPlayerIMP::LateInit();

	m_tFrame.fFrame += m_tFrame.fMax * CTimeMgr::GetInstance()->GetTime() * m_tFrame.fFrameAccel;

	if (m_tFrame.fFrame > m_tFrame.fMax)
		return ANIMATION_END;

	return NO_EVENT;
}

void CShootBowIMP::LateUpdate()
{
	
}

void CShootBowIMP::Release()
{
}