#pragma once
class CTimeMgr
{
	DECLARE_SINGLETON(CTimeMgr)

private:
	CTimeMgr();
	~CTimeMgr();

public:
	void InitTime();
	void UpdateTime();

	float GetTime() { return m_fTime; }

private:
	LARGE_INTEGER m_CurTime; // 현재 시간 변수
	LARGE_INTEGER m_OldTime1; // 지나간 시간을 비교할 변수
	LARGE_INTEGER m_OldTime2; // 진동수와의 갭을 비교할 변수
	LARGE_INTEGER m_CpuTick; // Cpu진동수를 확인할 변수

	float m_fTime;
};

