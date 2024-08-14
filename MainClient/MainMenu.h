#pragma once
#include "UserInterface.h"
class CMainMenu :
	public CUserInterface
{
public:
	CMainMenu();
	virtual ~CMainMenu();

	// CUserInterface을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;

	void RenderBack();
	void RenderMenu();
	void RenderText();

private:
	void Control();

private:
	wstring m_wstrStateKey;
	static int m_iMenuIndex;

	vector<wstring> m_vMenuText;
	int m_iTextAlpha;
};

