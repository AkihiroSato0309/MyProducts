#include "TitleScene.h"
#include "GameScene.h"
#include "SGraphMgr.h"
#include "GameBackEffectLayer.h"
#include "GameBackLayer.h"
#include "SCameraTargetMgr.h"
#include "TouchEffect.h"
#include "SimpleAudioEngine.h"

using namespace std;
using namespace CocosDenshion;
using namespace cocostudio;
using namespace cocos2d::ui;

USING_NS_CC;

Scene* TitleScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = TitleScene::create();

    scene->addChild(layer);

    return scene;
}

//************************************************************
// 初期化処理
//************************************************************
bool TitleScene::init()
{
	// スーパークラスの初期化を呼ぶ
    if ( !Layer::init() )
    {
        return false;
    }

	// シングルタッチフラグを倒す
	_singleTouchFlag = false;
    
	// 画像を使えるようにする
	SGraphMgr::Instance()->SetUsingGraph(eScene_title);
	SGraphMgr::Instance()->AddUsingScene(this);

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

	//--------------------------------------------------------------------
	// このゲームで使用するBGM,SEをすべて読み込む
	//--------------------------------------------------------------------
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sound/title_bgm.mp3");
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sound/game_bgm.mp3");
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sound/result_bgm.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/button_se.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/touch_up_se.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/disappear_se.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/touch_se.mp3");
	

	// BGMを再生する
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/title_bgm.mp3", true);


	// タッチモードを設定
	this->setTouchEnabled(true);
	this->setTouchMode(kCCTouchesOneByOne);
	
	// キー入力を設定
	this->setKeyboardEnabled(true);

	// 更新処理をつける
	this->scheduleUpdate();

	//--------------------------------------------------------------------
	// 保存するスコアを初期化
	//-------------------------------------------------------------------
	std::string scoreString;
	UserDefault* pUserDefault = UserDefault::getInstance();
	pUserDefault->destroyInstance();

	for (int i = 0; i < 5; i++)
	{
		scoreString = String::createWithFormat("%d", i)->getCString();

		// データがなかったら、データを初期化
		if (pUserDefault->getIntegerForKey(scoreString.c_str(), 0) == 0)
		{
			pUserDefault->setIntegerForKey(scoreString.c_str(), 0);
			pUserDefault->flush();
		}
	}
	

	//--------------------------------------------------------------------
	// スタートボタン作成
	//--------------------------------------------------------------------
	Sprite* pStartButton = Sprite::createWithTexture(SGraphMgr::Instance()->GetBatchTexture("title_start_button"));
	Sprite* pStartButtonTap = Sprite::createWithTexture(SGraphMgr::Instance()->GetBatchTexture("title_start_button"));

	pStartButtonTap->setAnchorPoint(Point(0.5f, 0.5f));
	pStartButtonTap->setColor(Color3B(120.0f, 120.0f, 120.0f));
	pStartButtonTap->setScale(0.9f);

	_pStartItem = MenuItemSprite::create(
		pStartButton,
		pStartButtonTap,
		(Ref*)this,
		menu_selector(TitleScene::TapStartbutton));

	Menu* pStartMenu = Menu::create(_pStartItem, NULL); // NULLを忘れては動かない
	pStartMenu->setPosition(
		Point(Director::getInstance()->getWinSize().width / 2, 
		Director::getInstance()->getWinSize().height / 5));

	// スタートボタンにアクション設定
	_pStartItem->setRotation(-3.0f);
	ActionInterval* pFlashAction = Sequence::create(
		EaseInOut::create(FadeOut::create(1.0f), 2.0f),
		EaseInOut::create(FadeIn::create(1.0f), 2.0f),
		nullptr);
	
	_pStartItem->runAction(RepeatForever::create(pFlashAction));

	// タイトルレイヤーに追加
	this->addChild(pStartMenu, 3);


	//--------------------------------------------------------------------
	// タイトル文字表示
	//--------------------------------------------------------------------
	Sprite* pTitleSprite_1 = Sprite::createWithTexture(SGraphMgr::Instance()->GetBatchTexture("title_name_1"));
	Sprite* pTitleSprite_2 = Sprite::createWithTexture(SGraphMgr::Instance()->GetBatchTexture("title_name_2"));
	Sprite* pTitleWordBack = Sprite::createWithTexture(SGraphMgr::Instance()->GetBatchTexture("title_word_back"));


	pTitleSprite_1->setPosition(visibleSize.width * 0.5f, Director::getInstance()->getWinSize().height / 5 * 4);
	pTitleSprite_2->setPosition(visibleSize.width * 0.5f, Director::getInstance()->getWinSize().height / 7 * 4);
	pTitleWordBack->setPosition(visibleSize.width * 0.5f, (pTitleSprite_1->getPositionY() + pTitleSprite_2->getPositionY()) * 0.5f - 50.0f);

	// タイトル文字に、色を変えるアクションをつける
	ActionInterval* pColorChange_1 = EaseInOut::create(TintTo::create(1.0f, 255, 255, 255), 2.0f);
	ActionInterval* pColorChange_2 = EaseInOut::create(TintTo::create(1.0f, 20, 240, 100), 2.0f);
	ActionInterval* pColorChange = RepeatForever::create(
		Sequence::create(
		pColorChange_1,
		pColorChange_2,
		nullptr));

	ActionInterval* pColorChangeCopy = pColorChange->clone();
	pTitleSprite_1->runAction(pColorChange);
	pTitleSprite_2->runAction(pColorChangeCopy);

	// タイトル文字背景にアクションをつける
	ActionInterval* fadeIn =  EaseInOut::create(FadeIn::create(1.7f), 2.0f);
	ActionInterval* fadeOut = EaseInOut::create(FadeOut::create(1.7f), 2.0f);
	ActionInterval* fadeRepeart = RepeatForever::create(Sequence::createWithTwoActions(fadeIn, fadeOut));
	pTitleWordBack->runAction(fadeRepeart);
	pTitleWordBack->setOpacity(180);
	pTitleWordBack->setBlendFunc(BlendFunc::ALPHA_NON_PREMULTIPLIED);

	// タイトルレイヤーに追加
	this->addChild(pTitleSprite_1, 4);
	this->addChild(pTitleSprite_2, 4);
	this->addChild(pTitleWordBack, 5);
	

	//--------------------------------------------------------------------
	// タイトル背景を表示
	//--------------------------------------------------------------------
	// レイヤーの生成
	auto pTitleBackLayer = GameBackLayer::create();
	auto pTitleBackEffectLayer = GameBackEffectLayer::create();

	// 背景エフェクトレイヤーの更新処理を止める
	pTitleBackEffectLayer->unscheduleUpdate();	

	// 背景に雲パーティクルを表示させる
	auto pTitleBackCloudLayer = LayerColor::create(Color4B(0.0f, 0.0f, 0.0f, 0.0f));
	BlendFunc cloudBlend;
	cloudBlend.dst = GL_ONE_MINUS_SRC_COLOR;
	cloudBlend.src = GL_ZERO;
	pTitleBackCloudLayer->setBlendFunc(cloudBlend);
	auto pCloudParticle = ParticleSystemQuad::create("particle/cloud/particleTexture.plist");

	pCloudParticle->setPosition(visibleSize.width * 0.5f, visibleSize.height * 0.5f);
	pTitleBackCloudLayer->addChild(pCloudParticle);

	// タイトルレイヤーに追加
	this->addChild(pTitleBackLayer, 0);
	this->addChild(pTitleBackCloudLayer, 1);
	this->addChild(pTitleBackEffectLayer, 2);

	// ダイアログポインタをヌルで初期化
	m_pDialog = nullptr;

    return true;
}


