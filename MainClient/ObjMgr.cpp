	#include "stdafx.h"
#include "ObjMgr.h"
#include "Terrain.h"

IMPLEMENT_SINGLETON(CObjMgr)

CObjMgr::CObjMgr()
{
}

CObjMgr::~CObjMgr()
{
	Release();
}

void CObjMgr::AddObject(CObj* obj, OBJECT::TYPE eType)
{
	m_ObjLst[eType].push_back(obj);
}		

void CObjMgr::Update()
{
	for (int i = 0; i < OBJECT::END; ++i)
	{
		if (m_ObjLst[i].empty())
			continue;

		OBJLST_ITER iter_begin = m_ObjLst[i].begin();
		OBJLST_ITER iter_end = m_ObjLst[i].end();
		for (; iter_begin != iter_end;)
		{
			int Event = (*iter_begin)->Update();

			if (Event == DEAD_OBJ)
			{
				SafeDelete(*iter_begin);
				iter_begin = m_ObjLst[i].erase(iter_begin);
			}
			else
				++iter_begin;
		}
	}
}

void CObjMgr::LateUpdate()
{
	for (auto& list : m_ObjLst)
	{
		for (auto*& obj : list)
		{
			obj->LateUpdate();
		}
	}

	// 오브젝트 간의 충돌체크

}

void CObjMgr::Render()
{
	for (auto& list : m_ObjLst)
	{
		if (list == m_ObjLst[OBJECT::BOSS] || list == m_ObjLst[OBJECT::EFFECT] ||
			list == m_ObjLst[OBJECT::PLAYER_ATTACK])
			continue;

		for (auto*& obj : list)
		{
			obj->Render();
		}
	}
}

void CObjMgr::Release()
{
	for (auto& list : m_ObjLst)
	{
		for_each(list.begin(), list.end(), SafeDelete<CObj*>);
		list.clear();
	}
}

void CObjMgr::RenderBoss()
{
	for (auto*& obj : m_ObjLst[OBJECT::BOSS])
	{
		obj->Render();
	}
}

void CObjMgr::RenderEffect()
{
	for (auto*& obj : m_ObjLst[OBJECT::EFFECT])
	{
		obj->Render();
	}
}

void CObjMgr::RenderPlayerAttackEffect()
{
	for (auto*& obj : m_ObjLst[OBJECT::PLAYER_ATTACK])
	{
		obj->Render();
	}
}