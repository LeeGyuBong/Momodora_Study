#include "stdafx.h"
#include "TimeMgr.h"

IMPLEMENT_SINGLETON(CTimeMgr)

CTimeMgr::CTimeMgr()
	: m_fTime(0.f)
{
	ZeroMemory(&m_CurTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_OldTime1, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_OldTime2, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_CpuTick, sizeof(LARGE_INTEGER));
}


CTimeMgr::~CTimeMgr()
{
}

void CTimeMgr::InitTime()
{
	// ���κ��忡 �ִ� ���ػ� Ÿ�̸ӿ��� ���� ������ �ð��� 1/10000�� ������ �޾ƿ��� �Լ�
	QueryPerformanceCounter(&m_CurTime);
	QueryPerformanceCounter(&m_OldTime1);
	QueryPerformanceCounter(&m_OldTime2);

	// Cpu�� ���� �ֱ⸦ �޾ƿ��� �Լ�
	QueryPerformanceFrequency(&m_CpuTick);
}

void CTimeMgr::UpdateTime()
{
	// �� ������Ʈ ���� �ð��� ����
	QueryPerformanceCounter(&m_CurTime);

	// ����ð��� ���Žð��� ���� �������͸� ���� ���
	if (m_CurTime.QuadPart - m_OldTime2.QuadPart > m_CpuTick.QuadPart)
	{
		// �������� ���� �޴´�
		QueryPerformanceFrequency(&m_CpuTick);
		m_OldTime2 = m_CurTime;
	}

	m_fTime = static_cast<float>(m_CurTime.QuadPart - m_OldTime1.QuadPart) / m_CpuTick.QuadPart;
	m_OldTime1 = m_CurTime;
}
