#include "GameScene.h"
#include "CPlayer.h"
#include "CDark.h"
#include "Collision.h"
#include "ObjectMgr.h"
#include "CustomFollow.h"
#include "ObjectFactory.h"
#include "SGraphMgr.h"
#include "GameBackLayer.h"
#include "ResultScene.h"
#include "SDataMgr.h"
#include "SCameraTargetMgr.h"
#include "CustomParticle.h"
#include "GameLayer.h"
#include "CustomPointEffect.h"
#include "GameBackEffectLayer.h"
#include "TouchEffect.h"
#include "SimpleAudioEngine.h"
#include "TitleScene.h"

USING_NS_CC;
using namespace CocosDenshion;
using namespace cocos2d::ui;


// 仮のタグ定義
static const int PLAYER_TAG = 1;
static const int TARGET_CAMERA_TAG = 2;
static const int TOUCH_NUM_TAG = 3;
static const int TOUCH_BACK_TAG = 4;
static const int OBJ_FACTORY_TAG = 7;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = GameScene::create();

	scene->addChild(layer);

    // return the scene
    return scene;
}


//***************************************************************************************
// 初期化処理
//***************************************************************************************
bool GameScene::init()
{

	// スーパークラスの初期化を呼ぶ
    if ( !Layer::init())
    {
        return false;
    }

	_singleTouchFlag = false;

	// 画面の大きさを取得
	Size winSize = Director::getInstance()->getVisibleSize();

	//------------------------------------------------------------
	// BGM、SEの再生
	//------------------------------------------------------------
	SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/game_bgm.mp3", true);
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.7f);


	//------------------------------------------------------------
	// 使用画像の読み込み
	//------------------------------------------------------------
	SGraphMgr::Instance()->SetUsingGraph(eScene_Game);
	SGraphMgr::Instance()->AddUsingScene(this);


	//------------------------------------------------------------
	// 始めのステージを設定する
	//------------------------------------------------------------
	SStageMgr::Instance()->Initialize(SStageMgr::eStageNum::eStage_0);


	//------------------------------------------------------------
	// 各レイヤーを初期化する
	//------------------------------------------------------------
	_pGameLayer = GameLayer::create();
	_pBackLayer = GameBackLayer::create();
	_pUiLayer = Layer::create();
	auto pBackEffectLayer = GameBackEffectLayer::create();
	auto pEdgeParticleLayer = Layer::create();										// 上下の端のパーティクルを出すレイヤー
	auto pBackHexagonLayer = LayerColor::create(Color4B(0.0f, 0.0f, 0.0f, 0.0f));	// 後ろから出てくる6角形を表示するレイヤー

	// 六角形パーティクルの作成
	auto pHexagonParticle = ParticleSystemQuad::create(FileUtils::getInstance()->fullPathForFilename("particle/effectHexagon/particleTexture.plist"));
	pHexagonParticle->setPosition(-100.0f, Director::getInstance()->getVisibleSize().height * 0.5f);
	pBackHexagonLayer->setBlendFunc(BlendFunc::ADDITIVE);
	pBackHexagonLayer->addChild(pHexagonParticle, 0);

	// 画面上下端につけるエフェクトを作成
	auto pEdgeParticle_up = ParticleSystemQuad::create(FileUtils::getInstance()->fullPathForFilename("particle/effectWindowEdge/particleTexture.plist"));
	pEdgeParticle_up->setPosition(winSize.width * 0.5f, winSize.height);
	pEdgeParticleLayer->addChild(pEdgeParticle_up, 0);

	auto pEdgeParticle_down = ParticleSystemQuad::create(FileUtils::getInstance()->fullPathForFilename("particle/effectWindowEdge/particleTexture.plist"));
	pEdgeParticle_down->setPosition(winSize.width * 0.5f, 0.0f);

	// 重力方向を反転させる
	pEdgeParticle_down->setGravity(Point(pEdgeParticle_down->getGravity().x, pEdgeParticle_down->getGravity().y * -1.0f));
	pEdgeParticleLayer->addChild(pEdgeParticle_down, 0);
	
	// 作ったレイヤーを順番に追加する
	this->addChild(_pBackLayer, 0);
	this->addChild(pBackHexagonLayer, 1);
	this->addChild(pBackEffectLayer, 2);
	this->addChild(pEdgeParticleLayer, 3);
	this->addChild(_pGameLayer, 4);
	// ステージ切り替え時に色が変わるレイヤーを追加
	SStageMgr::Instance()->AddBlendLayer(this, 5);
	this->addChild(_pUiLayer, 6);

	//------------------------------------------------------------
	// タッチモードを設定する
	//------------------------------------------------------------
	this->setTouchEnabled(true);
	this->setTouchMode(kCCTouchesOneByOne);


	//------------------------------------------------------------
	// 更新処理を許可する
	//------------------------------------------------------------
	this->scheduleUpdate();


	//------------------------------------------------------------
	// フラグの初期化
	//------------------------------------------------------------
	_gameEndFlag = false;
	_gameStopFlag = false;

	//------------------------------------------------------------
	// カメラターゲットを設定
	//------------------------------------------------------------
	// 自機を画面の中心へセット
	SCameraTargetMgr::Instance()->SetCameraTarget(CameraTarget::createWithTexture(SGraphMgr::Instance()->GetSpriteBatch("ball")->getTexture()));
	_pGameLayer->addChild(TargetPointer);


	//------------------------------------------------------------
	// 障害物オブジェクト生成関数を生成する
	//------------------------------------------------------------
	ObjectFactory* objFactory = ObjectFactory::create();
	objFactory->SetNode(_pGameLayer);
	objFactory->setTag(OBJ_FACTORY_TAG);
	objFactory->InitObjectData();
	_pGameLayer->addChild(objFactory, 1);


	//------------------------------------------------------------
	// オブジェクト管理変数を生成
	//------------------------------------------------------------
	objectMgr = ObjectMgr::create();
	objectMgr->InitObjMgr();
	this->addChild(objectMgr);


	//------------------------------------------------------------
	// プレイヤー生成
	//------------------------------------------------------------
	CPlayer* pPlayer = CPlayer::createWithTexture(SGraphMgr::Instance()->GetSpriteBatch("ball")->getTexture());
	pPlayer->setPosition(Point(pPlayer->GetRadius(), Director::getInstance()->getWinSize().height * 0.5f));
	pPlayer->setTag(PLAYER_TAG);
	objectMgr->SetPlayerStateChanger(pPlayer);
	_pGameLayer->addChild(pPlayer, 2);
	objectMgr->RegistObject(0, pPlayer);


	//------------------------------------------------------------
	// スコアを表示させる
	//------------------------------------------------------------
	pPlayer->ShowState(_pUiLayer);


	//------------------------------------------------------------
	// カメラをターゲットに追従させる
	//------------------------------------------------------------
	CustomFollow* actFollow = CustomFollow::create(TargetPointer);
	_pGameLayer->runAction(actFollow);
    
	//------------------------------------------------------------
	// メニューボタン作成
	//------------------------------------------------------------
	auto pNormalSprite = Sprite::create("graph/menu_button.png");
	auto pPushedSprite = Sprite::create("graph/menu_button.png");
	pPushedSprite->setColor(Color3B(100, 100, 100));
	auto pButtonItem = MenuItemSprite::create(pNormalSprite, pPushedSprite, CC_CALLBACK_1(GameScene::AwakeMenu, this));
	auto pMenu = Menu::create(pButtonItem, NULL);
	pMenu->setPosition(Point(128, 128));

	this->addChild(pMenu, 1000);
    return true;
}


