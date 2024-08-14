#pragma once
#include "Obj.h"
class CObserver;
class CCath :
	public CObj
{
public:
	CCath();
	virtual ~CCath();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;

public:
	// ReStart = -1.f -> ReStart == fMax
	inline void FrameChange(wstring stateKey, float Accel, float ReStart)
	{
		if (m_wstrStateKey != stateKey)
		{
			m_wstrStateKey = stateKey;
			m_tFrame.fFrame = 0.f;
			m_tFrame.fMax = CTextureMgr::GetInstance()->GetTextureSize(m_wstrObjKey.c_str(), m_wstrStateKey.c_str());
			m_tFrame.fFrameAccel = Accel;

			if (ReStart == -1.f)
				m_tFrame.fRestartFrame = m_tFrame.fMax;
			else
				m_tFrame.fRestartFrame = ReStart;

			m_tFrame.m_bIsEnd = false;
		}
	}

private:
	wstring m_wstrStateKey;
	CObserver* m_pObserver;
};

