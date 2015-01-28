/*------------------------------------------------------------------------------------------

 ■ 名前 : STextureMgr.h

 ■ 作成 : 服部和輝

 AhoStyleEngineに使用。書き換え可

------------------------------------------------------------------------------------------*/

#pragma once

#include "Global.h"
#include "Common.h"

//テクスチャ保存最大数
static const unsigned char MAX_TEXTURE_PTR_NUM = 128;

/*******************************************************************************************

 ■ -- Class名 ----- ■

　・STextureMgr（singleton）

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

class STextureMgr
{
public:

	// 唯一のアクセス経路
	static STextureMgr* GetInstance();

	// モデルを初回ロードする
	void Load( ID3D11ShaderResourceView*&, wchar_t* );
	// モデルを上書きし、読み込む
	//void Change( wchar_t*, wchar_t*, std::unique_ptr<DirectX::EffectFactory>& );
	// モデルを消去する
	void Relese( wchar_t* );

private:

	// コンストラクタ
	STextureMgr();

	// デストラクタ
	~STextureMgr();

	// アクセス禁止
	STextureMgr( const STextureMgr& rhs );
	STextureMgr& operator=( const STextureMgr& rhs );	

	wchar_t* _fileName[MAX_TEXTURE_PTR_NUM];		// モデル名保存
	wchar_t cat[100];							// 連結文字列保存

	// テクスチャデータ保存用
	ID3D11ShaderResourceView*	_texture[MAX_TEXTURE_PTR_NUM];
	// テクスチャポインタを保持しているポインタのポインタ
	ID3D11ShaderResourceView**	 _pTexture[MAX_TEXTURE_PTR_NUM];

};