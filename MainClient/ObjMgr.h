#pragma once

class CObj;
class CObjMgr
{
	DECLARE_SINGLETON(CObjMgr)

private:
	CObjMgr();
	~CObjMgr();

public:
	void AddObject(CObj* obj, OBJECT::TYPE eType);
	void Update();
	void LateUpdate();
	void Render();
	void Release();

	void RenderBoss();
	void RenderEffect();
	void RenderPlayerAttackEffect();

public:
	CObj*& GetPlayer() { return m_ObjLst[OBJECT::PLAYER].front(); }
	OBJLST& GetObjLst(OBJECT::TYPE eType) {return m_ObjLst[eType];}

private:
	OBJLST m_ObjLst[OBJECT::END];
};

