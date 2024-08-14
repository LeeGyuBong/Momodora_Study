#include "stdafx.h"
#include "DataMgr.h"

IMPLEMENT_SINGLETON(CDataMgr)

CDataMgr::CDataMgr()
{
}


CDataMgr::~CDataMgr()
{
	m_ItemData.clear();
	m_ItemData.shrink_to_fit();
}

HRESULT CDataMgr::InsertItemData(const wstring& DataPath)
{
	wifstream fin;

	fin.open(DataPath);

	fin.imbue(std::locale("kor"));

	if (fin.fail())
	{
		ERR_MSG(L"TextPath Load Failed!!");
		return E_FAIL;
	}

	TCHAR szID[MAX_STR] = L"";
	TCHAR szName[MAX_STR] = L"";
	TCHAR szType[MAX_STR] = L"";
	TCHAR szEffect[MAX_STR] = L"";
	TCHAR szDetail[MAX_STR] = L"";

	while (true)
	{
		fin.getline(szID, MAX_STR, '|');
		fin.getline(szName, MAX_STR, '|');
		fin.getline(szType, MAX_STR, '|');
		fin.getline(szEffect, MAX_STR, '|');
		fin.getline(szDetail, MAX_STR);
		
		if (fin.eof())
			break;

		ITEM Data;
		Data.ID = _ttoi(szID);
		Data.Name = szName;
		
		if (lstrcmp(szType, L"ACTIVE"))
			Data.eType = ITEMTYPE::ACTIVE;
		else if(lstrcmp(szType, L"PASSIVE"))
			Data.eType = ITEMTYPE::PASSIVE;
		else if(lstrcmp(szType, L"IMPORTANT"))
			Data.eType = ITEMTYPE::IMPORTANT;

		Data.Effect = szEffect;
		Data.Detail = szDetail;

		m_ItemData.push_back(Data);
	}
	fin.close();

	return S_OK;
}

ITEM CDataMgr::AddPassiveItem(int id)
{
	ITEM item;

	item.ID = id;
	item.Name = m_ItemData[id - 1].Name;
	item.eType = m_ItemData[id - 1].eType;
	item.Effect = m_ItemData[id - 1].Effect;
	item.Detail = m_ItemData[id - 1].Detail;
	
	return item;
}

ITEM CDataMgr::AddActiveItem(int id, int Count)
{
	ITEM item;

	item.ID = id;
	item.Name = m_ItemData[id - 1].Name;
	item.eType = m_ItemData[id - 1].eType;
	item.Effect = m_ItemData[id - 1].Effect;
	item.Detail = m_ItemData[id - 1].Detail;
	item.count = Count;

	return item;
}