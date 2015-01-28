#pragma once

#include "CObject.h"

//***************************************************************************************
// 闇の渦クラス
//***************************************************************************************
class CDark : public CObject
{
public:
	CDark();
	CDark(float radius, float x, float y);
	~CDark();

	// 移動アルゴリズム
	void Update(){}

	// 衝突時に実行される関数
	void CollisionFunction(IPlayerStateChange* playerStateChanger);

	// クリエイト関数
	CREATE_FUNC(CDark);
	static CDark* createWithTexture(cocos2d::Texture2D *texture);

private:
	
	
};

