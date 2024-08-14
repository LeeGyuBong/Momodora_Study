#include "stdafx.h"
#include "Cath.h"
#include "PlayerInfo.h"

CCath::CCath()
{
}


CCath::~CCath()
{
	Release();
}

void CCath::Initialize()
{
	m_tInfo.vPos = { 1000.f, 500.f, 0.f };

	m_tInfo.vDir = { 1.f, 0.f, 0.f };

	m_tInfo.vSize = { 96.f, 96.f, 0.f };
	m_vModify = { 0.f, 0.f };

	m_wstrObjKey = L"Cath";
	m_wstrStateKey = L"CathIdle";

	m_tFrame.fFrame = 0.f;
	m_tFrame.fMax = CTextureMgr::GetInstance()->GetTextureSize(m_wstrObjKey.c_str(), m_wstrStateKey.c_str());
	m_tFrame.fFrameAccel = 0.8f;
	m_tFrame.fRestartFrame = 0.f;
	m_tFrame.m_bIsEnd = false;

	m_pObserver = new CPlayerInfo;
	CDataSubject::GetInstance()->Subscribe(m_pObserver);
}

void CCath::LateInit()
{
}

int CCath::Update()
{
	CObj::LateInit();

	m_tInfo.vPos.y += m_fFallSpeed;
	// 지속적으로 중력을 받음
	m_fFallSpeed += (GRAVITY * 0.3f) * CTimeMgr::GetInstance()->GetTime();

	if (CTextMgr::GetInstance()->GetIsTalk())
	{
		if (dynamic_cast<CPlayerInfo*>(m_pObserver)->GetInfo().vPos.x > m_tInfo.vPos.x)
			m_tInfo.vDir.x = 1.f;
		else
			m_tInfo.vDir.x = -1.f;

		if (CTextMgr::GetInstance()->GetTalkIndex() == 1)
			FrameChange(L"CathPoint", 1.f, -1.f);
		else if(CTextMgr::GetInstance()->GetTalkIndex() == 3)
			FrameChange(L"CathHandOnHip", 1.f, -1.f);
		else if (CTextMgr::GetInstance()->GetTalkIndex() == 4)
		{
			if(m_wstrStateKey != L"CathIdle")
				FrameChange(L"CathToIdle", 5.f, -1.f);
			if(m_tFrame.m_bIsEnd)
				FrameChange(L"CathIdle", 0.8f, 0.f);
		}
		else if(CTextMgr::GetInstance()->GetTalkIndex() == 5)
			FrameChange(L"CathIdle", 0.8f, 0.f);
	}

	m_tInfo.matWorld = CMath::CalculateMatrix(m_tInfo.vPos + CScrollMgr::GetScroll(), D3DXVECTOR3(m_tInfo.vDir.x, 1.f, 0.f));

	m_tFrame.fFrame += m_tFrame.fMax * CTimeMgr::GetInstance()->GetTime() * m_tFrame.fFrameAccel;

	if (m_tFrame.fFrame > m_tFrame.fMax)
	{
		m_tFrame.fFrame = m_tFrame.fRestartFrame;
		m_tFrame.m_bIsEnd = true;
	}
	
	return NO_EVENT;
}

void CCath::LateUpdate()
{
}

void CCath::Render()
{
	CObj::UpdateRect();

	const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
		m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), (int)m_tFrame.fFrame);

	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	CTextMgr::GetInstance()->TalkText(m_wstrObjKey.c_str(), D3DXVECTOR3(m_tInfo.vPos.x - 100.f + CScrollMgr::GetScroll().x, m_tInfo.vPos.y - 165.f + CScrollMgr::GetScroll().y, 0.f));

	if (m_bIsInteractObj && !CTextMgr::GetInstance()->GetIsTalk())
	{
		pTexInfo = CTextureMgr::GetInstance()->GetTexture(
			L"UI", L"Talk", 10);

		NULL_CHECK(pTexInfo);

		fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		D3DXMATRIX matWorld;
		matWorld = CMath::CalculateMatrix(D3DXVECTOR3(m_tInfo.vPos.x + CScrollMgr::GetScroll().x, m_tInfo.vPos.y - 60.f + CScrollMgr::GetScroll().y, 0.f));
		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CCath::Release()
{
	SafeDeleteObserver<CDataSubject>(m_pObserver);
}
