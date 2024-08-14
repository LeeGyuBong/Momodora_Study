#include "stdafx.h"
#include "CrouchBowIMP.h"
#include "Arrow.h"


CCrouchBowIMP::CCrouchBowIMP()
{
}


CCrouchBowIMP::~CCrouchBowIMP()
{
	Release();
}

void CCrouchBowIMP::Initialize()
{
	m_wstrStateKey = L"KahoCrouchBow";
	m_tFrame.fFrame = 0.f;
	m_tFrame.fMax = CTextureMgr::GetInstance()->GetTextureSize(
		m_pObj->GetObjKey().c_str(), m_wstrStateKey.c_str());
	m_tFrame.fRestartFrame = 0.f;
	m_tFrame.fFrameAccel = 2.7f;
}

void CCrouchBowIMP::LateInit()
{
	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CArrow>::CreateObject(D3DXVECTOR3(0.f, 20.f, 0.f)), OBJECT::ARROW);

	if (dynamic_cast<CPlayer*>(m_pObj)->GetBowCharge())
	{
		CObjMgr::GetInstance()->AddObject(CAbstractFactory<CArrow>::CreateObject(D3DXVECTOR3(0.f, 20.f, 0.f), 10.f), OBJECT::ARROW);
		CObjMgr::GetInstance()->AddObject(CAbstractFactory<CArrow>::CreateObject(D3DXVECTOR3(0.f, 20.f, 0.f), 350.f), OBJECT::ARROW);
		dynamic_cast<CPlayer*>(m_pObj)->ChargeAttackDone();
	}
}

int CCrouchBowIMP::Update()
{
	CPlayerIMP::LateInit();

	m_tFrame.fFrame += m_tFrame.fMax * CTimeMgr::GetInstance()->GetTime() * m_tFrame.fFrameAccel;

	if (m_tFrame.fFrame > m_tFrame.fMax)
		return ANIMATION_END;

	return NO_EVENT;
}

void CCrouchBowIMP::LateUpdate()
{
}

void CCrouchBowIMP::Release()
{
}
