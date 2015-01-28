#pragma once

#include "cocos2d.h"

//***************************************************************************************
// 画像データを持たず、画面外に出た場合自動で消えるノード（回転アクション自などに使用）
//***************************************************************************************
class FakeNode : public cocos2d::Node
{
public:

	bool init() override;
	void update(float delta) override;
	CREATE_FUNC(FakeNode);

protected:

};