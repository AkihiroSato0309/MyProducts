#include "GameBackEffectLayer.h"
#include "CustomParticle.h"
#include "SGraphMgr.h"
#include "SCameraTargetMgr.h"
USING_NS_CC;

//***************************************************************************
// 初期化処理
//***************************************************************************
bool GameBackEffectLayer::init()
{
	// スーパークラスの初期化を呼ぶ
	if (!Layer::init())
	{
		return false;
	}

	this->scheduleUpdate();
	this->setTouchEnabled(false);

	log("%d, %d", getPositionX(), getPositionY());
	this->setPosition(0.0f, 0.0f);

	// ○パーティクルを付け加える
	_pCircleStarParticle = MyParticles::FlashParticle::create();
	this->addChild(_pCircleStarParticle);

	// 星パーティクルを付け加える
	_pRightStarParticle = MyParticles::FlashParticle::create();
	_pRightStarParticle->setTexture(SGraphMgr::Instance()->GetBatchTexture("right_star"));
	_pRightStarParticle->setTotalParticles(50);
	this->addChild(_pRightStarParticle);

	return true;
}



//***************************************************************************
// 更新処理
//***************************************************************************
void GameBackEffectLayer::update(float delta)
{

	float moveLayerX = TargetPointer->GetMoveX();
	float position = this->getPositionX() - moveLayerX;
	this->setPositionX(position);

	_pCircleStarParticle->setPositionX(_pCircleStarParticle->getPositionX() + moveLayerX);
	_pRightStarParticle->setPositionX(_pRightStarParticle->getPositionX() + moveLayerX);
}