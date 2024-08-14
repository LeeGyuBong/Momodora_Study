#include "stdafx.h"
#include "Equip.h"

#include "Slot.h"

#include "Player.h"


CEquip::CEquip()
	: m_eType(SLOT), m_iAlpha(0), m_iCurIndex(0), m_eItemType(ACTIVE), m_iInvenIndex(0), m_iInvenAlpha(255),
	m_fAlphaTime(0.f), m_iCalcul(0)
{
}


CEquip::~CEquip()
{
	Release();
}

void CEquip::Initialize()
{
	m_tInfo.vPos = { CLIENTWINCX * 0.5f, CLIENTWINCY * 0.5f, 0.f };

	m_wstrObjKey = L"UI";
	m_wstrStateKey = L"InventoryBox";

	pObj = CObjMgr::GetInstance()->GetPlayer();
}

void CEquip::LateInit()
{
	InsertSlot();
	dynamic_cast<CSlot*>(m_vecSlot[m_iCurIndex])->SetCurSlot(true);
}

int CEquip::Update()
{
	CObj::LateInit();

	if (m_iAlpha < 255)
		m_iAlpha += 3;
	if (m_iAlpha > 255)
		m_iAlpha = 255;

	if (m_eType == INVEN)
	{
		m_fAlphaTime += CTimeMgr::GetInstance()->GetTime();

		if (m_iInvenAlpha == 255)
			m_iCalcul = -1;
		else if (m_iInvenAlpha == 25)
			m_iCalcul = 1;

		if (m_fAlphaTime > 0.006f)
		{
			m_iInvenAlpha += m_iCalcul;
			m_fAlphaTime = 0.f;
		}
	}

	if (m_iCurIndex > 2)
	{
		m_eItemType = PASSIVE;
		m_CurLst = dynamic_cast<CPlayer*>(pObj)->GetPassiveItemLst();
	}
	else if (m_iCurIndex <= 2)
	{
		m_eItemType = ACTIVE;
		m_CurLst = dynamic_cast<CPlayer*>(pObj)->GetActiveItemLst();
	}
	
	for (int i = 0; i< (int)m_vecSlot.size(); ++i)
	{
		m_vecSlot[i]->Update();

		if (m_eType == SLOT)
		{
			if (m_iCurIndex == i)
				dynamic_cast<CSlot*>(m_vecSlot[i])->SetCurSlot(true);
			else
				dynamic_cast<CSlot*>(m_vecSlot[i])->SetCurSlot(false);
		}
		else if(m_eType == INVEN)
			dynamic_cast<CSlot*>(m_vecSlot[i])->SetCurSlot(false);
	}

	m_tInfo.matWorld = CMath::CalculateMatrix(m_tInfo.vPos);

	return NO_EVENT;
}

void CEquip::LateUpdate()
{
	for (auto*& pSlot : m_vecSlot)
		pSlot->LateUpdate();

	Control();
}

void CEquip::Render()
{
	RenderBack();
	RenderBox();
	
	for (auto*& pSlot : m_vecSlot)
		pSlot->Render();
	
	RenderText();
	RenderItemList();
}

void CEquip::RenderBack()
{
	const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"BackGround", L"Effect", 0);
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
}

void CEquip::RenderBox()
{
	D3DXMATRIX matWorld;

	int Max = 12;
	for (int i = 0; i < Max; ++i)
	{
		int frame;

		if (i == 0)
			frame = 2;
		else if (i == Max - 1)
			frame = 4;
		else
			frame = 3;

		const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), frame);
		NULL_CHECK(pTexInfo);

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		matWorld = CMath::CalculateMatrix(D3DXVECTOR3(70.f + pTexInfo->tImgInfo.Width * i, 80.f, 0.f));
		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
			nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
	}
	for (int i = 0; i < Max; ++i)
	{
		int frame;

		if (i == 0)
			frame = 2;
		else if (i == Max - 1)
			frame = 4;
		else
			frame = 3;

		const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), frame);
		NULL_CHECK(pTexInfo);

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		matWorld = CMath::CalculateMatrix(D3DXVECTOR3(70.f + pTexInfo->tImgInfo.Width * i, 230.f, 0.f));
		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
			nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
	}
	Max = 10;
	for (int i = 0; i < Max; ++i)
	{
		int frame;

		if (i == 0)
			frame = 2;
		else if (i == Max - 1)
			frame = 4;
		else
			frame = 3;

		const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), frame);
		NULL_CHECK(pTexInfo);

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		matWorld = CMath::CalculateMatrix(D3DXVECTOR3(500.f + pTexInfo->tImgInfo.Width * i, 80.f, 0.f));
		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
			nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
	}
}

