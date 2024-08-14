#pragma once
#include "Effect.h"
class CAttackEffect :
	public CEffect
{
public:
	CAttackEffect();
	virtual ~CAttackEffect();

	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;
};

