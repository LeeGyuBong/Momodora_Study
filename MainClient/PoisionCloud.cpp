#include "stdafx.h"
#include "PoisionCloud.h"


CPoisionCloud::CPoisionCloud()
{
}


CPoisionCloud::~CPoisionCloud()
{
	Release();
}

void CPoisionCloud::Initialize()
{

	m_wstrObjKey = L"Effect";
	m_wstrStateKey = L"PoisonCloud";

	m_tFrame.fFrame = 0;
	m_tFrame.fMax = CTextureMgr::GetInstance()->GetTextureSize(m_wstrObjKey.c_str(), m_wstrStateKey.c_str());
	m_tFrame.fFrameAccel = 1.f;

	m_tStatus.Attack = 1;
}

void CPoisionCloud::LateInit()
{
	m_tInfo.vSize = { 24.f, 14.f, 0.f };
}

int CPoisionCloud::Update()
{
	CObj::LateInit();

	m_tFrame.fFrame += m_tFrame.fMax * CTimeMgr::GetInstance()->GetTime() * m_tFrame.fFrameAccel;

	if (m_tFrame.fFrame > m_tFrame.fMax)
		return DEAD_OBJ;

	m_tInfo.matWorld = CMath::CalculateMatrix(m_tInfo.vPos + CScrollMgr::GetScroll());

	return NO_EVENT;
}

void CPoisionCloud::LateUpdate()
{
}

void CPoisionCloud::Render()
{
	D3DXMATRIX mataaaa;
	D3DXMatrixIdentity(&mataaaa);
	if (m_tInfo.matWorld == mataaaa)
		return;

	CObj::UpdateRect();

	const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
		m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), (int)m_tFrame.fFrame);

	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (CObjMgr::GetInstance()->GetPlayer()->GetTestView())
		RenderRect();
}


void CPoisionCloud::RenderRect()
{
	D3DXVECTOR2 m_Line[5];
	m_Line[0] = { (float)m_tRect.left + CScrollMgr::GetScroll().x, (float)m_tRect.top + CScrollMgr::GetScroll().y };
	m_Line[1] = { (float)m_tRect.right + CScrollMgr::GetScroll().x, (float)m_tRect.top + CScrollMgr::GetScroll().y };
	m_Line[2] = { (float)m_tRect.right + CScrollMgr::GetScroll().x, (float)m_tRect.bottom + CScrollMgr::GetScroll().y };
	m_Line[3] = { (float)m_tRect.left + CScrollMgr::GetScroll().x, (float)m_tRect.bottom + CScrollMgr::GetScroll().y };
	m_Line[4] = { (float)m_tRect.left + CScrollMgr::GetScroll().x, (float)m_tRect.top + CScrollMgr::GetScroll().y };

	CDevice::GetInstance()->GetSprite()->End();
	CDevice::GetInstance()->GetLine()->Begin();
	CDevice::GetInstance()->GetLine()->Draw(m_Line, 5, D3DCOLOR_XRGB(0, 0, 255));
	CDevice::GetInstance()->GetLine()->End();
	CDevice::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
}


void CPoisionCloud::Release()
{
}
