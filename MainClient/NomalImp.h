#pragma once
#include "Obj.h"
class CNomalImp :
	public CObj
{
	enum STANCE {PEACE, ATTACK, HURT, END };

public:
	CNomalImp();
	virtual ~CNomalImp();

public:
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;

	void RenderRect();

	void Move();

	void Falling();

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
	float m_fPosY;
	float m_fJumpTime;
	float m_fJumpPower;

	float m_fMoveTime;

	float m_fAttackTime;

	float m_fHurtTime;
	int m_iInviAlpha;
	float m_fHurtDirX;
	
	STANCE m_eCurStance;

	CObserver* m_pObserver;

	wstring m_wstrStateKey;

	bool m_bIsJumping;

	bool m_bIsThrow;

	float m_fSoundTime;
};

