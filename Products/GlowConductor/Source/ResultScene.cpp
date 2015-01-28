#include "ResultScene.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "SGraphMgr.h"
#include "SCameraTargetMgr.h"
#include "GameBackLayer.h"
#include "GameBackEffectLayer.h"
#include "SimpleAudioEngine.h"
#include "TouchEffect.h"

USING_NS_CC;
using namespace CocosDenshion;
using namespace std;

static const float RANK_SPACE_HEIGHT = 40.0f;			// ランキングの間隔
static const float RANK_BOTTOM_POS_Y = 80.0f;			// ランキングの一番下の位置
static const float CURRENT_SCORE_MOVE_COUNT = 1.5f;		// 現在スコアの動くカウント
static const float CURRENT_SCORE_DELAY = 1.0f;			// 現在スコアにかけるディレイタイム
static const float CURRENT_SOCRE_SCALE_COUNT = 1.0f;	// 現在スコアのスケールするカウント
static const float MENU_SHOW_COUNT = 
						CURRENT_SCORE_MOVE_COUNT +
						CURRENT_SCORE_DELAY +
						CURRENT_SOCRE_SCALE_COUNT;		// タイトル、リトライのボタンが出る時間

Scene* ResultScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = ResultScene::create();

    scene->addChild(layer);

    return scene;
}



//**************************************************************************************************
// 初期化処理
//**************************************************************************************************
bool ResultScene::init()
{
	// 基底クラスの初期化を読み込む
	if (!Layer::init())
	{
		return false;
	}

	_pThisLayer = this;

	// このシーンで使う画像を読み込む
	SGraphMgr::Instance()->SetUsingGraph(eScene_Result);
	SGraphMgr::Instance()->AddUsingScene(this);

	// 画面の大きさを取得
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	// ゲームシーンの音楽を止める
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	// BGMの再生
	SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/result_bgm.mp3", true);



	//--------------------------------------------------------------------
	// タイトル背景を表示
	//--------------------------------------------------------------------
	// レイヤーの生成
	auto pResultBackLayer = GameBackLayer::create();
	auto pResultBackEffectLayer = GameBackEffectLayer::create();

	// ゲームレイヤーエフェクトの更新処理を止める
	pResultBackEffectLayer->unscheduleUpdate();

	this->addChild(pResultBackLayer, 0);
	this->addChild(pResultBackEffectLayer, 1);


	//--------------------------------------------------------------------
	// リザルト文字を表示
	//--------------------------------------------------------------------
	auto pResultWord = Sprite::createWithTexture(SGraphMgr::Instance()->GetBatchTexture("result_word"));
	pResultWord->setAnchorPoint(Point(0.5f, 1.0f));
	pResultWord->setPosition(visibleSize.width * 0.5f, visibleSize.height + 10.0f);
	this->addChild(pResultWord, 1);
	

	//--------------------------------------------------------------------
	// ディレイをかけてタイトル、アゲインボタンを追加するアクション
	//--------------------------------------------------------------------
	auto pAddChildCallBack = CallFunc::create(this, callfunc_selector(ResultScene::AddButton));

	this->runAction(
		Sequence::createWithTwoActions(
		DelayTime::create(MENU_SHOW_COUNT),
		pAddChildCallBack));

	//--------------------------------------------------------------------
	// 点数を表示
	//--------------------------------------------------------------------
	// データを取得
	// 1を足しているのは、すでに記憶してあるデータ+今プレイした時のスコア
	_scoreData[SCORE_SAVE_MAX + 1];
	string scoreString;
	UserDefault* pUserDefault = UserDefault::getInstance();

	// 過去のスコアデータをとってくる
	for (int i = 0; i < SCORE_SAVE_MAX; i++)
	{
		scoreString = String::createWithFormat("%d", i)->getCString();

		_scoreData[i] = pUserDefault->getIntegerForKey(scoreString.c_str());

	}

	// 今回のプレイデータを保存する
	_scoreData[SCORE_SAVE_MAX] = SDataMgr::Instance()->GetScore();

	// 降順にソートする
	std::sort(_scoreData, _scoreData + SCORE_SAVE_MAX + 1, std::greater<int>());

	// 今回のゲームスコアをラベルにする
	TTFConfig ttfConfigEffect;
	ttfConfigEffect.distanceFieldEnabled = true;
	auto currentScoreString = String::createWithFormat("%d", SDataMgr::Instance()->GetScore());
	auto currentScoreLabel = Label::create(currentScoreString->getCString(), "fonts/HuiFontP29.ttf", 300.0f);
	currentScoreLabel->setAnchorPoint(Point(1.0f, 0.0f));
	// 下から出てくるため、初期位置を画面外に設定する
	Point labelPosition = Point(visibleSize.width * 0.5f + currentScoreLabel->getContentSize().width * 0.5f,
								visibleSize.height * 0.5f - currentScoreLabel->getContentSize().height * 0.5f);
	currentScoreLabel->setPosition(labelPosition.x, -visibleSize.height);
	this->addChild(currentScoreLabel, 1);

	// ランキングを表示する関数をアクションに組み込む
	auto pRankingShow = CallFunc::create(this, callfunc_selector(ResultScene::ShowRanking));

	// タッチを許可する関数をアクションに組み込む
	auto pTouchPermit = CallFunc::create(this, callfunc_selector(ResultScene::SetTouchPermit));

	// スコア、ランキングに関する一連の動作の登録
	currentScoreLabel->runAction(
		Sequence::create(
		EaseBackOut::create(MoveTo::create(CURRENT_SCORE_MOVE_COUNT, labelPosition)),
		DelayTime::create(CURRENT_SCORE_DELAY),
		Spawn::create(
		ScaleTo::create(CURRENT_SOCRE_SCALE_COUNT, 0.8f),
		EaseIn::create(MoveTo::create(1.0f, Point(visibleSize.width - 10.0f, 10.0f)), 3.0f),
		nullptr),
		pRankingShow,
		pTouchPermit,
		nullptr));

    return true;
}