//************************************************************
// タッチ開始時
//************************************************************
bool TitleScene::onTouchBegan(Touch* touch, Event* unused_event)
{
	// シングルタッチフラグが立っていたら返す
	if (_singleTouchFlag == true) return false;

	// タッチエフェクトを出す
	TouchEffect(this, touch->getLocation());

	_singleTouchFlag = true;

	return false;
}


//************************************************************
// 更新処理
//************************************************************
void TitleScene::update(float delta)
{
	_singleTouchFlag = false;
}


//************************************************************
// スタートボタンをタップしたときに呼ばれる関数
//************************************************************
void TitleScene::TapStartbutton(Ref* sender)
{
	// 音を鳴らす
	SimpleAudioEngine::getInstance()->playEffect("sound/button_se.mp3");

	// ボタンに渡した関数を止める（ダブルタップ防止）
	_pStartItem->setCallback(nullptr);

	// シーンを変更
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, GameScene::createScene()));

}

//************************************************************
// イベントキーが押された時の処理
//************************************************************
void TitleScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* eve)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE && m_pDialog == nullptr)
	{
		m_pDialog = GUIReader::getInstance()->widgetFromJsonFile("EndDialog/EndDialog_1.json");
		this->addChild(m_pDialog, 1000);

		auto yesButton = dynamic_cast<ui::Widget*>(m_pDialog->getChildByName("Yes"));
		yesButton->addTouchEventListener(this, toucheventselector(TitleScene::PushDialogYes));

		auto noButton = dynamic_cast<ui::Widget*>(m_pDialog->getChildByName("No"));
		noButton->addTouchEventListener(this, toucheventselector(TitleScene::PushDialogNo));
	}
}

void TitleScene::PushDialogYes(Ref* sender, ui::TouchEventType type)
{
	Director::getInstance()->end();
}

void TitleScene::PushDialogNo(Ref* sender, ui::TouchEventType type)
{
	dynamic_cast<Node*>(sender)->getParent()->removeFromParentAndCleanup(true);
	m_pDialog = nullptr;
}