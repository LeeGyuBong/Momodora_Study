#pragma once

#define WINCX 1280	
#define WINCY 720

#define CLIENTWINCX 800
#define CLIENTWINCY 600

// 그리드 사이즈는 무조건 홀수로
#define GREEDSIZE 321

#define TILESIZE 32
#define SIZEGAP TILESIZE * 0.5

#define MIN_STR 64
#define MID_STR 128
#define MAX_STR 256

// 이벤트
#define NO_EVENT 0
#define DEAD_OBJ 1

// 추상클래스 이벤트
#define ANIMATION_END 1
#define ATTACK_END 2

// 타일 옵션
#define N_BLOCK 0
#define Y_BLOCK 1
#define LADDER	2
#define TRAP 3
#define SWITCH	4
#define JUST_SELECT 5

#define ON true
#define OFF false

#define RESTART_MAX 99.f

// Tool 표시 여부 비트값
#define VISIBLE_GREED	0x0001
#define VISIBLE_TILE	0x0002
#define VISIBLE_OBJECT	0x0004

// 몬스터 아이디
#define IMP_ID ID_001

// 중력값
#define GRAVITY 9.8f

#define NULL_CHECK(ptr) if(ptr == nullptr) return;
 
#define ERR_MSG(str) MessageBox(g_hWnd, str, L"Error", MB_OK);

#define NO_COPY(ClassName)				\
private:								\
	ClassName(ClassName& _obj);			\
	ClassName& operator=(ClassName& _obj);

#define DECLARE_SINGLETON(ClassName)	\
		NO_COPY(ClassName)				\
public:									\
	static ClassName* GetInstance()		\
	{									\
		if(m_pInstance == nullptr)		\
			m_pInstance = new ClassName;\
		return m_pInstance;				\
	}									\
	void DestroyInstance()				\
	{									\
		if(m_pInstance)					\
		{								\
			delete m_pInstance;			\
			m_pInstance = nullptr;		\
		}								\
	}									\
private:								\
	static ClassName* m_pInstance;	

#define IMPLEMENT_SINGLETON(ClassName)	\
	ClassName* ClassName::m_pInstance = nullptr;	

#define D3DFVF_CUSTOM  D3DFVF_XYZRHW | D3DFVF_DIFFUSE