//**************************************************************************************************
// タッチ開始時
//**************************************************************************************************
bool ResultScene::onTouchBegan(Touch* touch, Event* unused_event)
{
	// タッチパーティクルを出す
	TouchEffect(this, touch->getLocation());
	
	return false;
}



//**************************************************************************************************
// ランキングを表示する
//**************************************************************************************************
void ResultScene::ShowRanking()
{
	// 上位5つを表示,保存
	for (int i = 0; i < SCORE_SAVE_MAX; i++)
	{

		// スコアに背景を表示
		auto pScoreBack = Sprite::createWithTexture(SGraphMgr::Instance()->GetBatchTexture("score_back"));
		pScoreBack->setAnchorPoint(Point(0.5f, 0.5f));

		// 初期位置は画面の左に隠れている
		pScoreBack->setScale(0.9f);
		pScoreBack->setPosition(Point(
			-pScoreBack->getContentSize().width,
			RANK_BOTTOM_POS_Y + static_cast<float>(SCORE_SAVE_MAX - i - 1) * (pScoreBack->getContentSize().height + RANK_SPACE_HEIGHT) *  pScoreBack->getScale()));	// 画像間の幅

		// 横から出現するアクションをつける
		pScoreBack->runAction(
			Sequence::create(
			DelayTime::create(0.5f * static_cast<float>(i)),
			EaseOut::create(
			MoveTo::create(1.0f,
			Point(
			200.0f + static_cast<float>(SCORE_SAVE_MAX - i - 1) * 30.0f,
			pScoreBack->getPositionY())), 3.0f),
			nullptr));

		this->addChild(pScoreBack, 1);

		// スコアにランクをつける
		String* pScoreString = String::createWithFormat("%d. %d", i + 1, _scoreData[i]);
		auto pScoreLabel = Label::create(pScoreString->getCString(), "fonts/HuiFontP29.ttf", 150);

		pScoreLabel->setAnchorPoint(Point(0.0f, -0.1f));
		pScoreLabel->setPosition(Point(pScoreBack->getContentSize().width * 0.3f, 0.0f));
		pScoreBack->addChild(pScoreLabel, 1);

		// 保存
		string scoreString = String::createWithFormat("%d", i)->getCString();
		UserDefault::getInstance()->setIntegerForKey(scoreString.c_str(), _scoreData[i]);
	}
}



