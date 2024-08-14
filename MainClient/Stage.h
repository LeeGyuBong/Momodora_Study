#pragma once
#include "Scene.h"

class CStage :
	public CScene
{
public:
	CStage();
	virtual ~CStage();

public:
	// CScene을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;

private:
	void RenderChangeBlend();
	void CoiisionCollect();

	void NotMapOut();

	void BossFight();

	void DeadRender();

private:
	int m_iMapChangeEvent;

	// 맵 이동시 페이드 인 관련 변수
	bool m_bIsBlend;
	int m_iChangeAlpha;

	D3DXVECTOR3 BossScrollPos;
	D3DXVECTOR3 MovePos;
	bool m_bIsBossLock;
	float m_fCreateTime;

	int m_iDeadAlpha;
};

