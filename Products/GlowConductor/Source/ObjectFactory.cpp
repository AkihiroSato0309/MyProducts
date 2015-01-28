#include "ObjectFactory.h"
#include "CDark.h"
#include "CObject.h"
#include "SGraphMgr.h"
#include "CTouchUp.h"
#include "SCameraTargetMgr.h"
#include "CustomParticle.h"
#include "ActionFactory.h"
#include "FakeNode.h"
#include <string>
#include <iostream>
#include <sstream>
USING_NS_CC;
using namespace std;

//**************************************************************************************************
// コンストラクタ
//**************************************************************************************************
ObjectFactory::ObjectFactory()
{
	_basePointX = TargetPointer->getPositionX() + OBJECT_CREATE_POINT;		// 先にカメラターゲットを初期化しておかなければならない
	_nextStageCount = 0;
}

//**************************************************************************************************
// デストラクタ
//**************************************************************************************************
ObjectFactory::~ObjectFactory()
{
	
}

//**************************************************************************************************
// オブジェクト生成関数
//**************************************************************************************************
CObject* ObjectFactory::CreateObject(	int objectType,
										float radian,
										float x, float y,
										int actionType,
										float useData[4],
										bool useFlag)
{
	CObject* object = nullptr;

	// 生成
	if (objectType == eDark)	object = CreateDark(radian, x, y, actionType);
	if (objectType == eTouchUp) object = CreateTouchUp(radian, x, y, actionType);


	// 生成されたオブジェクトにアクションを追加する
	if (actionType == eAction_MoveVartical) ActionFactory::CreateMoveVartical(object, useData[0], useData[1], useFlag);
	if (actionType == eAction_MoveHorizontal) ActionFactory::CreateMoveHorizontal(object, useData[0], useData[1], useFlag);
	if (actionType == eAction_MoveToDown) ActionFactory::CreateMoveToDown(object, useData[0], useData[1]);
	if (actionType == eAction_MoveToUp) ActionFactory::CreateMoveToUp(object, useData[0], useData[1]);
	if (actionType == eAction_ScaleChange) ActionFactory::CreateScaleChange(object, useData[0], useData[1], useData[2], useFlag);


	// 今のシーンにオブジェクトを追加する
	currentNode->addChild(object, 1);

	return object;
}

CObject* ObjectFactory::CreateObject(sObjectData& objectData)
{
	return CreateObject(objectData._type,
						objectData._radius,
						objectData._x,
						objectData._y,
						objectData._setActionType,
						objectData._useData,
						objectData._useFlag);
}

//**************************************************************************************************
// 闇の渦生成関数
//**************************************************************************************************
CObject* ObjectFactory::CreateDark(float radian, float x, float y, int ai)
{
	// 各画像関数は、そのシーンで読み込まれていることが前提となる
	CObject* object = CDark::createWithTexture(SGraphMgr::Instance()->GetBatchTexture("dark_back"));
	object->SetState(radian, x, y, ai);

	// 闇オブジェクトの背景スプライトを作成
	Sprite* pDarkBackSprite = Sprite::createWithTexture(SGraphMgr::Instance()->GetBatchTexture("dark"));
	pDarkBackSprite->setPosition(DARK_BACK_RADIUS, DARK_BACK_RADIUS);
	object->addChild(pDarkBackSprite, 0);

	// パーティクルをセット
	ParticleSystemQuad* particle = MyParticles::DarkParticle::create();
	particle->setPosition(BALL_RADIUS, BALL_RADIUS);
	particle->setStartRadius(BALL_RADIUS);
	particle->setPositionType(ParticleSystem::PositionType::RELATIVE);
	particle->setAutoRemoveOnFinish(true);
	object->addChild(particle);

	// アクションをセット
	RotateBy* rotate = RotateBy::create(1.0f, 600.0f);
	object->runAction(RepeatForever::create(rotate));

	return object;
}

//**************************************************************************************************
// タッチアップアイテム生成関数
//**************************************************************************************************
CObject* ObjectFactory::CreateTouchUp(float radius, float x, float y, int ai)
{
	CObject* object = CTouchUp::createWithTexture(SGraphMgr::Instance()->GetSpriteBatch("touch_up")->getTexture());
	object->SetState(radius, x, y, ai);

	// アクションを設定
	object->setRotation(-30);
	ActionInterval* rotateAct = Sequence::create(
		EaseInOut::create(RotateBy::create(1.0f, 60), 2.0f),
		EaseInOut::create(RotateBy::create(1.0f, -60), 2.0f),
		nullptr);
	object->runAction(RepeatForever::create(rotateAct));
	object->setBlendFunc(BlendFunc::ADDITIVE);

	return object;
}

