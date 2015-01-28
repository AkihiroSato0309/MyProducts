/*------------------------------------------------------------------------------------------

 ■ 名前 : SModelMgr.h

 ■ 作成 : 服部和輝

 AhoStyleEngineに使用。書き換え可

------------------------------------------------------------------------------------------*/

#pragma once

#include "Global.h"
#include <Model.h>

using namespace DirectX;

//モデル保存最大数
static const unsigned char MAX_MODEL_PTR_NUM = 128;

/*******************************************************************************************

 ■ -- Class名 ----- ■

　・SModelMgr（singleton）

 ■ -- 使用用途 ----- ■

　・3Dモデルの保管と読み取り、書き換え、消去をします

 ■ -- 使用方法 ----- ■
　
　・モデル名を事前に登録しておくことで、再度、モデルを使用する際に、参照渡しできる。

  ・モデル名のポインタを使用中の場合、書き換えは不可能。エラー処理になります。

  ・モデルのポインタ消去も同様に、使用中は使用できません。

  ・128以上のモデルを読み込ませてくことはできません。

　・モデルのポインタを他で保持する場合、そのポインタをデリートしてはいけません。

　・モデルはqnique_ptrで宣言ではなくModel*で宣言して使用してください。

********************************************************************************************/

class SModelMgr
{
public:

	// 唯一のアクセス経路
	static SModelMgr* GetInstance();

	// モデルを初回ロードする
	void Load( Model*&, wchar_t*, std::unique_ptr<EffectFactory>& );
	// モデルを上書きし、読み込む
	void Change( wchar_t*, wchar_t*, std::unique_ptr<EffectFactory>& );
	// モデルを消去する
	void Relese( wchar_t* );

private:

	// コンストラクタ
	SModelMgr();

	// アクセス禁止
	SModelMgr( const SModelMgr& rhs );
	SModelMgr& operator=( const SModelMgr& rhs );	

	wchar_t* _fileName[MAX_MODEL_PTR_NUM];		// モデル名保存
	wchar_t cat[100];							// 連結文字列保存

	// モデルデータ保存用
	std::unique_ptr<Model> _model[MAX_MODEL_PTR_NUM];
	// モデルポインタを保持しているポインタのポインタ
	Model**				   _pModel[MAX_MODEL_PTR_NUM];

};