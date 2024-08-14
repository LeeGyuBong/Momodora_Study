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
	LARGE_INTEGER m_CurTime; // ���� �ð� ����
	LARGE_INTEGER m_OldTime1; // ������ �ð��� ���� ����
	LARGE_INTEGER m_OldTime2; // ���������� ���� ���� ����
	LARGE_INTEGER m_CpuTick; // Cpu�������� Ȯ���� ����

	float m_fTime;
};