//***************************************************************************************
// メニュー起動
//***************************************************************************************
void GameScene::AwakeMenu(Ref* pSender)
{
	
	Director::getInstance()->pause();

	auto winSize = Director::getInstance()->getWinSize();

	auto pDialog = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("MenuDialog/MenuDialog.json");
	this->addChild(pDialog, 1000);

	pDialog->setPosition(Point(winSize.width * 0.5f, winSize.height * 0.5f));
	pDialog->setAnchorPoint(Point(0.5f, 0.5f));

	auto pToRetry = dynamic_cast<ui::Widget*>(pDialog->getChildByName("ToRetry"));
	pToRetry->addTouchEventListener(this, toucheventselector(GameScene::ToRetry));

	auto pToBack = dynamic_cast<ui::Widget*>(pDialog->getChildByName("ToBack"));
	pToBack->addTouchEventListener(this, toucheventselector(GameScene::ToBack));

	auto pToTitle = dynamic_cast<ui::Widget*>(pDialog->getChildByName("ToTitle"));
	pToTitle->addTouchEventListener(this, toucheventselector(GameScene::ToTitle));

	this->setTouchEnabled(false);
}


//***************************************************************************************
// タッチ開始時
//***************************************************************************************
bool GameScene::onTouchBegan(Touch* touch, Event* unused_event)
{
	// タッチフラグが立っていたら、通さない
	if (_singleTouchFlag == true) return false;

	// 各ノードを取得
	CPlayer* pPlayer = (CPlayer*)_pGameLayer->getChildByTag(PLAYER_TAG);

	if (pPlayer->GetTouchRemain() <= 0) return true;

	// 画面移動に伴うタップ位置のずれを補正し、プレイヤーに渡す
	Point hosei = Point(TargetPointer->getPosition().x - Director::getInstance()->getWinSize().width * 0.5f, 0);
	pPlayer->SetTouchPoint(touch->getLocation() + hosei);

	// タッチ残り回数を減らす
	pPlayer->DownTouch(1);

	// タッチパーティクルを出す
	TouchEffect(_pGameLayer, touch->getLocation() + hosei);

	// シングルタッチフラグを立てる
	_singleTouchFlag = true;

	return false;
}