//**************************************************************************************************
// カメラの位置によってオブジェクトを自動生成
//**************************************************************************************************
CObject* ObjectFactory::AutoCraeteObject(Point targetPoint)
{
	CObject* object = nullptr;

	// 今のステージデータを取得する
	int currentStage = SStageMgr::Instance()->GetCurrentStage();

	//====================================================================
	// プレイヤーにもっとも近いデータのみを比較
	// 一定範囲内に入ったらオブジェクトを生成する
	//====================================================================
	if (_nextStageCount == 0 && _dataIt != _objectData[currentStage].end() && TargetPointer->getPosition().x + OBJECT_CREATE_POINT > (*_dataIt).first + _basePointX)
	{
		// オブジェクトを生成
		sObjectData l_objectData = (*_dataIt).second;	// 生成起点ポイントを足すためのコピーオブジェクト
		l_objectData._x += _basePointX;					// 生成位置に生成起点ポイント分を足す
		object = CreateObject(l_objectData);

		// 次のオブジェクトデータを指す
		_dataIt++;

		// イテレーターが最後まで来た場合、次のステージに切り替わるカウントを始める
		if (_dataIt == _objectData[currentStage].end())
		{
			_nextStageCount = 1;
		}
	}
	// 次ステージまでのカウントが始まっていた場合
	else if (_nextStageCount > 0)
	{
		_nextStageCount += 1;
		// 指定カウントを超えたら、次のステージに移動し、カウントを0に戻す
		if (_nextStageCount > NEXT_STAGE_COUNT)
		{
			NextStage();
			_nextStageCount = 0;
		}
	}
	
	return object;
}

//**************************************************************************************************
// オブジェクトデータ群を初期化する
//**************************************************************************************************
void ObjectFactory::InitObjectData()
{
	// 始めのプレイステージを取得
	int startStage = SStageMgr::Instance()->GetCurrentStage();


	//====================================================================
	// あらかじめ使用するマップデータをすべて読み込んでおく
	//====================================================================
	for (int initStageId = 0; initStageId < SStageMgr::eStageNum::eStage_None; initStageId++)
	{
		//--------------------------------------------------------
		// 外部から敵データファイルの読み込み
		//--------------------------------------------------------
		string fileName = "ObjectData";
		stringstream idstream;					// 数値を入れる
		idstream << initStageId;
		string stageIdName = idstream.str();	// 今のステージ番号をStringに変換
		fileName = fileName + stageIdName + ".txt";
		std::string filePath = FileUtils::sharedFileUtils()->fullPathForFilename(fileName);

		//--------------------------------------------------------
		// データ読み込み
		//--------------------------------------------------------
		ssize_t size;
		unsigned char* data = FileUtils::getInstance()->getFileData(filePath, "r", &size);
		// データからストリングストリーム作成
		stringstream ss((char*)data);

		// 一行ずつデータを読み取る
		vector<string> value;					// 1行分のデータ格納用
		string loadStr;					
		while (getline(ss, loadStr))
		{
			value.push_back(loadStr);
		}

		vector<string>::iterator it = value.begin();
		
		int stage = atoi((*it).c_str());		// ステージ番号を取得
		it++;
		int row = atoi((*it++).c_str());		// オブジェクト数を取得
		int col = atoi((*it++).c_str());		// オブジェクト1つの持つデータ数を取得
			
		//--------------------------------------------------------
		// 読み取ったデータをもとにオブジェクトを作成
		//--------------------------------------------------------
		string kari;							// 読み取るための仮変数
		sObjectData tmpObjectData;				// 読み取ったデータを格納する

		// 読み取ったデータをコンマ区切りで分け、各変数に入れる
		for (int i = 0; i < row; i++)
		{
			stringstream s(*it++);
			getline(s, kari, ',');
			tmpObjectData._type = atoi(kari.c_str());

			getline(s, kari, ',');
			tmpObjectData._radius = static_cast<float>(atof(kari.c_str()));

			getline(s, kari, ',');
			tmpObjectData._x = static_cast<float>(atof(kari.c_str()));

			getline(s, kari, ',');
			tmpObjectData._y = static_cast<float>(atof(kari.c_str()));

			getline(s, kari, ',');
			tmpObjectData._setActionType = static_cast<float>(atof(kari.c_str()));

			for (auto& useData : tmpObjectData._useData)
			{
				getline(s, kari, ',');
				useData = static_cast<float>(atof(kari.c_str()));
			}

			getline(s, kari, ',');
			tmpObjectData._useFlag = static_cast<bool>(atoi(kari.c_str()));
			

			// 取得したステージ番号マップにオブジェクト情報を入れる
			AddObjectData(tmpObjectData, stage);
		}
	}

	// イテレーターをプレイするステージマップの始めに持ってくる
	_dataIt = _objectData[startStage].begin();
}

//****************************************************************
// オブジェクトデータ群に追加する
//****************************************************************
void ObjectFactory::AddObjectData(sObjectData& objData, const int& stageId)
{
	_objectData[stageId].emplace(type_map::value_type(objData._x, objData));
}

//****************************************************************
// ステージを進める
//****************************************************************
bool ObjectFactory::NextStage()
{
	// オブジェクトの生成起点を再設定する
	_basePointX = TargetPointer->getPositionX() + OBJECT_CREATE_POINT;

	// ステージ管理のポインタを取得
	SStageMgr* pStageMgr = SStageMgr::Instance();

	// ステージを進める
	if (pStageMgr->AddCurrentStage())
	{
		// イテレーターを始めにセット
		_dataIt = _objectData[pStageMgr->GetCurrentStage()].begin();
		return true;
	}

	// イテレーターを始めにセット
	_dataIt = _objectData[pStageMgr->GetCurrentStage()].begin();
	return false;
}