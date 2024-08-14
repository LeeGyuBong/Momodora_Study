#pragma once

class CMath
{
public:
	CMath();
	~CMath();

public:
	// �� ������Ʈ�� �Ÿ� ��ȯ
	static float CalculateDistance(D3DXVECTOR3 dstPos, D3DXVECTOR3 srcPos);
	// �� ������Ʈ�� �Ÿ� X��ǥ ���� ��ȯ
	static float CalculateDistanceX(D3DXVECTOR3 dstPos, D3DXVECTOR3 srcPos);

	// �� ������Ʈ�� ���� ��ȯ
	static float CalculateDegree(INFO dstInfo, INFO srcInfo);

	// ������Ʈ ��� ���
	static D3DXMATRIX CalculateMatrix(D3DXVECTOR3 pos, D3DXVECTOR3 scale = { 1.f,1.f,0.f }, ROTATION eType = ROTATION::Z, float degree = 0.f);
	
	// ������Ʈ�� �ִ� ��ġ�� �׸����� �ε��� ���
	static int CalculateGreedIndex(D3DXVECTOR3 vPos);
};

