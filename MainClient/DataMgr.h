#pragma once
class CDataMgr
{
	DECLARE_SINGLETON(CDataMgr)

private:
	CDataMgr();
	~CDataMgr();

public:
	HRESULT InsertItemData(const wstring& DataPath);
	ITEM AddPassiveItem(int id);
	ITEM AddActiveItem(int, int Count);

public:
	const ITEM& GetItemData(int index) { return m_ItemData[index]; }

private:
	vector<ITEM> m_ItemData;
};

