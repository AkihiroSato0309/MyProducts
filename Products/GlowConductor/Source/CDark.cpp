#include "CDark.h"
#include "CalclateMcr.h"
#include "CPlayer.h"
#include "CustomParticle.h"

USING_NS_CC;

//**************************************************************************************************
// コンストラクタ
//**************************************************************************************************
CDark::CDark()
{
}

CDark::CDark(float radius, float x, float y) : CObject(radius, x, y)
{

}



//**************************************************************************************************
// デストラクタ
//**************************************************************************************************
CDark::~CDark()
{

}



//**************************************************************************************************
// プレイヤーと衝突時に実行される関数
//**************************************************************************************************
void CDark::CollisionFunction(IPlayerStateChange* playerStateChanger)
{
	playerStateChanger->KillPlayer();
	_hitDeleteFlag = false;
}

//**************************************************************************************************
// クリエイト関数
//**************************************************************************************************
CDark* CDark::createWithTexture(Texture2D *texture)
{
	CDark *sprite = new CDark();
	if (sprite && sprite->initWithTexture(texture))
	{
		sprite->autorelease();
		sprite->setBlendFunc(BlendFunc::ADDITIVE);
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}