#include "ObjectMgr.h"
#include "CObject.h"
#include "cocos2d.h"
#include "Collision.h"
USING_NS_CC;
using namespace std;

//**************************************************************************************************
// コンストラクタ
//**************************************************************************************************
ObjectMgr::ObjectMgr()
{

}



//**************************************************************************************************
// デストラクタ
//**************************************************************************************************
ObjectMgr::~ObjectMgr()
{
	for (int i = 0; i < REGIST_ARRAY; i++)
	{
		list<CObject*>::iterator it = CollisionObject[i].begin();
		while (it != CollisionObject[i].end())
		{
			(*it)->removeFromParentAndCleanup(true);
			it = CollisionObject[i].erase(it);
		}
	}
}



//**************************************************************************************************
// 初期化処理
//**************************************************************************************************
void ObjectMgr::InitObjMgr()
{

}



//**************************************************************************************************
// オブジェクト登録
//**************************************************************************************************
bool ObjectMgr::RegistObject(const int arrayNum, CObject* obj)
{
	// 配列外アクセスを防ぐ
	if (arrayNum >= REGIST_ARRAY)
	{
		return false;
	}

	// オブジェクトを追加
	CollisionObject[arrayNum].emplace_back(obj);
	
	return false;
}



//**************************************************************************************************
// あたり判定計算
//**************************************************************************************************
void ObjectMgr::CalcCollision()
{
	// 配列群の総当たり
	for (int j = 1; j < REGIST_ARRAY; j++)
	{
		list<CObject*>::iterator it_1 = CollisionObject[REGIST_PLAYER].begin();

		while (it_1 != CollisionObject[REGIST_PLAYER].end())
		{
			list<CObject*>::iterator it_2 = CollisionObject[j].begin();
			while (it_2 != CollisionObject[j].end())
			{
				if (CollisionCircle(**it_1, **it_2))
				{
					// 先にオブジェクトの関数を作る
					(*it_2)->CollisionFunction(playerStateChanger);
					(*it_1)->CollisionFunction(playerStateChanger);
				}
				it_2++;
			}
			it_1++;
		}			
	}
	
}



//**************************************************************************************************
// 更新処理(画面中央座標を取得)
//**************************************************************************************************
void ObjectMgr::Update(const Point& target)
{

	for (int i = 1; i < REGIST_ARRAY; i++)
	{
		list<CObject*>::iterator it = CollisionObject[i].begin();
		while (it != CollisionObject[i].end())
		{
			// 死亡フラグが建ていたら消す
			// 画面外に行ったオブジェクトの処理(のちにプレイヤーを独立させる)
			if ((*it)->GetDeleteFlag() ||
				((target.x - Director::getInstance()->getWinSize().width / 2 - (*it)->GetRadius() - 10) >(*it)->getPosition().x))
			{
				(*it)->removeFromParentAndCleanup(true);
				it = CollisionObject[i].erase(it);
			}else it++;
		}
	}
}