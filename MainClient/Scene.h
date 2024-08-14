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
	// 맵 외각에 관한 처리
	int MapOutSideProcessing();

protected:
	// 맵 변경
	bool MapChange(wstring ID);
};

