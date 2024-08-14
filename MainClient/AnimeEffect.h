#pragma once
#include "EffectIMP.h"

class CAnimeEffect :
	public CEffectIMP
{
public:
	CAnimeEffect();
	virtual ~CAnimeEffect();

	// CEffectIMP을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;

public:
	void SetFrame(const FRAME& frame) { m_tFrame = frame; }
	FRAME& GetFrame() { return m_tFrame; }
private:
	FRAME m_tFrame;
};

