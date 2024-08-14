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

	// ����Ű
	if (GetAsyncKeyState(VK_UP) & 0x8000)
		m_dwKey |= KEY_UP;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		m_dwKey |= KEY_DOWN;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_dwKey |= KEY_LEFT;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_dwKey |= KEY_RIGHT;

	// ����
	if (GetAsyncKeyState('A') & 0x8000)
		m_dwKey |= KEY_A;
	// ���� ����
	if (GetAsyncKeyState('S') & 0x8000)
		m_dwKey |= KEY_S;
	// ���Ÿ� ����
	if (GetAsyncKeyState('D') & 0x8000)
		m_dwKey |= KEY_D;
	// ������
	if (GetAsyncKeyState('Q') & 0x8000)
		m_dwKey |= KEY_Q;
	// ������ ���
	if (GetAsyncKeyState('W') & 0x8000)
		m_dwKey |= KEY_W;
	// ������ ��ü
	if (GetAsyncKeyState('E') & 0x8000)
		m_dwKey |= KEY_E;
	// UI Ű�� ����
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
	// ���� ���ȴ��� ������ Ȯ���Ѵ�
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
	// ���� ���Ȱ� ������ ������ �ʾ�����
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
	// ���� ���Ȱ� ������ �������� ������ �����ٴ� ������ �־��ش�
	if ((m_dwKey & dwKey) && !(m_dwKeyPressed & dwKey))
	{
		m_dwKeyPressed |= dwKey;
		
		return false;
	}
	// ���� ������ ������ ���� ���� ������ ���� �ð��� ����Ѵ�.
	else if ((m_dwKey & dwKey) && (m_dwKeyPressed & dwKey))
	{
		m_fPressingTime += CTimeMgr::GetInstance()->GetTime();

		return true;
	}
	// ���� ������ �ʰ� ������ �������� ������ �����ٴ� ������ �����ش�.
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
	// ���� ���Ȱ� ������ �������� ������ �����ٴ� ������ �־��ش�
	if ((m_dwKey & dwKey) && !(m_dwKeyPressedUp & dwKey))
	{
		m_dwKeyPressedUp |= dwKey;

		return false;
	}
	// ���� ������ ������ ���� ���� ������ ���� �ð��� ����Ѵ�.
	else if ((m_dwKey & dwKey) && (m_dwKeyPressedUp & dwKey))
	{
		m_fPressingUpTime += CTimeMgr::GetInstance()->GetTime();

		return false;
	}
	// ���� ������ �ʰ� ������ �������� ������ �����ٴ� ������ �����ش�.
	else if (!(m_dwKey & dwKey) && (m_dwKeyPressedUp & dwKey))
	{
		m_dwKeyPressedUp ^= dwKey;
		return true;
	}
	// ����� ������ ���� ���� ������ ���� �ð��� �ʱ�ȭ
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