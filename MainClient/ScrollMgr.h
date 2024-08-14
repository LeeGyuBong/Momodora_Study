#pragma once

class CScrollMgr
{
public:
	CScrollMgr();
	~CScrollMgr();

public:
	static void ScrollLock();
	static void ScrollInit();
	static const D3DXVECTOR3& GetScroll() { return Scroll; }
	static void SetScroll(D3DXVECTOR3 vScroll);
	static void ScrollShake();
	static void ScreenShakeOn() { m_bIsShake = true; }
	static void ScreenShakeSmallOn() { m_bIsShakeSmall = true; }

	static void ChangeScrollInit(D3DXVECTOR3 vPos);
	// 특정 대상 스크롤 적용
	static void ChangeScroll() { m_bIsChangeInit = true; }
	// 플레이어 중심 스크롤 재적용
	static void ReturnScroll() { m_bIsChangeInit = false; }

	static bool GetChangeInit() { return m_bIsChangeInit; }

	static void Reset();

private:
	static D3DXVECTOR3 Scroll;

	static float m_fShakeTurn;
	static float m_fShakeTime;
	static bool m_bIsShake;
	static bool m_bIsShakeSmall;

	static bool m_bIsChangeInit;
};

