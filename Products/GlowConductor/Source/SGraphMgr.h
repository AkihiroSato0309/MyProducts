#pragma once

static const int GRAPH_MAX = 64;

#include "Singleton.h"
#include "cocos2d.h"

enum eScene
{
	eScene_title,
	eScene_Game,
	eScene_Result,
};


//**************************************************************
// グラフィック管理のシングルトンクラス
//**************************************************************
class SGraphMgr : public Singleton<SGraphMgr>, public cocos2d::Node
{
	friend Singleton< SGraphMgr >;

	typedef std::map<std::string, cocos2d::SpriteBatchNode*> graphMap;

public:
	//**************************************************************
	// 各シーンで使用する画像
	//**************************************************************


	// そのシーンで使用する画像を配列に登録する関数
	bool SetUsingGraph(eScene scene);

	// 配列内のスプライトバッチを指定ノードに登録する関数
	bool AddUsingScene(cocos2d::Node* node);

	// 配列画像を取得する
	cocos2d::SpriteBatchNode* GetSpriteBatch(const std::string& fileName);

	// テクスチャを取得
	cocos2d::Texture2D* GetBatchTexture(const std::string& fileName);

	//**********************************************************
	// 各シーンで使用する画像読み込み関数
	//**********************************************************
	bool SetGraph_Game();
	bool SetGraph_Title();
	bool SetGraph_Result();

private:
	SGraphMgr();

	// 画像配列
	graphMap pSpriteBatchNode;

	// 画像が存在するフォルダ
	std::string _texturePlace;

	// 画像配列にスプライトバッチを追加(PNG専用、fileNameに拡張子不要)
	void AddSpriteBatch(const std::string& fileName);

};

