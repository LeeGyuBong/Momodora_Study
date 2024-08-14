#pragma once
#include "EffectIMP.h"
class CNonAnimeEffect :
	public CEffectIMP
{
public:
	CNonAnimeEffect();
	virtual ~CNonAnimeEffect();

	// CEffectIMP��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;

public:
	void SetDrawID(int drawID) { m_iDrawID = drawID; }

private:
	int m_iDrawID;
	float m_fTime;
};

