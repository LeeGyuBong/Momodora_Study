#pragma once
#include "Effect.h"
class CChargeEffect :
	public CEffect
{
public:
	CChargeEffect();
	virtual ~CChargeEffect();

	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;

private:
	float m_fTime;
};