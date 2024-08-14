#pragma once
#include "UserInterface.h"
class CImportItem :
	public CUserInterface
{
public:
	enum TYPE { SLOT, INVEN, END };

public:
	CImportItem();
	virtual ~CImportItem();

	// CUserInterface을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;

	void RenderBack();

private:
	void Control();

private:
	TYPE eType;

	int m_iAlpha;
};

