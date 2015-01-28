#pragma once

#include "cocos2d.h"
#include "cocostudio\CocoStudio.h"

class ObjectMgr;
class CameraTarget;
class GameBackLayer;
class GameLayer;

//***************************************************************************************
// ゲームシーンクラス
//***************************************************************************************
class GameScene : public cocos2d::Layer
{
public:
    // シーン作成
    static cocos2d::Scene* createScene();

    // 初期化処理
    virtual bool init();  
    
    // クリエイト関数
    CREATE_FUNC(GameScene);

	// タッチ開始時に呼ばれる関数
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event);

	// 更新処理
	void update(float time);

	// メニュー起動関数
	void AwakeMenu(cocos2d::Ref* pSender);

	// タイトルへ戻る
	void ToTitle(cocos2d::Ref *sender, cocos2d::ui::TouchEventType type);

	// リトライする
	void ToRetry(cocos2d::Ref *sender, cocos2d::ui::TouchEventType type);

	// ゲームに戻る
	void ToBack(cocos2d::Ref *sender, cocos2d::ui::TouchEventType type);

private:
	// リザルトシーンへ遷移する
	void ChangeSceneResult();

	// オブジェクト管理
	ObjectMgr* objectMgr;

	
	bool _gameEndFlag;				// ゲーム終了フラグ
	bool _gameStopFlag;				// 停止フラグ

	//****************************************
	// 各レイヤーへのポインタ
	//****************************************
	GameLayer* _pGameLayer;
	GameBackLayer* _pBackLayer;
	cocos2d::Layer*	_pUiLayer;
	cocos2d::LayerColor* _pBlendLayer;

	// シングルタッチ用のフラグ
	bool _singleTouchFlag;

};