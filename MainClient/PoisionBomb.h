#pragma once
#include "Obj.h"
class CPoisionBomb :
	public CObj
{
public:
	CPoisionBomb();
	virtual ~CPoisionBomb();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;

	void RenderRect();

private:
	wstring m_wstrStateKey;

	float m_fPosY;
	float m_fJumpTime;

	float m_fSpawnTime;
	float m_fIntervalTime;
};

