#pragma once

class CMath
{
public:
	CMath();
	~CMath();

public:
	// 두 오브젝트의 거리 반환
	static float CalculateDistance(D3DXVECTOR3 dstPos, D3DXVECTOR3 srcPos);
	// 두 오브젝트의 거리 X좌표 값만 반환
	static float CalculateDistanceX(D3DXVECTOR3 dstPos, D3DXVECTOR3 srcPos);

	// 두 오브젝트의 각도 반환
	static float CalculateDegree(INFO dstInfo, INFO srcInfo);

	// 오브젝트 행렬 계산
	static D3DXMATRIX CalculateMatrix(D3DXVECTOR3 pos, D3DXVECTOR3 scale = { 1.f,1.f,0.f }, ROTATION eType = ROTATION::Z, float degree = 0.f);
	
	// 오브젝트가 있는 위치의 그리드의 인덱스 계산
	static int CalculateGreedIndex(D3DXVECTOR3 vPos);
};

