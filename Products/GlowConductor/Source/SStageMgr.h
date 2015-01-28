#pragma once

#include "Singleton.h"
#include "cocos2d.h"

//***************************************************************************************
// ステージ管理クラス
//***************************************************************************************
class SStageMgr : public Singleton<SStageMgr>
{
	friend Singleton<SStageMgr>;
	
public:
	// ステージ番号の列挙型
	// ステージを増やす際は、ここに書き加える
	enum eStageNum
	{
		eStage_0,
		eStage_1,
		eStage_2,
		eStage_3,
		eStage_4,
		eStage_5,
		eStage_6,
		eStage_7,

		eStage_None
	};

public:

	// 背景色レイヤー
	void AddBlendLayer(cocos2d::Node* addNode, int localZOrder = 0);

	// 現在のステージ番号を取得
	int GetCurrentStage(){ return _currentStage; }

	// ステージを1進める
	bool AddCurrentStage();

	// 初期化処理
	void Initialize(int stage);

private:
	SStageMgr();
	~SStageMgr(){}

	// 現在のステージ番号
	int _currentStage;

	// 背景色レイヤーのカラー情報
	float _r;
	float _g;
	float _b;
	float _a;

	// 背景色レイヤーポインタ
	cocos2d::LayerColor* _pBlendLayer;

};