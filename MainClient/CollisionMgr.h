#pragma once

class CObj;
class CCollisionMgr
{
public:
	CCollisionMgr();
	~CCollisionMgr();

public:
	// 피격 체크를 위한 사각형 충돌
	static void CollisionRect(OBJECT::TYPE eTypeDst, OBJECT::TYPE eTypeSrc);
	// 바닥 충돌시 밀어내기 함수
	static void CollisionRectEX(OBJECT::TYPE eType);
	// 적 시야 범위 충돌 체크
	static void CollisionEnemyView();
	// 오브젝트와 바닥 충돌 함수
	static void CollisionFloor(OBJECT::TYPE eType);
	// 플레이어 근접공격 충돌 함수
	static void CollisionPlayerAttack(OBJECT::TYPE eType);
	// 적 근접공격 충돌 함수
	static void CollisionEnemyAttack(OBJECT::TYPE eType);

	static void GreedLstReset() { pCheckLst.clear(); }

	// 오브젝트가 있는 그리드 포함 총 9개의 그리드에 
	// 오브젝트 Rect가 포함되 있는지 여부 조사
	static void CheckTrueGreed();
	// 검사할 그리드의 타일 데이터를 정해주는 함수
	static void GreedTileDataInit();
	// 검사할 그리드의 오브젝트 데이터를 정해주는 함수
	static void GreedObjectDataInit();

	static void ColliInteractObj();
	
private:
	// 원 충돌
	static bool CheckCircle(CObj*& pDst, CObj*& pSrc);
	// 사각형 충돌
	// 충돌 범위 반환
	static bool CheckRect(CObj*& pDst, TILE*& pSrc, float & moveX, float & moveY);
	static bool CheckRect(CObj *& pDst, TILE *& pSrc);
	// 원 - 사각형 충돌 
	// pDst : 사각형(객체범위) pSrc : 원(시야)
	static bool CollisionCircleToRect(CObj *& pDst, CObj *& pSrc);
	// 원- 점 충돌
	static bool CollisionCircleToPoint(D3DXVECTOR2 DstPos, D3DXVECTOR2 srcPos, float radius);
	

private:
	// 그리드 리스트
	static list<GREED*> pCheckLst;
};

