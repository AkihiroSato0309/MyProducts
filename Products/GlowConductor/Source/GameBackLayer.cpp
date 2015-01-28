#include "GameBackLayer.h"
#include "SCameraTargetMgr.h"

USING_NS_CC;

//***************************************************************************
// 初期化処理
//***************************************************************************
bool GameBackLayer::init()
{
    // スーパークラスの初期化を呼ぶ
	if (!LayerColor::initWithColor(Color4B(0, 10, 100, 255)))
    {
        return false;
    }

	this->setTouchEnabled(false);

    return true;
}



//***************************************************************************
// 更新処理
//***************************************************************************
void GameBackLayer::Update()
{
	// カメラの中心位置に持ってくる
	this->setPosition(TargetPointer->getPosition());

}
