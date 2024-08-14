#pragma once

#include "TileObj.h"
#include "BackObj.h"
#include "InteractObj.h"

// CMySheet

class CMySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CMySheet)

public:
	CMySheet();
	CMySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CMySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CMySheet();

protected:
	DECLARE_MESSAGE_MAP()

public:
	CTileObj m_TileObj;
	CBackObj m_BackObj;
	CInteractObj m_InteractObj;
};


