#pragma once

#include "Obj.h"

template <typename T>
class CAbstractFactory
{
public:
	static CObj* CreateObject()
	{
		CObj* pObj = new T;
		pObj->Initialize();

		return pObj;
	}

	static CObj* CreateObject(INFO info)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->SetInfo(info);

		return pObj;
	}

	static CObj* CreateObject(D3DXVECTOR3 pos)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->SetPos(pos);

		return pObj;
	}

	static CObj* CreateObject(float degree)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->SetDegree(degree);

		return pObj;
	}

	static CObj* CreateObject(D3DXVECTOR3 pos, float degree)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->SetPos(pos);
		pObj->SetDegree(degree);

		return pObj;
	}

	static CObj* CreateObject(D3DXVECTOR3 pos, int Hp)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->SetPos(pos);
		pObj->SetHp(Hp);

		return pObj;
	}

	static CObj* CreateObject(D3DXVECTOR3 pos, D3DXVECTOR3 dir)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->SetPos(pos);
		pObj->SetDir(dir);

		return pObj;
	}

	static CObj* CreateObject(D3DXVECTOR3 pos, float degree, float dirX)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->SetPos(pos);
		pObj->SetDirX(dirX);
		pObj->SetDegree(degree);

		return pObj;
	}
};

template <typename T1, typename T2>
class CEffectFactory
{
public:
	// NonAnimeEffect
	static CObj* CreateEffect(const TCHAR* szObjKey, const TCHAR* szStateKey, int drawID, int alpha = 255, float time = 0.f)
	{
		T2* pImp = new T2;
		pImp->SetStateKey(szStateKey);
		pImp->SetDrawID(drawID);
		pImp->SetAlpha(alpha);

		CObj* pEffect = CAbstractFactory<T1>::CreateObject();
		pEffect->SetObjKey(szObjKey);
		dynamic_cast<T1*>(pEffect)->SetLifeTime(time);
		dynamic_cast<T1*>(pEffect)->SetBridge(pImp);

		pImp->SetObj(pEffect);

		return pEffect;
	}
	// AnimeEffect
	static CObj* CreateEffect(const TCHAR* szObjKey, const TCHAR* szStateKey, FRAME& tFrame, int alpha = 255, float time = 0.f)
	{
		T2* pImp = new T2;
		pImp->SetStateKey(szStateKey);
		pImp->SetFrame(tFrame);
		pImp->SetAlpha(alpha);

		CObj* pEffect = CAbstractFactory<T1>::CreateObject();
		pEffect->SetObjKey(szObjKey);
		dynamic_cast<T1*>(pEffect)->SetLifeTime(time);
		dynamic_cast<T1*>(pEffect)->SetBridge(pImp);

		pImp->SetObj(pEffect);

		return pEffect;
	}
	// NonAnimeEffect
	static CObj* CreateEffect(const TCHAR* szObjKey, const TCHAR* szStateKey, D3DXVECTOR3& vPos, int drawID, int alpha = 255, float time = 0.f, float degree = 0.f)
	{
		T2* pImp = new T2;
		pImp->SetStateKey(szStateKey);
		pImp->SetDrawID(drawID);
		pImp->SetAlpha(alpha);

		CObj* pEffect = CAbstractFactory<T1>::CreateObject(vPos);
		pEffect->SetObjKey(szObjKey);
		dynamic_cast<T1*>(pEffect)->SetLifeTime(time);
		dynamic_cast<T1*>(pEffect)->SetDegree(degree);
		dynamic_cast<T1*>(pEffect)->SetBridge(pImp);

		pImp->SetObj(pEffect);

		return pEffect;
	}

	// AnimeEffect
	static CObj* CreateEffect(const TCHAR* szObjKey, const TCHAR* szStateKey, D3DXVECTOR3& vPos, FRAME& tFrame, int alpha = 255, float time = 0.f)
	{
		T2* pImp = new T2;
		pImp->SetStateKey(szStateKey);
		pImp->SetFrame(tFrame);
		pImp->SetAlpha(alpha);

		CObj* pEffect = CAbstractFactory<T1>::CreateObject(vPos);
		pEffect->SetObjKey(szObjKey);
		dynamic_cast<T1*>(pEffect)->SetLifeTime(time);
		dynamic_cast<T1*>(pEffect)->SetBridge(pImp);

		pImp->SetObj(pEffect);

		return pEffect;
	}
	static CObj* CreateEffect(const TCHAR* szObjKey, const TCHAR* szStateKey, INFO& info, FRAME& tFrame, int alpha = 255, float time = 0.f)
	{
		T2* pImp = new T2;
		pImp->SetStateKey(szStateKey);
		pImp->SetFrame(tFrame);
		pImp->SetAlpha(alpha);

		CObj* pEffect = CAbstractFactory<T1>::CreateObject(info);
		pEffect->SetObjKey(szObjKey);
		dynamic_cast<T1*>(pEffect)->SetLifeTime(time);
		dynamic_cast<T1*>(pEffect)->SetBridge(pImp);

		pImp->SetObj(pEffect);

		return pEffect;
	}
};