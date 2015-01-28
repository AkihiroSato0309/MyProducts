#pragma once

class CObject;

// 円と円のあたり判定
bool CollisionCircle(	float radius1, float x1, float y1,
						float radius2, float x2, float y2);
bool CollisionCircle(CObject& obj1, CObject& obj2);