//**************************************************************************************************
// タッチを許可する関数
//**************************************************************************************************
void ResultScene::SetTouchPermit()
{
	this->setTouchEnabled(true);
	this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
}



//**************************************************************************************************
// タイトルシーンに戻る
//**************************************************************************************************
void ResultScene::ChangeSceneTitle(Object* pSender)
{
	// ボタンに渡した関数を止める（ダブルタップ防止）
	pTitleItem->setCallback(nullptr);

	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, TitleScene::createScene()));	
}



//**************************************************************************************************
// ゲームシーンに戻る
//**************************************************************************************************
void ResultScene::ChangeSceneGame(Object* pSender)
{
	// ボタンに渡した関数を止める（ダブルタップ防止）
	pGameItem->setCallback(nullptr);

	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, GameScene::createScene()));
}



//**************************************************************************************************
// タイトル、アゲインボタンを追加する関数
//**************************************************************************************************
void ResultScene::AddButton()
{
	//--------------------------------------------------------------------
	// タイトルシーンへ移行するボタンを作成
	//--------------------------------------------------------------------
	Sprite* pTitleButton = Sprite::createWithTexture(SGraphMgr::Instance()->GetBatchTexture("result_title"));
	Sprite* pTitleButtonTap = Sprite::createWithTexture(SGraphMgr::Instance()->GetBatchTexture("result_title"));

	// タッチ後の雰囲気を変える
	pTitleButtonTap->setColor(Color3B(127.0f, 127.0f, 127.0f));
	pTitleButtonTap->setScale(0.8f);

	pTitleItem = MenuItemSprite::create(
		pTitleButton,
		pTitleButtonTap,
		(Ref*)this,
		menu_selector(ResultScene::ChangeSceneTitle));

	Menu* pTitleMenu = Menu::create(pTitleItem, NULL); // NULLを忘れては動かない
	pTitleMenu->setPosition(
		Point(Director::getInstance()->getWinSize().width / 7 * 4,
		Director::getInstance()->getWinSize().height / 2));

	// はじめは隠しておく
	pTitleMenu->setOpacity(0.0f);

	pTitleMenu->runAction(FadeIn::create(1.0f));
	this->addChild(pTitleMenu, 3);


	//--------------------------------------------------------------------
	// ゲームシーンへ移行するボタンを作成
	//--------------------------------------------------------------------
	Sprite* pGameButton = Sprite::createWithTexture(SGraphMgr::Instance()->GetBatchTexture("result_again"));
	Sprite* pGameButtonTap = Sprite::createWithTexture(SGraphMgr::Instance()->GetBatchTexture("result_again"));

	// タッチ後の雰囲気を変える
	pGameButtonTap->setColor(Color3B(127.0f, 127.0f, 127.0f));
	pGameButtonTap->setScale(0.8f);

	pGameItem = MenuItemSprite::create(
		pGameButton,
		pGameButtonTap,
		(Ref*)this,
		menu_selector(ResultScene::ChangeSceneGame));

	Menu* pGameMenu = Menu::create(pGameItem, NULL); // NULLを忘れては動かない
	pGameMenu->setPosition(
		Point(Director::getInstance()->getWinSize().width / 7 * 6,
		Director::getInstance()->getWinSize().height / 2));

	// はじめは隠しておく
	pGameMenu->setOpacity(0.0f);

	// フェードする
	pGameMenu->runAction(FadeIn::create(1.0f));	
	this->addChild(pGameMenu, 3);
}