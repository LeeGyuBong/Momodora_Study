#pragma once
#include "Obj.h"
class CJumpEffect :
	public CObj
{
public:
	CJumpEffect();
	virtual ~CJumpEffect();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;

private:
	int m_iAlpha;
	float m_fTime;
	float m_fSpinDir;

	float m_fAlphaTime;
};

