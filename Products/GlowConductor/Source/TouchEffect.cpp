#include "TouchEffect.h"
#include "CustomParticle.h"
#include "SGraphMgr.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;

//**************************************************************************************************
// �^�b�`���̃p�[�e�B�N�����o������
//**************************************************************************************************
void TouchEffect(Node* node, const Point& createPoint)
{
	// ���p�[�e�B�N�����o��
	ParticleSystemQuad* particle = MyParticles::TouchParticle::create();
	particle->setTexture(SGraphMgr::Instance()->GetBatchTexture("rectangle"));
	particle->setPosition(createPoint);
	particle->setAutoRemoveOnFinish(true);
	node->addChild(particle, 500);

	// �����O�̃p�[�e�B�N�����o��
	Sprite* pTouchCircle = Sprite::createWithTexture(SGraphMgr::Instance()->GetBatchTexture("particle_normal_ring"));
	pTouchCircle->setScale(0.01f);
	pTouchCircle->runAction(Sequence::createWithTwoActions(
		Spawn::createWithTwoActions(ScaleTo::create(0.5f, 1.0f), FadeOut::create(0.5f)),
		RemoveSelf::create()));
	pTouchCircle->setPosition(createPoint);
	node->addChild(pTouchCircle, 500);

	// �T�E���h�𔭐�������
	SimpleAudioEngine::getInstance()->playEffect("sound/touch_se.mp3");
}