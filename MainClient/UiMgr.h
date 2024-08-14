#pragma once

class CUserInterface;
class CUiMgr
{
	DECLARE_SINGLETON(CUiMgr)

public:
	enum MENU_UI { MENU, EQUIP, IMPORTANT, MENU_END };
	enum  ALWAYS_UI { PLAYER, BOSS, ALWAYS_END };

private:
	CUiMgr();
	~CUiMgr();

public:
	void Add_UI(CObj * obj, ALWAYS_UI eType);
	void UiChanger(MENU_UI eUI);
	void Update();
	void LateUpdate();
	void Render();
	void Release();

	void ReleaseAlwayUI();

public:
	MENU_UI GetCurUI() { return m_eCurUI; }
	const bool& GetUIState() const { return m_bIsOnOff; }
	OBJLST& GetAlwaysUI(ALWAYS_UI eType) { return m_AlwaysUILst[eType]; }

	void UI_ON() { m_bIsOnOff = ON; }
	void UI_OFF() { m_bIsOnOff = OFF; }
	inline void UI_Swtich() 
	{
		if (m_bIsOnOff)
			m_bIsOnOff = OFF;
		else
			m_bIsOnOff = ON;
	};

private:
	CUserInterface*	m_pUI;
	MENU_UI	m_ePreUI;
	MENU_UI	m_eCurUI;

	bool	m_bIsOnOff;

	OBJLST m_AlwaysUILst[ALWAYS_END];
};
