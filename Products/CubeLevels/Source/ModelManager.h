//********************************************************************************
//
// モデルの管理クラス
//
//********************************************************************************
#pragma once

#include "Singleton.h"
#include <map>

// モデル名の列挙型
enum eModels
{
	eModel_Cube,
};

//モデルクラス
class ModelManager : public Singleton<ModelManager>
{
	friend Singleton<ModelManager>;
public:

	// モデルの登録
	DirectX::Model* EntryModel(eModels modelNumber, const wchar_t* modelName);

	// モデルポインタの取得
	DirectX::Model* GetModel(eModels modelNumber);

private:
	ModelManager();
	~ModelManager();

	// モデル保持マップ
	std::map<eModels, DirectX::Model*> _models;
};