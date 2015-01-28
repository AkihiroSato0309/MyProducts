#include "CameraTarget.h"
USING_NS_CC;

//**************************************************************************************************
// コンストラクタ
//**************************************************************************************************
CameraTarget::CameraTarget()
{
	_movedX = 0.0f;
}



//**************************************************************************************************
// 更新処理
//**************************************************************************************************
void CameraTarget::Update(const Point& playerPoint)
{
	// スクロールでプレイヤーの右側を開けるための変数
	float spaceLeft = Director::getInstance()->getWinSize().width / 3;


	// ターゲットを更新
	float moveX = 3.0f;	// ターゲットが進む距離
	float moveY = (playerPoint.y - this->getPosition().y) * 0.05f;
	this->setPosition(this->getPosition() + Point(moveX, moveY));
	

	// 総移動量を足す
	_movedX = moveX;	


	// 強制スクロールが追い付かない場合、ターゲットをプレイヤーに寄せる
	if (this->getPosition().x <= playerPoint.x + spaceLeft)
	{
		moveX = (playerPoint.x + spaceLeft - this->getPosition().x) * 0.005f;
		_movedX += moveX;
		this->setPosition(this->getPosition() + Point(moveX, this->getPosition().y));
	}
}



//**************************************************************************************************
// クリエイト関数
//**************************************************************************************************
CameraTarget* CameraTarget::createWithTexture(Texture2D *texture)
{
	CameraTarget *sprite = new CameraTarget();
	if (sprite && sprite->initWithTexture(texture))
	{
		sprite->setOpacity(0);
		sprite->autorelease();
		sprite->setPosition(Director::getInstance()->getWinSize().width / 2, Director::getInstance()->getWinSize().height / 2);
		return sprite;

	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}