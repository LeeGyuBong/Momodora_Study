#pragma once

class CObj;
class CEffectIMP
{
public:
	CEffectIMP();
	virtual ~CEffectIMP();

public:
	virtual void Initialize() PURE;
	virtual void LateInit();
	virtual int Update() PURE;
	virtual void LateUpdate() PURE;
	virtual void Render() PURE;
	virtual void Release() PURE;

public:
	void SetObj(CObj* pObj) { m_pObj = pObj; }
	void SetStateKey(wstring wstrStateKey) { m_wstrStateKey = wstrStateKey; }

	wstring& GetStateKey() { return m_wstrStateKey; }
	void SetInfo(INFO& info) { m_tInfo = info; }

	void SetAlpha(int alpha) { m_iAlpha = alpha; }
	
protected:
	CObj* m_pObj;
	wstring m_wstrStateKey;

	
	INFO m_tInfo;

	bool m_bIsInit;

	int m_iAlpha;
};