void CEquip::InsertSlot()
{
	for (int i = 0; i < 3; ++i)
		m_vecSlot.push_back(CAbstractFactory<CSlot>::CreateObject(
			D3DXVECTOR3(93.f + ((67.f + 5.f) * i), 155.f, 0.f)));

	for (int i = 0; i < 2; ++i)
		m_vecSlot.push_back(CAbstractFactory<CSlot>::CreateObject(
			D3DXVECTOR3(93.f + ((67.f + 5.f) * i), 305.f, 0.f)));

	dynamic_cast<CSlot*>(m_vecSlot[0])->SetItem(dynamic_cast<CPlayer*>(pObj)->GetActiveSlotItem(0));
	dynamic_cast<CSlot*>(m_vecSlot[1])->SetItem(dynamic_cast<CPlayer*>(pObj)->GetActiveSlotItem(1));
	dynamic_cast<CSlot*>(m_vecSlot[2])->SetItem(dynamic_cast<CPlayer*>(pObj)->GetActiveSlotItem(2));
	dynamic_cast<CSlot*>(m_vecSlot[3])->SetItem(dynamic_cast<CPlayer*>(pObj)->GetPassiveSlotItem(0));
	dynamic_cast<CSlot*>(m_vecSlot[4])->SetItem(dynamic_cast<CPlayer*>(pObj)->GetPassiveSlotItem(1));
}

void CEquip::RenderText()
{
	D3DXMATRIX matWorld;

	matWorld = CMath::CalculateMatrix(D3DXVECTOR3(110.f, 65.f, 0.f));
	CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
	CDevice::GetInstance()->GetBigFont()->DrawTextW(CDevice::GetInstance()->GetSprite(),
		L"활성 슬롯", lstrlen(L"활성 슬롯"), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	matWorld = CMath::CalculateMatrix(D3DXVECTOR3(110.f, 215.f, 0.f));
	CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
	CDevice::GetInstance()->GetBigFont()->DrawTextW(CDevice::GetInstance()->GetSprite(),
		L"지속 슬롯", lstrlen(L"지속 슬롯"), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	matWorld = CMath::CalculateMatrix(D3DXVECTOR3(535.f, 65.f, 0.f));
	CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
	CDevice::GetInstance()->GetBigFont()->DrawTextW(CDevice::GetInstance()->GetSprite(),
		L"인벤토리", lstrlen(L"인벤토리"), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));


	wstring strText[2];
	strText[0] = L"A: ";
	strText[1] = L"S: ";
	for (int i = 0; i < 2; ++i)
	{
		matWorld = CMath::CalculateMatrix(D3DXVECTOR3(40.f + 150.f * i, 565.f, 0.f));
		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
		CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite(),
			strText[i].c_str(), lstrlen(strText[i].c_str()), nullptr, 0, D3DCOLOR_ARGB(255, 140, 140, 140));
	}
	strText[0] = L"확인";
	strText[1] = L"취소";
	for (int i = 0; i < 2; ++i)
	{
		matWorld = CMath::CalculateMatrix(D3DXVECTOR3(80.f + 150.f * i, 565.f, 0.f));
		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
		CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite(),
			strText[i].c_str(), lstrlen(strText[i].c_str()), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (!m_vecSlot.empty())
	{
		if (dynamic_cast<CSlot*>(m_vecSlot[m_iCurIndex])->GetSlotItem() != nullptr &&
			dynamic_cast<CSlot*>(m_vecSlot[m_iCurIndex])->GetSlotItem()->ID != None)
		{
			ITEM* pItme = dynamic_cast<CSlot*>(m_vecSlot[m_iCurIndex])->GetSlotItem();

			RECT rc = { 0, 0, CLIENTWINCX - 100, CLIENTWINCY };
			matWorld = CMath::CalculateMatrix(D3DXVECTOR3(50.f, 400.f, 0.f));
			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
			CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite(),
				pItme->Name.c_str(), lstrlen(pItme->Name.c_str()), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 0));

			matWorld = CMath::CalculateMatrix(D3DXVECTOR3(50.f, 430.f, 0.f));
			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
			CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite(),
				pItme->Effect.c_str(), lstrlen(pItme->Effect.c_str()), &rc, DT_WORDBREAK, D3DCOLOR_ARGB(255, 255, 255, 255));

			matWorld = CMath::CalculateMatrix(D3DXVECTOR3(50.f, 480.f, 0.f));
			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
			CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite(),
				pItme->Detail.c_str(), lstrlen(pItme->Detail.c_str()), 
				&rc, DT_WORDBREAK, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
}

