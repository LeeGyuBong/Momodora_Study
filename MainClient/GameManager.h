#pragma once

class CGameManager
{
public:
	CGameManager();
	~CGameManager();

	void Initialize();	// �ʱ�ȭ
	void Update();		// ����
	void LateUpdate();	//
	void Render();		// ������
	void Release();		// ����
};

