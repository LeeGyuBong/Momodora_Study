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
	// 메인보드에 있는 고해상도 타이머에서 현재 누적된 시간을 1/10000초 단위로 받아오는 함수
	QueryPerformanceCounter(&m_CurTime);
	QueryPerformanceCounter(&m_OldTime1);
	QueryPerformanceCounter(&m_OldTime2);

	// Cpu의 진동 주기를 받아오는 함수
	QueryPerformanceFrequency(&m_CpuTick);
}

void CTimeMgr::UpdateTime()
{
	// 매 업데이트 마다 시간을 갱신
	QueryPerformanceCounter(&m_CurTime);

	// 현재시간과 과거시간의 갭이 진동수와를 넘을 경우
	if (m_CurTime.QuadPart - m_OldTime2.QuadPart > m_CpuTick.QuadPart)
	{
		// 진동수를 새로 받는다
		QueryPerformanceFrequency(&m_CpuTick);
		m_OldTime2 = m_CurTime;
	}

	m_fTime = static_cast<float>(m_CurTime.QuadPart - m_OldTime1.QuadPart) / m_CpuTick.QuadPart;
	m_OldTime1 = m_CurTime;
}