void CEquip::RenderItemList()
{
	D3DXMATRIX matWorld;
	int i = 0;

	if (m_eType == INVEN)
	{
		const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), 6);
		NULL_CHECK(pTexInfo);

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		matWorld = CMath::CalculateMatrix(D3DXVECTOR3(595.f, 140.f + 35.f * m_iInvenIndex, 0.f));

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
			nullptr, D3DCOLOR_ARGB(m_iInvenAlpha, 255, 255, 255));
	}

	for (auto*& pItem : m_CurLst)
	{
		matWorld = CMath::CalculateMatrix(D3DXVECTOR3(500.f, 130.f + 35.f * i, 0.f));
		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
		CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite(),
			pItem->Name.c_str(), lstrlen(pItem->Name.c_str()), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		++i;
	}
}

void CEquip::Release()
{
	for_each(m_vecSlot.begin(), m_vecSlot.end(), SafeDelete<CObj*>);
}

void CEquip::Control()
{
	if (CKeyMgr::GetInstance()->KeyDown(KEY_S))
	{
		if (m_eType == SLOT)
		{
			CUiMgr::GetInstance()->UiChanger(CUiMgr::MENU);
			CSoundMgr::GetInstance()->PlaySound(L"sndUIcancel1.wav", CSoundMgr::EFFECT);
			return;
		}
		else if (m_eType == INVEN)
			m_eType = SLOT;
		CSoundMgr::GetInstance()->PlaySound(L"sndUIcancel1.wav", CSoundMgr::EFFECT);
	}

	if (CKeyMgr::GetInstance()->KeyDown(KEY_A))
	{
		if (m_eType == SLOT)
		{
			m_eType = INVEN;
			m_iInvenIndex = 0;
		}
		else if (m_eType == INVEN)
		{
			// 슬롯이 비었을 경우
			if (dynamic_cast<CSlot*>(m_vecSlot[m_iCurIndex])->GetSlotItem() == nullptr)
			{
				// 현재 선택한 인벤토리 아이템이 None이 아니면 슬롯에 아이템 추가
				if (m_CurLst[m_iInvenIndex]->ID != None)
				{
					// 슬롯 배열에 선택한 아이템 추가
					if (m_eItemType == ACTIVE)
						dynamic_cast<CPlayer*>(pObj)->GetActiveSlotItem(m_iCurIndex) = m_CurLst[m_iInvenIndex];
					else if (m_eItemType == PASSIVE)
						dynamic_cast<CPlayer*>(pObj)->GetPassiveSlotItem(m_iCurIndex - 3) = m_CurLst[m_iInvenIndex];

					// 슬롯에 선택한 아이템 추가
					dynamic_cast<CSlot*>(m_vecSlot[m_iCurIndex])->SetItem(m_CurLst[m_iInvenIndex]);

					vector<ITEM*>::iterator iter_being;
					vector<ITEM*>::iterator iter_end;
					// 플레이어 아이템 리스트에서는 
					if (m_eItemType == ACTIVE)
					{
						iter_being = dynamic_cast<CPlayer*>(pObj)->GetActiveItemLst().begin();
						iter_end = dynamic_cast<CPlayer*>(pObj)->GetActiveItemLst().end();
					}
					else if (m_eItemType == PASSIVE)
					{
						iter_being = dynamic_cast<CPlayer*>(pObj)->GetPassiveItemLst().begin();
						iter_end = dynamic_cast<CPlayer*>(pObj)->GetPassiveItemLst().end();
					}

					for (; iter_being != iter_end;)
					{
						if ((*iter_being)->ID == m_CurLst[m_iInvenIndex]->ID)
						{
							if (m_eItemType == ACTIVE)
							{
								iter_being = dynamic_cast<CPlayer*>(pObj)->GetActiveItemLst().erase(iter_being);
								m_CurLst = dynamic_cast<CPlayer*>(pObj)->GetActiveItemLst();
							}
							else if (m_eItemType == PASSIVE)
							{
								iter_being = dynamic_cast<CPlayer*>(pObj)->GetPassiveItemLst().erase(iter_being);
								m_CurLst = dynamic_cast<CPlayer*>(pObj)->GetPassiveItemLst();
							}
							--m_iInvenIndex;
							m_eType = SLOT;
							break;
						}
						else
							++iter_being;
					}
				}
			}
			// 슬롯에 아이템이 있을 경우 
			else
			{
				vector<ITEM*>::iterator iter_being;
				vector<ITEM*>::iterator iter_end;
				// 아이템과 아이템을 교체해야하는 경우
				if (m_eItemType == ACTIVE)
				{
					// 슬롯의 아이템을 임시로 빼고
					ITEM* tempItem = dynamic_cast<CPlayer*>(pObj)->GetActiveSlotItem(m_iCurIndex);

					if (tempItem->ID != None)
					{
						// 플레이어의 인벤토리에 슬롯 배열 아이템 추가
						dynamic_cast<CPlayer*>(pObj)->GetActiveItemLst().push_back(dynamic_cast<CPlayer*>(pObj)->GetActiveSlotItem(m_iCurIndex));
						dynamic_cast<CSlot*>(m_vecSlot[m_iCurIndex])->SetItem(nullptr);

						// 현재 선택한 인벤토리의 아이템이 None이 아니라면
						if (m_CurLst[m_iInvenIndex]->ID != None)
						{
							// 슬롯 배열에 넣을 아이템은 플레이어 인벤토리에서 해제
							iter_being = dynamic_cast<CPlayer*>(pObj)->GetActiveItemLst().begin();
							iter_end = dynamic_cast<CPlayer*>(pObj)->GetActiveItemLst().end();

							for (; iter_being != iter_end;)
							{
								if ((*iter_being)->ID == m_CurLst[m_iInvenIndex]->ID)
								{
									ITEM* Item = *iter_being;

									dynamic_cast<CPlayer*>(pObj)->GetActiveSlotItem(m_iCurIndex) = m_CurLst[m_iInvenIndex];
									dynamic_cast<CSlot*>(m_vecSlot[m_iCurIndex])->SetItem(dynamic_cast<CPlayer*>(pObj)->GetActiveSlotItem(m_iCurIndex));

									iter_being = dynamic_cast<CPlayer*>(pObj)->GetActiveItemLst().erase(iter_being);
									m_CurLst = dynamic_cast<CPlayer*>(pObj)->GetActiveItemLst();
									break;
								}
								else
									++iter_being;
							}
						}
						else
							dynamic_cast<CPlayer*>(pObj)->GetActiveSlotItem(m_iCurIndex) = nullptr;
					}
					
					m_CurLst = dynamic_cast<CPlayer*>(pObj)->GetActiveItemLst();
				}
				else if (m_eItemType == PASSIVE)
				{
					// 슬롯의 아이템을 임시로 빼고
					ITEM tempItem = *dynamic_cast<CSlot*>(m_vecSlot[m_iCurIndex])->GetSlotItem();

					if (tempItem.ID != None)
					{
						// 플레이어의 인벤토리에 슬롯 아이템 동적 할당 추가
						dynamic_cast<CPlayer*>(pObj)->GetPassiveItemLst().push_back(dynamic_cast<CPlayer*>(pObj)->GetPassiveSlotItem(m_iCurIndex - 3));
						dynamic_cast<CSlot*>(m_vecSlot[m_iCurIndex])->SetItem(nullptr);
				
						if (m_CurLst[m_iInvenIndex]->ID != None)
						{
							// 슬롯에 넣을 아이템은 플레이어 인벤토리에서 동적 해제
							iter_being = dynamic_cast<CPlayer*>(pObj)->GetPassiveItemLst().begin();
							iter_end = dynamic_cast<CPlayer*>(pObj)->GetPassiveItemLst().end();

							// 현재 선택한 인벤토리의 아이템이 None이 아니라면
							for (; iter_being != iter_end;)
							{
								if ((*iter_being)->ID == m_CurLst[m_iInvenIndex]->ID)
								{
									ITEM* Item = *iter_being;

									dynamic_cast<CPlayer*>(pObj)->GetPassiveSlotItem(m_iCurIndex - 3) = m_CurLst[m_iInvenIndex];
									dynamic_cast<CSlot*>(m_vecSlot[m_iCurIndex])->SetItem(dynamic_cast<CPlayer*>(pObj)->GetPassiveSlotItem(m_iCurIndex - 3));

									iter_being = dynamic_cast<CPlayer*>(pObj)->GetPassiveItemLst().erase(iter_being);
									m_CurLst = dynamic_cast<CPlayer*>(pObj)->GetPassiveItemLst();
									break;
								}
								else
									++iter_being;
							}
						}
						else
							dynamic_cast<CPlayer*>(pObj)->GetPassiveSlotItem(m_iCurIndex - 3) = nullptr;
					}
					m_CurLst = dynamic_cast<CPlayer*>(pObj)->GetPassiveItemLst();
				}
				m_eType = SLOT;

			}
		}
		CSoundMgr::GetInstance()->PlaySound(L"sndUIok1.wav", CSoundMgr::EFFECT);
	}

	if (CKeyMgr::GetInstance()->KeyDown(KEY_LEFT))
	{
		if (m_eType == SLOT)
		{
			if (m_iCurIndex <= 0)
				m_iCurIndex = 4;
			else
				--m_iCurIndex;
		}
	}
	else if (CKeyMgr::GetInstance()->KeyDown(KEY_RIGHT))
	{
		if (m_eType == SLOT)
		{
			if (m_iCurIndex >= 4)
				m_iCurIndex = 0;
			else
				++m_iCurIndex;
		}
	}
	else if (CKeyMgr::GetInstance()->KeyDown(KEY_UP))
	{
		if (m_eType == SLOT)
		{
			if (m_iCurIndex < 2)
				m_iCurIndex += 3;
			else if (m_iCurIndex > 2)
				m_iCurIndex -= 3;
			else if (m_iCurIndex == 2)
				m_iCurIndex += 2;
		}
		else if (m_eType == INVEN)
		{
			if (m_iInvenIndex <= 0)
				m_iInvenIndex = m_CurLst.size() - 1;
			else
				--m_iInvenIndex;
		}
	}
	else if (CKeyMgr::GetInstance()->KeyDown(KEY_DOWN))
	{
		if (m_eType == SLOT)
		{
			if (m_iCurIndex < 2)
				m_iCurIndex += 3;
			else if (m_iCurIndex > 2)
				m_iCurIndex -= 3;
			else if (m_iCurIndex == 2)
				m_iCurIndex += 2;
		}
		else if (m_eType == INVEN)
		{
			if (m_iInvenIndex >= (int)m_CurLst.size() - 1)
				m_iInvenIndex = 0;
			else
				++m_iInvenIndex;
		}
	}
}
