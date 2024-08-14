#pragma once

class CGameManager
{
public:
	CGameManager();
	~CGameManager();

	void Initialize();	// 초기화
	void Update();		// 구동
	void LateUpdate();	//
	void Render();		// 렌더링
	void Release();		// 해제
};

