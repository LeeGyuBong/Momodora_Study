#pragma once
#include "Obj.h"
class CShieldImp :
	public CObj
{
	enum STANCE { PEACE, ATTACK, HURT, END };

public:
	CShieldImp();
	virtual ~CShieldImp();

public:
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;

	void RenderRect();
	void RenderAttackRect();
	void Falling();
	void Move();

public:
	void FrameChange(wstring stateKey, float Accel, float ReStart)
	{
		if (m_wstrStateKey != stateKey)
		{
			m_wstrStateKey = stateKey;
			m_tFrame.fMax = CTextureMgr::GetInstance()->GetTextureSize(m_wstrObjKey.c_str(), m_wstrStateKey.c_str());
			m_tFrame.fFrameAccel = Accel;
			m_tFrame.fRestartFrame = ReStart;
		}
	}

private:
	CObserver * m_pObserver;

	wstring m_wstrStateKey;

	STANCE m_eCurStance;


	float m_fMoveTime;

	float m_fAttackTime;
	float m_fHurtTime;
	float m_fHurtDirX;

	bool m_bIsThrow;
	bool m_bIsMove;

	float m_fSoundTime;
};

