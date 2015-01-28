#ifndef __OBJECT_MGR__
#define __OBJECT_MGR__

#include "cocos2d.h"
#include <list>

class CObject;
class IPlayerStateChange;

static const int REGIST_OBJ_MAX = 32;

class ObjectMgr : public cocos2d::Node
{
public:
	static const int REGIST_PLAYER = 0;
	static const int REGIST_ENEMY = 1;
	static const int REGIST_ARRAY = 2;

	ObjectMgr();
	~ObjectMgr();

	// 初期化処理
	void InitObjMgr();

	// オブジェクトに登録(どっちの箱に登録するか)
	// 登録に成功した場合、trueを返す
	bool RegistObject(const int arrayNum, CObject* obj);

	// あたり判定の計算
	void CalcCollision();

	// 更新処理
	void Update(const cocos2d::Point& target);

	// クリエイト関数
	CREATE_FUNC(ObjectMgr);

	// プレイヤーアドレスを登録
	void SetPlayerStateChanger(IPlayerStateChange* changer){ playerStateChanger = changer; }

private:
	// あたり判定をとるオブジェクトの登録配列
	std::list<CObject*> CollisionObject[REGIST_ARRAY];

	// プレイヤーのアドレス
	IPlayerStateChange* playerStateChanger;

};

#endif