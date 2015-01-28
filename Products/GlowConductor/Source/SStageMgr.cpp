#include "SStageMgr.h"
USING_NS_CC;

static float R_START = 0.0f;
static float G_START = 0.0f;
static float B_START = 0.0f;
static float A_START = 100.0f;

//*************************************************************
// コンストラクタ
//*************************************************************
SStageMgr::SStageMgr()
{
	
}

//*************************************************************
// 初期化処理
//*************************************************************
void SStageMgr::Initialize(int stage)
{
	// はじめのステージ背景色を設定
	_r = R_START;
	_g = G_START;
	_b = B_START;
	_a = A_START;

	// ステージ番号を引数で初期化
	_currentStage = stage;
}


//*************************************************************
// ブレンドレイヤーを追加
//*************************************************************
void SStageMgr::AddBlendLayer(Node* addNode, int localZOrder)
{
	// 背景色レイヤーを設定
	_pBlendLayer = LayerColor::create(Color4B(_r, _g, _b, _a));
	BlendFunc blend;
	blend.dst = GL_ONE_MINUS_DST_COLOR;
	blend.src = GL_ONE;
	_pBlendLayer->setBlendFunc(BlendFunc::ALPHA_NON_PREMULTIPLIED);
	addNode->addChild(_pBlendLayer, localZOrder);
}


//*************************************************************
// ステージ番号を一つ進める
//*************************************************************
bool SStageMgr::AddCurrentStage()
{
	_currentStage++;
	bool returnFlag = false;

	// カラーを変更
	_r += 50;
	_g += 200;
	_b += 140;

	if (_r > 255.0f) _r -= 255.0f;
	if (_g > 255.0f) _g -= 255.0f;
	if (_b > 255.0f) _b -= 255.0f;

	// 次のステージが終わりだった場合、初めのステージに戻る
	if (_currentStage == static_cast<int>(eStage_None))
	{
		// 色情報を初期化
		_r = R_START;
		_g = G_START;
		_b = B_START;
		_a = A_START;

		_currentStage = static_cast<int>(eStage_0);
		returnFlag = true;
	}	

	// カラー変更アクションを追加
	_pBlendLayer->runAction(TintTo::create(5.5f, _r, _g, _b));

	return returnFlag;
}

