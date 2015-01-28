#pragma once

#include "cocos2d.h"

class BaseAI
{
public:
	virtual void AI(cocos2d::Node* pObject) = 0;
};
