#pragma once

#include "cocos2d.h"
#include "SDataMgr.h"

//***************************************************************************************
// リザルトシーン
//***************************************************************************************
class ResultScene : public cocos2d::Layer
{
public:
    // シーンを作成
    static cocos2d::Scene* createScene();

    // 初期化処理
    virtual bool init();  
    
	// クリエイト関数
    CREATE_FUNC(ResultScene);

	// タッチ開始時に呼ばれる関数
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event);
	
	// ランキングを表示する
	void ShowRanking();

	// タッチを許可する関数
	void SetTouchPermit();

	// タイトルシーンに戻る
	void ChangeSceneTitle(cocos2d::Object* pSender);

	// ゲームシーンに戻る
	void ChangeSceneGame(cocos2d::Object* pSender);

	// タイトル、アゲインボタンを追加する関数
	void AddButton();

private:
	int _scoreData[SCORE_SAVE_MAX + 1];			// 保存されたデータを初期化する

	cocos2d::Layer* _pThisLayer;

	cocos2d::MenuItemSprite* pTitleItem;
	cocos2d::MenuItemSprite* pGameItem;
};