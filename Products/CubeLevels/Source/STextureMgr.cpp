#include "STextureMgr.h"
#include <io.h>

using namespace DirectX;

// コンストラクタ
STextureMgr::STextureMgr()
{
	// _filenameをnullptrで初期化
	for( int i = 0; i < MAX_TEXTURE_PTR_NUM; i++ )
	{
		_fileName[i] = nullptr;
		_texture[i] = nullptr;
	}
	
}

// デストラクタ
STextureMgr::~STextureMgr()
{
	// _filenameをnullptrで初期化
	for( int i = 0; i < MAX_TEXTURE_PTR_NUM; i++ )
	{
		if( _texture[i] != nullptr)
			_texture[i]->Release();
	}
}

// 唯一のアクセス経路
STextureMgr* STextureMgr::GetInstance()
{
	static STextureMgr _instance;
	return &_instance;
}

// モデルを初回ロードする
void STextureMgr::Load( ID3D11ShaderResourceView*& _registerTexture, wchar_t* _registerName  )
{

	// モデル名が存在しない場合警告し、モデルのアドレスにnullptrを返す
	if( _registerName == nullptr || _waccess_s(_registerName, 0) != 0 )
	{
		swprintf_s( cat, 100, L"ファイルが存在しません : %s", _registerName );
		MessageBox( NULL, cat, TEXT("警告"), MB_OK );
		return;
	}

	// 同ファイル名が存在するか
	for( int i = 0; i < MAX_TEXTURE_PTR_NUM; i++ )
	{
		// _fileName[i]がnullptrなら処理を飛ばす
		if( _fileName[i] == nullptr ) continue;

		// 同ファイル名が存在する場合、その値を参照させる
		if( wcscmp( _fileName[i], _registerName ) == 0 )
		{
			_registerTexture = _texture[i];
			_pTexture[i] = &_registerTexture;
			return;
		}

	}

	// 同ファイルが存在しないので登録する
	for( int i = 0; i < MAX_TEXTURE_PTR_NUM; i++ )
	{
		// 同ファイル名が存在する場合、その値を参照させる
		if( _fileName[i] == nullptr )
		{
			_fileName[i] = _registerName;
			// テクスチャ読み込み
			CreateWICTextureFromFile(g_pd3dDevice, g_pImmediateContext, _registerName, nullptr, &_texture[i]);
			_registerTexture = _texture[i];	// 作成したモデル情報を代入する
			_pTexture[i] = &_registerTexture;		// fileName[i]をnullptrを代入
			return;
		}
	}
}

//// モデルを上書きし、読み込む
//void STextureMgr::Change(wchar_t* _beforeName, wchar_t* _afterName, std::unique_ptr<EffectFactory>& _fxX )
//{
//	// モデル名が存在しない場合
//	if( _beforeName == nullptr || _waccess_s(_beforeName, 0) != 0 )
//	{
//		swprintf_s( cat, 100, L"ファイルが存在しません : %s", _beforeName );
//		MessageBox( NULL, cat, TEXT("警告"), MB_OK );
//		return;
//	}
//
//	// モデル名が存在しない場合
//	if( _afterName == nullptr || _waccess_s(_afterName, 0) != 0 )
//	{
//		swprintf_s( cat, 100, L"ファイルが存在しません : %s", _afterName );
//		MessageBox( NULL, cat, TEXT("警告"), MB_OK );
//		return;
//	}
//
//	//同ファイル名が存在するか
//	for( int i = 0; i < MAX_TEXTURE_PTR_NUM; i++ )
//	{
//		// _fileName[i]がnullptr以外の場合
//		if( _fileName[i] != nullptr )
//		{
//			if( wcscmp( _fileName[i], _beforeName ) == 0 && wcscmp( _fileName[i], _afterName ) != 0  )
//			{
//				_fileName[i] = _afterName;
//				*_texture[i] = *Model::CreateFromCMO( g_pd3dDevice, _afterName, *_fxX );
//				return;
//			}
//		}
//	}
//}

// モデルを消去する
void STextureMgr::Relese( wchar_t* _registerName )
{
	// ディレクトリに存在しない場合は警告して抜ける
	if( _waccess_s( _registerName, 0 ) != 0 )
	{
		swprintf_s( cat, 100, L"ファイルが存在しません : %s", _registerName );
		MessageBox( NULL, cat, TEXT("警告"), MB_OK );
		return;
	}

	// 同ファイル名が存在するか
	for( int i = 0; i < MAX_TEXTURE_PTR_NUM; i++ )
	{
		// _fileName[i]がnullptr以外の場合
		if( _fileName[i] != nullptr )
		{
			//同名ファイルに対応した_texture[i]を消去する
			if( wcscmp( _fileName[i], _registerName ) == 0 )
			{
				_fileName[i] = nullptr;		// fileName[i]をnullptrを代入
				delete _texture[i];			//_texture[i]をresetする
				// テクスチャのポインタを持っている全てのポインタをnullptrで初期化
				*_pTexture[i] = nullptr;
				return;
			}
		}
	}
}