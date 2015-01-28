#pragma once

#include "CObject.h"

//***************************************************************************************
// タッチ回数アップアイテム（シャボン玉）のクラス
//***************************************************************************************
class CTouchUp : public CObject
{
public:
	CTouchUp();
	~CTouchUp();

	// 更新アルゴリズム
	void Update(){}

	// オブジェクトにあたった時の処理
	void CollisionFunction(IPlayerStateChange* playerStateChanger);

	// クリエイト関数
	CREATE_FUNC(CTouchUp);
	static CTouchUp* createWithTexture(cocos2d::Texture2D *texture);

private:

};