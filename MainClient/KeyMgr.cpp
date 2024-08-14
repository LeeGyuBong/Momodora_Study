#include "stdafx.h"
#include "KeyMgr.h"

IMPLEMENT_SINGLETON(CKeyMgr)

CKeyMgr::CKeyMgr()
	: m_dwKey(0), m_dwKeyUp(0), m_dwKeyDown(0), m_dwKeyPressed(0), 
	m_fPressingTime(0.f), m_fPressingUpTime(0.f), m_dwKeyPressedUp(0)
{
}


CKeyMgr::~CKeyMgr()
{
}

void CKeyMgr::KeyCheck()
{
	m_dwKey = 0;

	// 방향키
	if (GetAsyncKeyState(VK_UP) & 0x8000)
		m_dwKey |= KEY_UP;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		m_dwKey |= KEY_DOWN;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_dwKey |= KEY_LEFT;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_dwKey |= KEY_RIGHT;

	// 점프
	if (GetAsyncKeyState('A') & 0x8000)
		m_dwKey |= KEY_A;
	// 근접 공격
	if (GetAsyncKeyState('S') & 0x8000)
		m_dwKey |= KEY_S;
	// 원거리 공격
	if (GetAsyncKeyState('D') & 0x8000)
		m_dwKey |= KEY_D;
	// 구르기
	if (GetAsyncKeyState('Q') & 0x8000)
		m_dwKey |= KEY_Q;
	// 아이템 사용
	if (GetAsyncKeyState('W') & 0x8000)
		m_dwKey |= KEY_W;
	// 아이템 교체
	if (GetAsyncKeyState('E') & 0x8000)
		m_dwKey |= KEY_E;
	// UI 키고 끄기
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
		m_dwKey |= KEY_SHIFT;


	if (GetAsyncKeyState('V') & 0x8000)
		m_dwKey |= KEY_V;
	if (GetAsyncKeyState('R') & 0x8000)
		m_dwKey |= KEY_R;
	if (GetAsyncKeyState('1') & 0x8000)
		m_dwKey |= KEY_1;
	if (GetAsyncKeyState('2') & 0x8000)
		m_dwKey |= KEY_2;
}

bool CKeyMgr::KeyUp(DWORD dwKey)
{
	// 현재 눌렸는지 정보를 확인한다
	if (m_dwKey & dwKey)
	{
		m_dwKeyUp |= dwKey;
		return false;
	}
	else if (m_dwKeyUp & dwKey)
	{
		m_dwKeyUp ^= dwKey;
		return true;
	}

	return false;
}

bool CKeyMgr::KeyDown(DWORD dwKey)
{
	// 현재 눌렸고 이전에 눌리지 않았으면
	if ((m_dwKey & dwKey) && !(m_dwKeyDown & dwKey))
	{
		m_dwKeyDown |= dwKey;
		return true;
	}
	else if (!(m_dwKey & dwKey) && (m_dwKeyDown & dwKey))
	{
		m_dwKeyDown ^= dwKey;
		return false;
	}

	return false;
}

bool CKeyMgr::KeyPressing(DWORD dwKey)
{
	// 현재 눌렸고 이전에 눌린적이 없으면 눌렀다는 정보를 넣어준다
	if ((m_dwKey & dwKey) && !(m_dwKeyPressed & dwKey))
	{
		m_dwKeyPressed |= dwKey;
		
		return false;
	}
	// 현재 눌리고 이전에 눌린 적이 있으면 눌린 시간을 계산한다.
	else if ((m_dwKey & dwKey) && (m_dwKeyPressed & dwKey))
	{
		m_fPressingTime += CTimeMgr::GetInstance()->GetTime();

		return true;
	}
	// 현재 눌리지 않고 이전에 눌린적이 있으면 눌렀다는 정보를 없애준다.
	else if (!(m_dwKey & dwKey) && (m_dwKeyPressed & dwKey))
	{
		m_dwKeyPressed ^= dwKey;
		m_fPressingTime = 0.f;

		return false;
	}

	return false;
}

bool CKeyMgr::KeyPressingUp(DWORD dwKey)
{
	// 현재 눌렸고 이전에 눌린적이 없으면 눌렀다는 정보를 넣어준다
	if ((m_dwKey & dwKey) && !(m_dwKeyPressedUp & dwKey))
	{
		m_dwKeyPressedUp |= dwKey;

		return false;
	}
	// 현재 눌리고 이전에 눌린 적이 있으면 눌린 시간을 계산한다.
	else if ((m_dwKey & dwKey) && (m_dwKeyPressedUp & dwKey))
	{
		m_fPressingUpTime += CTimeMgr::GetInstance()->GetTime();

		return false;
	}
	// 현재 눌리지 않고 이전에 눌린적이 있으면 눌렀다는 정보를 없애준다.
	else if (!(m_dwKey & dwKey) && (m_dwKeyPressedUp & dwKey))
	{
		m_dwKeyPressedUp ^= dwKey;
		return true;
	}
	// 현재와 이전에 눌린 적이 없으면 눌린 시간을 초기화
	else if (!(m_dwKey & dwKey) && !(m_dwKeyPressedUp & dwKey))
	{
		m_fPressingUpTime = 0.f;
		return false;
	}

	return false;
}

bool CKeyMgr::KeyCombine(DWORD dwFirstKey, DWORD dwSecondKey)
{
	if (KeyDown(dwSecondKey) && (m_dwKey & dwFirstKey))
		return true;

	return false;
}