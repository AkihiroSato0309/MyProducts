#include "FakeNode.h"
#include "SCameraTargetMgr.h"
USING_NS_CC;

bool FakeNode::init()
{
	// スーパークラスの初期化を呼ぶ
	Node::init();

	this->scheduleUpdate();
	return true;
}

void FakeNode::update(float delta)
{
	if (TargetPointer->getPositionX() - Director::getInstance()->getWinSize().width > this->getPositionX())
		this->removeFromParentAndCleanup(true);
}
