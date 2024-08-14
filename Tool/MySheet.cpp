// MySheet.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MySheet.h"


// CMySheet

IMPLEMENT_DYNAMIC(CMySheet, CPropertySheet)

CMySheet::CMySheet()
{
	AddPage(&m_TileObj);
	m_TileObj.m_psp.dwFlags |= PSP_USETITLE;
	m_TileObj.m_psp.pszTitle = L"Tile";

	AddPage(&m_BackObj);
	m_BackObj.m_psp.dwFlags |= PSP_USETITLE;
	m_BackObj.m_psp.pszTitle = L"Back";

	AddPage(&m_InteractObj);
	m_InteractObj.m_psp.dwFlags |= PSP_USETITLE;
	m_InteractObj.m_psp.pszTitle = L"Interact";
	
}

CMySheet::CMySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CMySheet::CMySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

}

CMySheet::~CMySheet()
{
}


BEGIN_MESSAGE_MAP(CMySheet, CPropertySheet)
END_MESSAGE_MAP()


// CMySheet 메시지 처리기입니다.
