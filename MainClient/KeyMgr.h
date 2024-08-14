#pragma once
class CKeyMgr
{
	DECLARE_SINGLETON(CKeyMgr)

private:
	CKeyMgr();
	~CKeyMgr();

public:
	void KeyCheck();
	bool KeyUp(DWORD dwKey);
	bool KeyDown(DWORD dwKey);
	bool KeyPressing(DWORD dwKey);
	bool KeyPressingUp(DWORD dwKey);
	bool KeyCombine(DWORD dwFirstKey, DWORD dwSecondKey);

	const DWORD& GetKeyState() const { return m_dwKey; }
	const float& GetPressingTime() const { return m_fPressingTime; }
	const float& GetPressingUpTime() const { return m_fPressingUpTime; }

private:
	DWORD m_dwKey;
	DWORD m_dwKeyUp;
	DWORD m_dwKeyDown;
	DWORD m_dwKeyPressed;
	DWORD m_dwKeyPressedUp;
	
	float m_fPressingTime;
	float m_fPressingUpTime;
};
