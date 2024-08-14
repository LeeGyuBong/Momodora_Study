#pragma once
#include "Obj.h"
class CObserver;
class CFennel :
	public CObj
{
	enum STANCE { PEACE, IDLE, ATTACK, HURT, DEAD, END };
	enum ATTACKSTANCE { NOMAL_ATTACK, DASH_ATTACK, SLAM_LIGHTING, JUMP_ATTACK, BUFF, PLUNGE_IMPACT, BACKFLIP, ATTACKSTANCE_END };

public:
	CFennel();
	virtual ~CFennel();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;

	void AfterImageRender();
	void RenderRect();
	void RenderAttackRect();

private:
	void Falling();
	void SetAttackColliRect();

	// ReStart = -1.f -> ReStart == fMax
	inline void FrameChange(wstring stateKey, float Accel, float ReStart)
	{
		if (m_wstrStateKey != stateKey)
		{
			m_wstrStateKey = stateKey;
			m_tFrame.fFrame = 0.f;
			m_tFrame.fMax = CTextureMgr::GetInstance()->GetTextureSize(m_wstrObjKey.c_str(), m_wstrStateKey.c_str());
			m_tFrame.fFrameAccel = Accel;

			if(ReStart == -1.f)
				m_tFrame.fRestartFrame = m_tFrame.fMax;
			else
				m_tFrame.fRestartFrame = ReStart;

			m_tFrame.m_bIsEnd = false;
		}
	}

private:
	CObserver* m_pObserver;
	wstring m_wstrStateKey;

	STANCE m_eCurStance;

	// 점프 관련 함수
	float m_fJumpTime;
	float m_fJumpPower;
	float m_fPosY;
	float m_bIsJump;

	// 대화 관련 함수
	int TextIndex;

	// 렌더링 관련 함수
	D3DXMATRIX matPreWorld;
	D3DXMATRIX matPreWorld2;

	// 공격 관련 함수
	ATTACKSTANCE m_eCurAttack;
	bool m_bIsBuff;
	float m_fAttackTime;
	float m_fAttackRandomTime;

	D3DXVECTOR2 SlamLightingPos[4];
	bool m_bIsLighting;
	bool m_bIsLighting2;
	float m_fLightingDirX;
	int m_iLightingCount;

	float m_fDashTime;

	float m_fInviTime;

	int m_iHitRandom;

	int m_iAlpha;

	float m_fActivateTime;
	bool m_bIsActivate;
};

