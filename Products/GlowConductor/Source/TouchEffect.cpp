#include "TouchEffect.h"
#include "CustomParticle.h"
#include "SGraphMgr.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;

//**************************************************************************************************
// タッチ時のパーティクルを出す処理
//**************************************************************************************************
void TouchEffect(Node* node, const Point& createPoint)
{
	// ■パーティクルを出す
	ParticleSystemQuad* particle = MyParticles::TouchParticle::create();
	particle->setTexture(SGraphMgr::Instance()->GetBatchTexture("rectangle"));
	particle->setPosition(createPoint);
	particle->setAutoRemoveOnFinish(true);
	node->addChild(particle, 500);

	// リングのパーティクルを出す
	Sprite* pTouchCircle = Sprite::createWithTexture(SGraphMgr::Instance()->GetBatchTexture("particle_normal_ring"));
	pTouchCircle->setScale(0.01f);
	pTouchCircle->runAction(Sequence::createWithTwoActions(
		Spawn::createWithTwoActions(ScaleTo::create(0.5f, 1.0f), FadeOut::create(0.5f)),
		RemoveSelf::create()));
	pTouchCircle->setPosition(createPoint);
	node->addChild(pTouchCircle, 500);

	// サウンドを発生させる
	SimpleAudioEngine::getInstance()->playEffect("sound/touch_se.mp3");
}