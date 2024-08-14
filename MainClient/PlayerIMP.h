#pragma once

#include "Obj.h"
#include "Player.h"
#include "PlayerInfo.h"

class CObserver;
class CPlayerIMP
{
public:
	CPlayerIMP();
	virtual ~CPlayerIMP();

public:
	virtual void Initialize() PURE;
	virtual void LateInit();
	virtual int Update() PURE;
	virtual void LateUpdate() PURE;
	virtual void Render();
	virtual void Release() PURE;

public:
	void SetObj(CObj* pObj) { m_pObj = pObj; }
	const wstring GetStateKey() const { return m_wstrStateKey; }
	const FRAME& GetFrame() const { return m_tFrame; }
protected:
	CObj* m_pObj;
	CObserver* m_pObserver;
	FRAME m_tFrame;
	wstring m_wstrStateKey;

	bool m_bIsInit;
};

