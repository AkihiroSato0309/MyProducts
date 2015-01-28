#pragma once

#include "cocos2d.h"
#include <list>
#include <vector>
#include <map>
#include "SStageMgr.h"

// 前方宣言
class CObject;

static const float OBJECT_CREATE_POINT = 2500.0f;	// プレイヤーのいくら先にオブジェクトを生成するか
static const int NEXT_STAGE_COUNT = 240;			// 次のステージに切り替わるまでのカウント

// オブジェクト作成に必要なデータ群
class sObjectData
{
public:
	// コンストラクタ
	sObjectData(){}

	int	_type;						// 作成オブジェクトのタイプ
	float _radius;					// オブジェクトの半径（あたり判定にも使用）
	float _x;						// X座標
	float _y;						// Y座標
	int	_setActionType;				// アクションタイプ
	float _useData[4];				// アクション作成時に使用する数値を格納（最大4つ）
	bool _useFlag;					// アクション作成時に使用するフラグを格納
};

// オブジェクトの種類
enum eObjectType
{
	eDark, 
	eTouchUp
};

// アクション番号は、エクセルのActionManualシートのIDに則る
enum eObjectActionType
{
	eAction_None = 0,
	eAction_MoveVartical,
	eAction_MoveHorizontal,
	eAction_MoveToDown,
	eAction_MoveToUp,
	eAction_ScaleChange
};

//****************************************************************
// ObjectFactoryクラス
//****************************************************************
class ObjectFactory : public cocos2d::Node
{
	typedef std::multimap<float, sObjectData> type_map;
public:
	ObjectFactory();
	~ObjectFactory();

	// オブジェクトを生成
	CObject* CreateObject(
		int objectType,
		float radian,
		float x, float y,
		int actionType = 0,
		float useData[4] = {0},
		bool useFlag = false);

	CObject* CreateObject(sObjectData& objectData);

	// 生成ノードを登録する
	void SetNode(cocos2d::Node* node){ currentNode = node; }

	// オブジェクトデータ群にデータを入れる（仮）
	void InitObjectData();

	// オブジェクトデータ群に追加する
	void AddObjectData(sObjectData& objData, const int& stageId);

	// カメラの位置によってオブジェクトを生成する
	CObject* AutoCraeteObject(cocos2d::Point playerPoint);

	CREATE_FUNC(ObjectFactory);

private:
	// 生成するオブジェクト
	// 闇生成関数
	CObject* CreateDark(float radian, float x, float y, int ai = 0);
	CObject* CreateTouchUp(float radian, float x, float y, int ai = 0);

	// 生成したオブジェクトを吊り下げるノード
	cocos2d::Node* currentNode;

	// オブジェクトのデータベース
	type_map _objectData[static_cast<int>(SStageMgr::eStageNum::eStage_None)];

	//データベースへアクセスするためのイテレーター
	type_map::iterator _dataIt;

	// ステージを進める(ステージが最後まで来た場合trueを返す)
	bool NextStage();

	// 自動オブジェクト生成時、起点となるX座標
	float _basePointX;

	// 次のステージへ移動する際に使用するカウント
	int _nextStageCount;
};