#include "SGraphMgr.h"
USING_NS_CC;

// コンストラクタ
SGraphMgr::SGraphMgr()
{
	// 画像ファイルがあるフォルダ
	_texturePlace = "graph/";
}


//**************************************************************************************
// 指定したシーン内で使用する画像を読み込む
//**************************************************************************************
bool SGraphMgr::SetUsingGraph(eScene scene)
{
	// すべての要素を解放
	for (auto& sprite : pSpriteBatchNode)
	{
		sprite.second->removeFromParentAndCleanup(true);
	}
	pSpriteBatchNode.clear();

	// 指定されたシーンの画像を読み込む
	if (scene == eScene_Game) return SetGraph_Game();
	if (scene == eScene_title) return SetGraph_Title();
	if (scene == eScene_Result) return SetGraph_Result();
	return true;
}


//**************************************************************************************
// 配列内のスプライトバッチを指定ノード(シーン)に登録する関数
//**************************************************************************************
bool SGraphMgr::AddUsingScene(Node* node)
{
	for (auto& sprite : pSpriteBatchNode)
	{
		node->addChild(sprite.second);
	}

	return true;
}


//**************************************************************************************
// バッチノードを取得
//**************************************************************************************
SpriteBatchNode* SGraphMgr::GetSpriteBatch(const std::string& fileName)
{
	if (pSpriteBatchNode[fileName] == nullptr) return nullptr;
	else return pSpriteBatchNode[fileName];
}


//**************************************************************************************
// バッチノードのテクスチャを取得
//**************************************************************************************
Texture2D* SGraphMgr::GetBatchTexture(const std::string& fileName)
{
	// 登録されていない場合nullptrを返す
	if (pSpriteBatchNode[fileName] == nullptr) return nullptr;
	return pSpriteBatchNode[fileName]->getTexture();
}


//**************************************************************************************
// 各シーンで使用する画像読み込み関数
//**************************************************************************************
// ゲームシーンで使用する画像を読み込む関数
bool SGraphMgr::SetGraph_Game()
{
	AddSpriteBatch("ball");
	AddSpriteBatch("touch_up");
	AddSpriteBatch("touchWindow");
	AddSpriteBatch("rectangle");
	AddSpriteBatch("particle_normal_ring");
	AddSpriteBatch("right_star");
	AddSpriteBatch("dark_back");
	AddSpriteBatch("dark");
	AddSpriteBatch("touch_up_light_1");
	AddSpriteBatch("touch_up_light_2");
	AddSpriteBatch("menu_button");

	return true;
}


//**************************************************************************************
// タイトルシーンで使用する画像を読み込む関数
//**************************************************************************************
bool SGraphMgr::SetGraph_Title()
{ 
	AddSpriteBatch("title_word_back");
	AddSpriteBatch("right_star"); 
	AddSpriteBatch("ball");
	AddSpriteBatch("title_right");
	AddSpriteBatch("title_name_1");
	AddSpriteBatch("title_name_2");
	AddSpriteBatch("title_start_button");
	AddSpriteBatch("title_back_graph");
	AddSpriteBatch("particle_normal_ring");
	AddSpriteBatch("rectangle");

	return true;
}


//**************************************************************************************
//リザルトシーンで使用する画像を読み込む関数
//**************************************************************************************
bool SGraphMgr::SetGraph_Result()
{
	AddSpriteBatch("right_star"); 
	AddSpriteBatch("ball");
	AddSpriteBatch("result_back_graph");
	AddSpriteBatch("particle_normal_ring");
	AddSpriteBatch("rectangle");
	AddSpriteBatch("score_back");
	AddSpriteBatch("result_word");
	AddSpriteBatch("result_again");
	AddSpriteBatch("result_title");
	return true;
}


//**************************************************************************************
// 画像配列にスプライトバッチを追加(PNG専用、fileNameに拡張子不要)
//**************************************************************************************
void SGraphMgr::AddSpriteBatch(const std::string& fileName)
{
	pSpriteBatchNode.insert(graphMap::value_type(fileName, SpriteBatchNode::create(_texturePlace + fileName + ".png")));
}