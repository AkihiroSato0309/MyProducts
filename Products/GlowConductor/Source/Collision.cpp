#include "Collision.h"
#include "CObject.h"

//**************************************************************************************************
// 円と円のあたり判定
//**************************************************************************************************
bool CollisionCircle(float radius1, float x1, float y1,
	float radius2, float x2, float y2)
{
	float distanceX = x2 - x1;
	float distanceY = y2 - y1;
	float sumRadius = radius1 + radius2;

	if (sumRadius * sumRadius > (distanceX * distanceX + distanceY * distanceY)) return true;

	return false;
}

bool CollisionCircle(CObject& obj1, CObject& obj2)
{
	return CollisionCircle(	obj1.GetRadius(), obj1.getPosition().x, obj1.getPosition().y,
							obj2.GetRadius(), obj2.getPosition().x, obj2.getPosition().y);
}
