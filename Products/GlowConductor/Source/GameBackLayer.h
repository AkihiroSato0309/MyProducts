#pragma once

#include "cocos2d.h"

class GameBackLayer : public cocos2d::LayerColor
{
public:

    virtual bool init();  

	CREATE_FUNC(GameBackLayer);

	// 更新処理
	void Update();

private:

};