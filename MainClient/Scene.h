#pragma once

#include "Terrain.h"
class CScene
{
public:
	CScene();
	virtual ~CScene();

public:
	virtual void Initialize() PURE;
	virtual void Update() PURE;
	virtual void LateUpdate() PURE;
	virtual void Render() PURE;
	virtual void Release() PURE;

public:
	// �� �ܰ��� ���� ó��
	int MapOutSideProcessing();

protected:
	// �� ����
	bool MapChange(wstring ID);
};

