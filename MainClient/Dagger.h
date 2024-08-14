#pragma once
#include "Obj.h"
class CDagger :
	public CObj
{
public:
	CDagger();
	virtual ~CDagger();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;

	void Bouncing();
	void RenderRect();

private:
	wstring m_wstrStateKey;

	float m_fLifeTime;

	bool m_bIsBounce;
	float m_fPosY;
	float m_fJumpTime;
};

