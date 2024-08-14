#pragma once

class CObj;
class CCollisionMgr
{
public:
	CCollisionMgr();
	~CCollisionMgr();

public:
	// �ǰ� üũ�� ���� �簢�� �浹
	static void CollisionRect(OBJECT::TYPE eTypeDst, OBJECT::TYPE eTypeSrc);
	// �ٴ� �浹�� �о�� �Լ�
	static void CollisionRectEX(OBJECT::TYPE eType);
	// �� �þ� ���� �浹 üũ
	static void CollisionEnemyView();
	// ������Ʈ�� �ٴ� �浹 �Լ�
	static void CollisionFloor(OBJECT::TYPE eType);
	// �÷��̾� �������� �浹 �Լ�
	static void CollisionPlayerAttack(OBJECT::TYPE eType);
	// �� �������� �浹 �Լ�
	static void CollisionEnemyAttack(OBJECT::TYPE eType);

	static void GreedLstReset() { pCheckLst.clear(); }

	// ������Ʈ�� �ִ� �׸��� ���� �� 9���� �׸��忡 
	// ������Ʈ Rect�� ���Ե� �ִ��� ���� ����
	static void CheckTrueGreed();
	// �˻��� �׸����� Ÿ�� �����͸� �����ִ� �Լ�
	static void GreedTileDataInit();
	// �˻��� �׸����� ������Ʈ �����͸� �����ִ� �Լ�
	static void GreedObjectDataInit();

	static void ColliInteractObj();
	
private:
	// �� �浹
	static bool CheckCircle(CObj*& pDst, CObj*& pSrc);
	// �簢�� �浹
	// �浹 ���� ��ȯ
	static bool CheckRect(CObj*& pDst, TILE*& pSrc, float & moveX, float & moveY);
	static bool CheckRect(CObj *& pDst, TILE *& pSrc);
	// �� - �簢�� �浹 
	// pDst : �簢��(��ü����) pSrc : ��(�þ�)
	static bool CollisionCircleToRect(CObj *& pDst, CObj *& pSrc);
	// ��- �� �浹
	static bool CollisionCircleToPoint(D3DXVECTOR2 DstPos, D3DXVECTOR2 srcPos, float radius);
	

private:
	// �׸��� ����Ʈ
	static list<GREED*> pCheckLst;
};

