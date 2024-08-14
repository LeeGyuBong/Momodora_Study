#include "stdafx.h"
#include "EffectIMP.h"

CEffectIMP::CEffectIMP()
	: m_pObj(nullptr), m_bIsInit(false), m_wstrStateKey(L""), m_iAlpha(255)
{
}


CEffectIMP::~CEffectIMP()
{
}

void CEffectIMP::LateInit()
{
	if (!m_bIsInit)
	{
		this->LateInit();
		m_bIsInit = true;
	}
}
