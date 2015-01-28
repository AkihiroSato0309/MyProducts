#include "CustomFollow.h"

USING_NS_CC;

// クリエイト関数
CustomFollow* CustomFollow::create(Node *followedNode, const Rect& rect)
{
	CustomFollow *follow = new CustomFollow();
	if (follow && follow->initWithTarget(followedNode, rect))
	{
		follow->autorelease();
		return follow;
	}
	CC_SAFE_DELETE(follow);
	return nullptr;
}

bool CustomFollow::initWithTarget(Node *followedNode, const Rect& rect)
{
	CCASSERT(followedNode != nullptr, "");

	followedNode->retain();
	_followedNode = followedNode;
	_worldRect = rect;
	if (rect.equals(Rect::ZERO))
	{
		_boundarySet = false;
	}
	else
	{
		_boundarySet = true;
	}

	_boundaryFullyCovered = false;

	Size winSize = Director::getInstance()->getWinSize();
	_fullScreenSize = Point(winSize.width, winSize.height);
	_halfScreenSize = _fullScreenSize * 0.5f;

	if (_boundarySet)
	{
		_leftBoundary = -((rect.origin.x + rect.size.width) - _fullScreenSize.x);
		_rightBoundary = -rect.origin.x;
		_topBoundary = -rect.origin.y;
		_bottomBoundary = -((rect.origin.y + rect.size.height) - _fullScreenSize.y);

		if (_rightBoundary < _leftBoundary)
		{
			_rightBoundary = _leftBoundary = (_leftBoundary + _rightBoundary) / 2;
		}
		if (_topBoundary < _bottomBoundary)
		{
			_topBoundary = _bottomBoundary = (_topBoundary + _bottomBoundary) / 2;
		}

		if ((_topBoundary == _bottomBoundary) && (_leftBoundary == _rightBoundary))
		{
			_boundaryFullyCovered = true;
		}
	}

	return true;
}

void CustomFollow::step(float dt)
{
	CC_UNUSED_PARAM(dt);

	if (_boundarySet)
	{
		if (_boundaryFullyCovered)
			return;

		Point tempPos = _halfScreenSize - _followedNode->getPosition();

		_target->setPosition(Point(clampf(tempPos.x, _leftBoundary, _rightBoundary),
			clampf(tempPos.y, _bottomBoundary, _topBoundary)));
	}
	else
	{
		_target->setPosition(Point(_halfScreenSize.x - _followedNode->getPosition().x, 0));
	}
}