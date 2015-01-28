#pragma once

#include "cocos2d.h"
#include "cocostudio\CocoStudio.h"

//***************************************************************************************
// タイトルシーンクラス
//***************************************************************************************
class TitleScene : public cocos2d::Layer
{
public:
	// シーンを作成
    static cocos2d::Scene* createScene();

	// 初期化処理
	virtual bool init();  
    
    // クリエイト関数作成
    CREATE_FUNC(TitleScene);

	// タッチ開始時に呼ばれる関数
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event);

	// 更新処理
	void update(float delta);

	// スタートボタンを押したときに呼ばれる関数
	void TapStartbutton(cocos2d::Ref* sender);

	// ダイアログに渡す関数
	void PushDialogYes(cocos2d::Ref *sender, cocos2d::ui::TouchEventType type);
	void PushDialogNo(cocos2d::Ref *sender, cocos2d::ui::TouchEventType type);

	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* eve);

private:
	cocos2d::MenuItemSprite* _pStartItem;	// スタートボタンポインタ
	bool _singleTouchFlag;					// 多重タッチを防ぐためのフラグ
	cocos2d::ui::Widget* m_pDialog;			// ダイアログを格納するポインタ
};
