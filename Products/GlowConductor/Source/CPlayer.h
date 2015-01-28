#pragma once

#include "CObject.h"
#include "IPlayerStateChange.h"

static const int TOUCH_REMAIN_INIT = 10;	// 残りタッチ回数の初期値
static const int REMOVE_COUNT = 80;			// シーンを遷移するまでの猶予カウント

//***************************************************************************************
// 自機クラス
//***************************************************************************************
class CPlayer : public CObject, public IPlayerStateChange
{
public:
	CPlayer();
	~CPlayer();

	// 親レイヤーノードを登録する
	void SetParent(cocos2d::Node* parentNode);

	// 更新処理
	void Update(){}

	// 進む処理
	void Move();

	// プレイヤーのみあたり判定を小さくするため、小さめに半径を返す
	float GetRadius(){ return _radius / 2.0f; }

	// removeCountを減らす関数
	void CountDown(float delta);

	// スコアを更新する
	void UpdateScore();

	// タップ回数を更新
	void UpdateTapRemain();

	// スコアを表示する関数
	void ShowState(cocos2d::Node* parentNode);

	// スコアを取得する
	int GetScore(){ return _score; }

	// タッチされた座標を取得する
	void SetTouchPoint(const cocos2d::Point& point);

	// クリエイト関数
	CREATE_FUNC(CPlayer);
	static CPlayer* createWithTexture(cocos2d::Texture2D *texture);

	// オブジェクトにあたった時の処理
	void CollisionFunction(IPlayerStateChange* playerStateChanger = nullptr) override;

	int GetTouchRemain(){ return _remainTap; }

	// プレイヤーパーティクルを発生させる関数
	void ParticleCreater(float delta);

	// プレイヤーのキルフラグを取得する
	bool GetKillFlag(){ return _killFlag; }

	// プレイヤーが完全に消失したフラグを取得する
	bool GetRemoveFlag(){ return _removeFlag; }

	//**************************************
	// プレイヤーの状態変化をオーバーライド
	//**************************************
	void AddTouch(int add);				// 残りタッチ回数を加える
	void DownTouch(int down);			// 残りタッチ回数を減らす
	void ScoreUp(int score);			// スコアを加算する
	void ScoreDown(int score);			// スコアを減算する
	void KillPlayer();					// プレイヤー（自機）を殺す

private:
	//+++++++++++++++++++++++++++++++++++++++++++++++++
	// プライベート変数
	//+++++++++++++++++++++++++++++++++++++++++++++++++

	bool _killFlag;						// 死亡フラグ
	bool _removeFlag;					// プレイヤーが完全に消失したかどうかのフラグ（ゲームのエンドフラグ）
	int _removeCount;					// プレイヤーが死亡してから次のシーンに遷移するまでのカウント
	bool _turnFlag;						// 曲がる必要がある場合、曲げるフラグ

	float _speed;						// 速さ
	float _moveRadian;					// 進む角度(右方向を0とした場合)
	float _turnRadianMax;				// 曲がり角度の最大値

	int _remainTap;						// タップできる回数
	cocos2d::Sprite* _pTapRemainBack_1;	// 残りタップ回数の背景１
	cocos2d::Sprite* _pTapRemainBack_2;	// 残りタップ回数の背景２	
	cocos2d::Label* _pTapRemainLabel;	// 残りタップ回数を表示するラベル

	cocos2d::Point _touchedPoint;		// 最後にタッチされたポイント

	float _score;						// スコア
	cocos2d::Label* _pScoreLabel;		// スコアを表示するためのラベル

	cocos2d::Node*	_pParentNode;		//	親のノードを設定する

	//+++++++++++++++++++++++++++++++++++++++++++++++++
	// プライベート関数
	//+++++++++++++++++++++++++++++++++++++++++++++++++

	// 曲がる処理
	void Turn();		

};

