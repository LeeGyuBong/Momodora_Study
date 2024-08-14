#pragma once
#include "Obj.h"

class CObserver;
class CArrow :
	public CObj
{
public:
	CArrow();
	virtual ~CArrow();

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
	CObserver* m_pObserver;
	float m_fLifeTime;
	float m_fDirX;

	wstring m_wstrStateKey;

	bool m_bIsBounce;
	float m_fJumpTime;
	float m_fPosY;
};