//***************************************************************************************
// 更新処理
//***************************************************************************************
void GameScene::update(float time)
{
	// 各ノードをレイヤーより取得する
	CPlayer* pPlayer = (CPlayer*)_pGameLayer->getChildByTag(PLAYER_TAG);
	ObjectFactory* objFactory = (ObjectFactory*)_pGameLayer->getChildByTag(OBJ_FACTORY_TAG);

	// プレイヤーが死んでいない場合、ターゲットを更新
	if(!pPlayer->GetKillFlag()) TargetPointer->Update(pPlayer->getPosition());

	// プレイヤー情報を更新
	pPlayer->Move();
	pPlayer->UpdateScore();
	pPlayer->UpdateTapRemain();

	// プレイヤーの位置によってオブジェクトを自動生成
	CObject* obj = objFactory->AutoCraeteObject(TargetPointer->getPosition());
	if (obj != nullptr)
	{
		objectMgr->RegistObject(1, obj);
	}

	// 衝突判定を行う
	objectMgr->CalcCollision();

	// オブジェクトマネージャの更新
	objectMgr->Update(TargetPointer->getPosition());

	// プレイヤーのリムーブフラグを取得し、フラグが立っていたらシーンを変更する
	if (pPlayer->GetRemoveFlag())
	{
		SDataMgr::Instance()->SetScore(pPlayer->GetScore());
		ChangeSceneResult();
		this->unscheduleUpdate();
	}

	// タッチフラグをfalseにする
	_singleTouchFlag = false;
}


//***************************************************************************************
// リザルトシーンへ移行
//***************************************************************************************
void GameScene::ChangeSceneResult()
{
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, ResultScene::createScene()));
}


//***************************************************************************************
// メニュー項目
//***************************************************************************************
void GameScene::ToTitle(Ref* sender, ui::TouchEventType type)
{
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, TitleScene::createScene()));
	dynamic_cast<Node*>(sender)->getParent()->removeFromParentAndCleanup(true);
	Director::getInstance()->resume();
	this->unscheduleUpdate();
}
void GameScene::ToRetry(Ref* sender, ui::TouchEventType type)
{
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, GameScene::createScene()));
	dynamic_cast<Node*>(sender)->getParent()->removeFromParentAndCleanup(true);
	Director::getInstance()->resume();
	this->unscheduleUpdate();
}
void GameScene::ToBack(Ref* sender, ui::TouchEventType type)
{
	dynamic_cast<Node*>(sender)->getParent()->removeFromParentAndCleanup(true);
	this->setTouchEnabled(true);
	Director::getInstance()->resume();

}