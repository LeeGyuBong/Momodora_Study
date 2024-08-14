#include "stdafx.h"
#include "UiMgr.h"

#include "MainMenu.h"
#include "Equip.h"
#include "ImportItem.h"

IMPLEMENT_SINGLETON(CUiMgr)

CUiMgr::CUiMgr()
	: m_bIsOnOff(OFF), m_ePreUI(MENU_END), m_eCurUI(MENU_END)
{
}


CUiMgr::~CUiMgr()
{
	Release();
}

void CUiMgr::Add_UI(CObj* obj, ALWAYS_UI eType)
{
	m_AlwaysUILst[eType].push_back(obj);
}

void CUiMgr::UiChanger(MENU_UI eUI)
{
	m_eCurUI = eUI;

	if (m_ePreUI != m_eCurUI)
	{
		SafeDelete(m_pUI);

		switch (eUI)
		{
		case MENU:
			m_pUI = new CMainMenu;
			break;
		case EQUIP:
			m_pUI = new CEquip;
			break;
		case IMPORTANT:
			m_pUI = new CImportItem;
			break;
		}
		m_pUI->Initialize();

		m_ePreUI = m_eCurUI;
	}
}

void CUiMgr::Update()
{
	if (!m_bIsOnOff)
	{
		for (int i = 0; i < ALWAYS_END; ++i)
		{
			if (m_AlwaysUILst[i].empty())
				continue;

			OBJLST_ITER iter_begin = m_AlwaysUILst[i].begin();
			OBJLST_ITER iter_end = m_AlwaysUILst[i].end();
			for (; iter_begin != iter_end;)
			{
				int Event = (*iter_begin)->Update();

				if (Event == DEAD_OBJ)
				{
					SafeDelete(*iter_begin);
					iter_begin = m_AlwaysUILst[i].erase(iter_begin);
				}
				else
					++iter_begin;
			}
		}
	}
	else
		m_pUI->Update();
}

void CUiMgr::LateUpdate()
{
	if (!m_bIsOnOff)
	{
		for (auto& UILst : m_AlwaysUILst)
			for (auto*& pUI : UILst)
				pUI->LateUpdate();
	}
	else
		m_pUI->LateUpdate();
}

void CUiMgr::Render()
{
	if (!m_bIsOnOff)
	{
		for (auto& UILst : m_AlwaysUILst)
			for (auto*& pUI : UILst)
				pUI->Render();
	}
	else
		m_pUI->Render();
}

void CUiMgr::Release()
{
	SafeDelete(m_pUI);
	
	for (auto& UILst : m_AlwaysUILst)
	{
		for_each(UILst.begin(), UILst.end(), SafeDelete<CObj*>);
		UILst.clear();
	}
}

void CUiMgr::ReleaseAlwayUI()
{
	for (auto& UILst : m_AlwaysUILst)
	{
		for_each(UILst.begin(), UILst.end(), SafeDelete<CObj*>);
		UILst.clear();
	}
}
