#include "ActionFactory.h"
USING_NS_CC;

//****************************************************************
// 上下移動のアクション
//****************************************************************
void ActionFactory::CreateMoveVartical(Node* node, float amplitude, float period, bool startPrace)
{
	// 上下に移動するアクションをそれぞれ作成する
	ActionInterval* pMoveUp = EaseInOut::create(MoveBy::create(period, Point(0.0f, amplitude)), 2.0f);
	ActionInterval* pMoveDown = EaseInOut::create(MoveBy::create(period, Point(0.0f, -amplitude)), 2.0f);

	// アクションを組み合わせる
	ActionInterval* pMoveVartical = nullptr;
	// スタート位置によって、オブジェクトの位置をずらす
	if (startPrace == START_UP)
	{
		node->setPositionY(node->getPositionY() + amplitude / 2.0f);
		pMoveVartical = Sequence::createWithTwoActions(pMoveDown, pMoveUp);
	}
	if (startPrace == START_DOWN)
	{
		node->setPositionY(node->getPositionY() - amplitude / 2.0f);
		pMoveVartical = Sequence::createWithTwoActions(pMoveUp, pMoveDown);
	}

	// リピートアクションを作成する
	ActionInterval* pMoveRepeat = RepeatForever::create(pMoveVartical);

	node->runAction(pMoveRepeat);
}



//****************************************************************
// 左右移動のアクション
//****************************************************************
void ActionFactory::CreateMoveHorizontal(Node* node, float amplitude, float period, bool startPrace)
{
	ActionInterval* pMoveRight = EaseInOut::create(MoveBy::create(period, Point(amplitude, 0.0f)), 2.0f);
	ActionInterval* pMoveLeft = EaseInOut::create(MoveBy::create(period, Point(-amplitude, 0.0f)), 2.0f);

	ActionInterval* pMoveHorizontal = nullptr;
	
	if (startPrace == START_RIGHT)
	{
		node->setPositionX(node->getPositionX() + amplitude / 2.0f);
		pMoveHorizontal = Sequence::createWithTwoActions(pMoveLeft, pMoveRight);
	}
	if (startPrace == START_LEFT)
	{
		node->setPositionX(node->getPositionX() - amplitude / 2.0f);
		pMoveHorizontal = Sequence::createWithTwoActions(pMoveRight, pMoveLeft);
	}

	node->runAction(RepeatForever::create(pMoveHorizontal));
}



//****************************************************************
// 上からきて止まる
//****************************************************************
void ActionFactory::CreateMoveToDown(Node* node, float distance, float duration)
{
	node->runAction(MoveBy::create(duration, Point(0, -distance)));
}



//****************************************************************
// 下からきて止まる
//****************************************************************
void ActionFactory::CreateMoveToUp(Node* node, float distance, float duration)
{
	node->runAction(MoveBy::create(duration, Point(0, distance)));
}



//****************************************************************
// 収縮を繰り返す
//****************************************************************
void ActionFactory::CreateScaleChange(Node* node, float maxScale, float smallScale, float period, bool startScale)
{
	float halfPeriod = period * 0.5f;
	ActionInterval* bigAction = EaseInOut::create(ScaleTo::create(halfPeriod, maxScale), 2.0f);
	ActionInterval* smallAction = EaseInOut::create(ScaleTo::create(halfPeriod, smallScale), 2.0f);

	ActionInterval* pRunAction = nullptr;

	if (startScale == START_SMALL)
	{
		pRunAction = Sequence::create(smallAction, bigAction, nullptr);
	}
	else
	{
		pRunAction = Sequence::create(bigAction, smallAction, nullptr);
	}

	node->runAction(RepeatForever::create(pRunAction));

}