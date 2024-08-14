#pragma once
#include "Obj.h"
#include "PlayerInfo.h"
#include "EffectIMP.h"

class CObserver;
class CEffectIMP;
class CEffect :
	public CObj
{
public:
	CEffect();
	virtual ~CEffect();

public:
	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;

public:
	void SetBridge(CEffectIMP* pBridge) { m_pBridge = pBridge; }
	void SetLifeTime(float time) { m_fLifeTime = time; }
	void SetDegree(float degree) { m_fDegree = degree; }
	
protected:
	CEffectIMP* m_pBridge;
	CObserver* m_pObserver;
	float m_fLifeTime;
	float m_fTime;

	float m_fDegree;
};

