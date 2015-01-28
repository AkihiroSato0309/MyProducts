#include "SModelMgr.h"
#include <io.h>

// コンストラクタ
SModelMgr::SModelMgr()
{
	// _filenameをnullptrで初期化
	for( int i = 0; i < MAX_MODEL_PTR_NUM; i++ )
	{
		_fileName[i] = nullptr;
	}
}



// 唯一のアクセス経路
SModelMgr* SModelMgr::GetInstance()
{
	static SModelMgr _instance;
	return &_instance;
}

// モデルを初回ロードする
void SModelMgr::Load( Model*& _registerModel, wchar_t* _registerName, std::unique_ptr<EffectFactory>& _fxX  )
{

	// モデル名が存在しない場合警告し、モデルのアドレスにnullptrを返す
	if( _registerName == nullptr || _waccess_s(_registerName, 0) != 0 )
	{
		swprintf_s( cat, 100, L"ファイルが存在しません : %s", _registerName );
		MessageBox( NULL, cat, TEXT("警告"), MB_OK );
		return;
	}

	// 同ファイル名が存在するか
	for( int i = 0; i < MAX_MODEL_PTR_NUM; i++ )
	{
		// _fileName[i]がnullptrなら処理を飛ばす
		if( _fileName[i] == nullptr ) continue;

		// 同ファイル名が存在する場合、その値を参照させる
		if( wcscmp( _fileName[i], _registerName ) == 0 )
		{
			_registerModel = _model[i].get();
			_pModel[i] = &_registerModel;
			return;
		}

	}

	// 同ファイルが存在しないので登録する
	for( int i = 0; i < MAX_MODEL_PTR_NUM; i++ )
	{
		// 同ファイル名が存在する場合、その値を参照させる
		if( _fileName[i] == nullptr )
		{
			_fileName[i] = _registerName;
			_model[i] = Model::CreateFromCMO( g_pd3dDevice, _registerName, *_fxX );
			_registerModel = _model[i].get();	// 作成したモデル情報を代入する
			_pModel[i] = &_registerModel;		// fileName[i]をnullptrを代入
			return;
		}
	}
}

// モデルを上書きし、読み込む
void SModelMgr::Change(wchar_t* _beforeName, wchar_t* _afterName, std::unique_ptr<EffectFactory>& _fxX )
{
	// モデル名が存在しない場合
	if( _beforeName == nullptr || _waccess_s(_beforeName, 0) != 0 )
	{
		swprintf_s( cat, 100, L"ファイルが存在しません : %s", _beforeName );
		MessageBox( NULL, cat, TEXT("警告"), MB_OK );
		return;
	}

	// モデル名が存在しない場合
	if( _afterName == nullptr || _waccess_s(_afterName, 0) != 0 )
	{
		swprintf_s( cat, 100, L"ファイルが存在しません : %s", _afterName );
		MessageBox( NULL, cat, TEXT("警告"), MB_OK );
		return;
	}

	//同ファイル名が存在するか
	for( int i = 0; i < MAX_MODEL_PTR_NUM; i++ )
	{
		// _fileName[i]がnullptr以外の場合
		if( _fileName[i] != nullptr )
		{
			if( wcscmp( _fileName[i], _beforeName ) == 0 && wcscmp( _fileName[i], _afterName ) != 0  )
			{
				_fileName[i] = _afterName;
				*_model[i] = *Model::CreateFromCMO( g_pd3dDevice, _afterName, *_fxX );
				return;
			}
		}
	}
}

// モデルを消去する
void SModelMgr::Relese( wchar_t* _registerName )
{
	// ディレクトリに存在しない場合は警告して抜ける
	if( _waccess_s( _registerName, 0 ) != 0 )
	{
		swprintf_s( cat, 100, L"ファイルが存在しません : %s", _registerName );
		MessageBox( NULL, cat, TEXT("警告"), MB_OK );
		return;
	}

	// 同ファイル名が存在するか
	for( int i = 0; i < MAX_MODEL_PTR_NUM; i++ )
	{
		// _fileName[i]がnullptr以外の場合
		if( _fileName[i] != nullptr )
		{
			//同名ファイルに対応した_model[i]を消去する
			if( wcscmp( _fileName[i], _registerName ) == 0 )
			{
				_fileName[i] = nullptr;		// fileName[i]をnullptrを代入
				_model[i].reset();			//_model[i]をresetする
				// モデルのポインタを持っている全てのポインタをnullptrで初期化
				*_pModel[i] = nullptr;
				return;
			}
		}
	}
}