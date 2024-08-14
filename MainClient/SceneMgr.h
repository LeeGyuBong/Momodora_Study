#pragma once

class CScene;
class CSceneMgr
{
	DECLARE_SINGLETON(CSceneMgr)

public:
	enum SCENE { LOGO, STAGE, END };

private:
	CSceneMgr();
	~CSceneMgr();

public:
	SCENE GetCurScene() { return m_eCurScene; }

	void SetBoss(bool FightorNot) { m_bIsBoss = FightorNot; }
	void SetBossfight(bool FightorNot) { m_bIsBossFight = FightorNot; }

	const bool& GetBoss() { return m_bIsBoss; }
	const bool& GetBossFight() { return m_bIsBossFight; }

public:
	void SceneChange(SCENE eScene);
	void Update();
	void LateUpdate();
	void Render();
	void Release();

private:
	CScene* m_pScene;
	SCENE m_eCurScene;
	SCENE m_ePreScene;

	bool m_bIsBoss;
	bool m_bIsBossFight;
};

