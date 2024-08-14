#pragma once
#include "Obj.h"
class CPoisionCloud :
	public CObj
{
public:
	CPoisionCloud();
	virtual ~CPoisionCloud();

public:
	// CObj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;

	void RenderRect();

private:
	wstring m_wstrStateKey;
};

