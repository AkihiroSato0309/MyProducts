#pragma once

#include "cocos2d.h"
//***************************************************************************************
// 自機に合わせて画面をスクロールさせるクラス
//***************************************************************************************
class CustomFollow : public cocos2d::Follow
{
public:
	CustomFollow(){}
	~CustomFollow(){}

	bool initWithTarget(cocos2d::Node *followedNode, const cocos2d::Rect& rect = cocos2d::Rect::ZERO);
	void step(float dt) override;
	static CustomFollow* create(cocos2d::Node *followedNode, const cocos2d::Rect& rect = cocos2d::Rect::ZERO);

private:

};
