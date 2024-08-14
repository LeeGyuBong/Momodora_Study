#include "stdafx.h"
#include "ItemBox.h"

#include "Player.h"
#include "PlayerInfo.h"


CItemBox::CItemBox()
{
}


CItemBox::~CItemBox()
{
	Release();
}

void CItemBox::Initialize()
{
	m_tInfo.vPos = { 63.f, 71.f, 0.f };
	m_vMoneyPos = { 119.f, 87.f, 0.f };

	m_wstrObjKey = L"UI";

	m_pObserver = new CPlayerInfo;
	CDataSubject::GetInstance()->Subscribe(m_pObserver);
}

void CItemBox::LateInit()
{
	m_tInfo.matWorld = CMath::CalculateMatrix(m_tInfo.vPos);
	m_matMoney = CMath::CalculateMatrix(m_vMoneyPos);
}

int CItemBox::Update()
{
	CObj::LateInit();

	if (m_bIsDead)
		return DEAD_OBJ;

	m_pItem = dynamic_cast<CPlayer*>(CObjMgr::GetInstance()->GetPlayer())->GetCurItem();

	return NO_EVENT;
}

void CItemBox::LateUpdate()
{
	if (dynamic_cast<CPlayerInfo*>(m_pObserver)->GetStatus().HP <= 0)
		m_bIsDead = true;
}

void CItemBox::Render()
{
	const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
		m_wstrObjKey.c_str(), L"InventoryBox", 0);

	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	// Item
	if (m_pItem != nullptr)
	{
		pTexInfo = CTextureMgr::GetInstance()->GetTexture(
			m_wstrObjKey.c_str(), L"Item", m_pItem->ID - 1);

		NULL_CHECK(pTexInfo);

		fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
		
		// Item Count Text
		TCHAR szCount[MIN_STR] = L"";
		swprintf_s(szCount, L"%d", m_pItem->count);

		D3DXMATRIX matTrans;
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + 5.f, m_tInfo.vPos.y + 5.f, 0.f);

		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
		CDevice::GetInstance()->GetBigFont()->DrawTextW(CDevice::GetInstance()->GetSprite(),
			szCount, lstrlen(szCount), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	

	////////////////////////////////////////////////////////////////////////////////

	pTexInfo = CTextureMgr::GetInstance()->GetTexture(
		m_wstrObjKey.c_str(), L"InventoryBox", 1);

	NULL_CHECK(pTexInfo);

	fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_matMoney);
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	/////////////////////////////////////////////////////////////////////////////

	RenderMoneyText();
}

void CItemBox::Release()
{
	SafeDeleteObserver<CDataSubject>(m_pObserver);
}

void CItemBox::RenderMoneyText()
{
	TCHAR szMoney[MIN_STR] = L"";

	int Money = dynamic_cast<CPlayerInfo*>(m_pObserver)->GetMoney();
	int Hundred = 0, Ten = 0, One = 0;

	Hundred = Money / 100;
	Ten = (Money - (Hundred * 100)) / 10;
	One = Money % 10;

	swprintf_s(szMoney, L"%d%d%d", Hundred, Ten, One);

	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, m_vMoneyPos.x + 30.f, m_vMoneyPos.y - 15.f, 0.f);

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDevice::GetInstance()->GetBigFont()->DrawTextW(CDevice::GetInstance()->GetSprite(),
		szMoney, lstrlen(szMoney), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
}
