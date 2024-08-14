#include "stdafx.h"
#include "Math.h"

CMath::CMath()
{
}


CMath::~CMath()
{
}

float CMath::CalculateDistance(D3DXVECTOR3 dstPos, D3DXVECTOR3 srcPos)
{
	return D3DXVec3Length(&(dstPos - srcPos));
}

float CMath::CalculateDistanceX(D3DXVECTOR3 dstPos, D3DXVECTOR3 srcPos)
{
	return fabs(dstPos.x - srcPos.x);
}

float CMath::CalculateDegree(INFO dstInfo, INFO srcInfo)
{
	D3DXVECTOR3 Dir = dstInfo.vPos - srcInfo.vPos;

	D3DXVec3Normalize(&Dir, &Dir);

	float Dot = D3DXVec3Dot(&Dir, &srcInfo.vLook);
	float Angle = acosf(Dot);

	if (srcInfo.vPos.y < dstInfo.vPos.y)
		Angle = 2.f * D3DX_PI - Angle;

	return D3DXToDegree(Angle);
}

D3DXMATRIX CMath::CalculateMatrix(D3DXVECTOR3 pos, D3DXVECTOR3 scale/* = { 1.f,1.f,0.f }*/, ROTATION eType/*= ROTATION::Z*/,
	float degree/* = 0.f*/)
{
	D3DXMATRIX matScale, matRota, matTrans, matWorld;

	D3DXMatrixIdentity(&matWorld);

	D3DXMatrixScaling(&matScale, scale.x, scale.y, 0.f);
	switch (eType)
	{
	case X:
		D3DXMatrixRotationX(&matRota, D3DXToRadian(degree));
		break;
	case Y:
		D3DXMatrixRotationY(&matRota, D3DXToRadian(degree));
		break;
	case Z:
		D3DXMatrixRotationZ(&matRota, D3DXToRadian(degree));
		break;
	}
	D3DXMatrixTranslation(&matTrans, pos.x, pos.y, 0.f);

	// 크기 * 회전 * 위치
	matWorld = matScale * matRota * matTrans;
	
	return matWorld;
}

int CMath::CalculateGreedIndex(D3DXVECTOR3 vPos)
{
	int X = (int)vPos.x / GREEDSIZE;
	int Y = (int)vPos.y / GREEDSIZE;

	return (X + (CTerrainMgr::GetInstance()->GetGreedX() * Y));
}