#include "CTouchUp.h"
#include "CalclateMcr.h"
#include "CPlayer.h"
#include "SGraphMgr.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

//****************************************************************
// コンストラクタ
//****************************************************************
CTouchUp::CTouchUp()
{

}


//****************************************************************
// デストラクタ
//****************************************************************
CTouchUp::~CTouchUp()
{

}



//****************************************************************
// 衝突時に実行される関数
//****************************************************************
void CTouchUp::CollisionFunction(IPlayerStateChange* playerStateChanger)
{

	// 消失パーティクルを作成
	auto pParticle = ParticleExplosion::createWithTotalParticles(70);
	pParticle->setPosition(this->getPosition());
	pParticle->setPositionType(ParticleSystem::PositionType::RELATIVE);
	pParticle->setTexture(SGraphMgr::Instance()->GetBatchTexture("rectangle"));

	pParticle->setLife(1.0f);
	pParticle->setLifeVar(0.3f);

	pParticle->setSpeed(1000.0f * this->getScale());

	pParticle->setStartColor(Color4F(1.0f, 1.0f, 0.0f, 1.0f));
	pParticle->setStartSize(GetRadius() * 0.3f);

	pParticle->setStartSpin(600.0f);
	pParticle->setEndSpin(200.0f);

	pParticle->setAutoRemoveOnFinish(true);

	// 音を鳴らす
	SimpleAudioEngine::getInstance()->playEffect("sound/touch_up_se.mp3");

	// 生成したパーティクルを親ノードにくっつける
	this->getParent()->addChild(pParticle);

	playerStateChanger->AddTouch(5);
	_hitDeleteFlag = true;
}

//**********************************************************
// クリエイト関数
//**********************************************************
CTouchUp* CTouchUp::createWithTexture(Texture2D *texture)
{
	CTouchUp *sprite = new CTouchUp();
	if (sprite && sprite->initWithTexture(texture))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}