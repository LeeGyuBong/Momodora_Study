#include "stdafx.h"
#include "Obj.h"


CObj::CObj()
	: m_bIsInit(false), m_bIsDead(false), m_fDegree(0.f), m_fSpeed(0.f),
	m_bIsFloorColli(false), m_fFallSpeed(0.f), m_bIsHit(false), m_eColliObjType(OBJECT::END),
	m_bIsView(false), m_bIsColliRectView(false), m_bIsInteractObj(false)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tStatus, sizeof(STATUS));
	D3DXMatrixIdentity(&m_tInfo.matWorld);
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_tViewRect, sizeof(RECT));
	m_vModify = { 0.f, 0.f };
}


CObj::~CObj()
{
}

void CObj::LateInit()
{
	if (!m_bIsInit)
	{
		this->LateInit();
		m_bIsInit = true;
	}
}

void CObj::UpdateRect()
{
	m_tRect.left = { LONG(m_tInfo.vPos.x - m_tInfo.vSize.x * 0.5f + m_vModify.x) };
	m_tRect.top = { LONG(m_tInfo.vPos.y - m_tInfo.vSize.y * 0.5f + m_vModify.y) };
	m_tRect.right = { LONG(m_tInfo.vPos.x + m_tInfo.vSize.x * 0.5f + m_vModify.x) };
	m_tRect.bottom = { LONG(m_tInfo.vPos.y + m_tInfo.vSize.y * 0.5f + m_vModify.y) };
}

void CObj::UpdateViewRect(float fSize)
{
	m_tViewRect.left = { LONG(m_tInfo.vPos.x - fSize * 0.5f) };
	m_tViewRect.top = { LONG(m_tInfo.vPos.y - fSize * 0.5f) };
	m_tViewRect.right = { LONG(m_tInfo.vPos.x + fSize * 0.5) };
	m_tViewRect.bottom = { LONG(m_tInfo.vPos.y + fSize * 0.5f) };
}

void CObj::UpdateAttackRect()
{
	for (size_t i = 0; i < m_vAttackSize.size(); ++i)
	{
		RECT Rc = {};
		Rc.left = { LONG(m_tInfo.vPos.x - m_vAttackSize[i].x * 0.5f + (m_vAttackModify[i].x * m_tInfo.vDir.x)) };
		Rc.top = { LONG(m_tInfo.vPos.y - m_vAttackSize[i].y * 0.5f + m_vAttackModify[i].y) };
		Rc.right = { LONG(m_tInfo.vPos.x + m_vAttackSize[i].x * 0.5f + (m_vAttackModify[i].x * m_tInfo.vDir.x)) };
		Rc.bottom = { LONG(m_tInfo.vPos.y + m_vAttackSize[i].y * 0.5f + m_vAttackModify[i].y) };

		m_vAttackRect.push_back(Rc);
	}
}
