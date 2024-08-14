#pragma once

class CObj;
class CDataMgr;

typedef struct tagInfo
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vDir;
	D3DXVECTOR3 vLook;
	D3DXVECTOR3 vSize;

	D3DXMATRIX matWorld;
}INFO;

typedef struct tagFrame
{
	tagFrame()
		: fFrame(0.f), fMax(0.f), fRestartFrame(0.f), fFrameAccel(1.f), m_bIsEnd(false)
	{}
	tagFrame(float Accel, float re = 0.f)
		: fFrameAccel(Accel), fRestartFrame(re)
	{}
	tagFrame(float frame, float max, float re, float accel)
		: fFrame(frame), fMax(max), fRestartFrame(re), fFrameAccel(accel)
	{}

	float fFrame; // 현재 애니메이션 재생 구간.
	float fMax;	// 최대 재생 길이.
	float fRestartFrame; // 반복시 시작 프레임
	float fFrameAccel;
	bool m_bIsEnd; // 애니메이션 종료 여부
}FRAME;

typedef struct tagStatus
{
	int HP;
	int MaxHP;
	int Attack;
	
}STATUS;

typedef struct tagTexture
{
	// 이미지 한장을 제어할 com 객체
	LPDIRECT3DTEXTURE9 pTexture;
	// 이미지 정보를 저장할 구조체
	D3DXIMAGE_INFO tImgInfo;
}TEXINFO;

typedef struct tagTexturePath
{
	wstring wstrObjKey = L"";
	wstring wstrStateKey = L"";
	wstring wstrPath = L"";
	int iCount = 0;
}IMGPATH;

typedef struct tagTile
{
	tagTile() : ReversalX(1.f), ReversalY(1.f) {}

	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vSize;

	BYTE byDrawID;
	BYTE byOption;

	float ReversalX;
	float ReversalY;

	const D3DXVECTOR3& GetPos() const { return vPos; }
	const D3DXVECTOR3& GetSize() const { return vSize; }
	const BYTE& GetOption() const { return byOption; }
}TILE;

typedef struct tagMonster
{
#ifndef _AFX
	wstring strID;
#else
	CString strID;
#endif
	int HP;

	D3DXVECTOR3 vPos;
	BYTE byDrawID;

}MONSTER;

typedef struct tagMapLink
{
#ifndef _AFX
	wstring MyMapID;

	wstring UpMapID;
	wstring DownMapID;
	wstring LeftMapID;
	wstring RightMapID;
#else
	CString MyMapID;

	CString UpMapID;
	CString DownMapID;
	CString LeftMapID;
	CString RightMapID;
#endif
	

}MAPLINK;

typedef struct tagGreed
{
	tagGreed() : bIsColli(false) 
	{
		ZeroMemory(&rect, sizeof(RECT));
	}
	tagGreed(const RECT& rc) : bIsColli(false)
	{
		memcpy_s(&rect, sizeof(RECT), &rc, sizeof(RECT));
	}
	~tagGreed()
	{
		ListReset();
	}

	bool bIsColli;
	RECT rect;

	// 리스트 내 정보를 동적해제 하지 말것.
	list<TILE*> ColliTileLst;
	// 리스트 내 정보를 동적해제 하지 말것.
	list<CObj*> ColliObjLst[OBJECT::END];

	// colli - true
	void IsColli() { bIsColli = true; }
	// colli - false
	void NotColli() { bIsColli = false; }
	// 오브젝트 리스트 비우기
	void ObjLstClear() 
	{
		for (int i = 0; i<OBJECT::END; ++i)
			ColliObjLst[(OBJECT::TYPE)i].clear();
	}
	// 단순 리스트 비우기(clear)
	void ListReset()
	{
		ColliTileLst.clear();
		for(int i=0;i<OBJECT::END;++i)
			ColliObjLst[(OBJECT::TYPE)i].clear();
		NotColli();
	}
}GREED;

#ifndef _AFX
typedef struct tagItem
{
	tagItem() : count(0) {}
	tagItem(const DWORD id)
		: ID(id), count(0)
	{
		if (ID == None)
			Name = L"[None]";
	}
	tagItem(const DWORD id, const wstring name, ITEMTYPE type, const wstring effect, const wstring detail)
		: ID(id), Name(name), eType(type), Effect(effect), Detail(detail), count(0)
	{}

	// ID 값
	DWORD ID;
	// 이름
	wstring Name;
	// 타입
	ITEMTYPE eType;
	// 효능
	wstring Effect;
	// 세부 설명
	wstring Detail;

	int count;
}ITEM;
#else
#endif

typedef struct tagInteract
{
	tagInteract()
	{
		vPos = {};
		vSize = {};
		ID = None;
		CurInteract = false;
	}

	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vSize;
	DWORD ID;

	bool CurInteract;

}INTERACT;