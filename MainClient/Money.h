#pragma once
#include "Obj.h"
class CMoney :
	public CObj
{
public:
	CMoney();
	virtual ~CMoney();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;

	void RenderRect();

private:
	float m_fPosY;
	float m_fJumpPower;
	float m_fJumpTime;

	float m_fLifeTime;
};

