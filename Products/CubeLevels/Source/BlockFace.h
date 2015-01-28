//***************************************************************************************************
//
//	BlockFace.h				佐藤彰洋  12月16日
//
//	ブロック一つの面についてのクラス
//
//***************************************************************************************************
#pragma once

#include "Global.h"

// ブロックの大きさ
static const int BLOCK_SIZE = 5;
static const float BLOCK_SIZE_HALF = BLOCK_SIZE * 0.5f;

// 軸を中心とした時計回り
static const int TURN_RIGHT = -1;
static const int TURN_LEFT = 1;

static const int GOAL = 9;

// 軸を指定した回転の列挙型
enum eRotation
{
	eRotation_x,
	eRotation_y,
	eRotation_z,
};

// ステージ全体で、どの面に属しているか
enum eFaceDirection
{
	eTop,
	eBottom,
	eRight,
	eLeft,
	eFront,
	eBack,
};


// ブロックの面クラス
class BlockFace
{
public:
	BlockFace(int hit, int faceDirection);
	~BlockFace();

	// 更新処理
	void Update();

	// 描画処理
	void Draw(DirectX::SimpleMath::Matrix* matrix);

	// 当たり判定の有無を取得
	int IsHitState(){return _hit;}

	// 回転によって板の向きを変える
	void Rotation(eRotation rotation, int turn_direction);


private:
	// モデルのアドレス
	DirectX::Model* _model;

	// テクスチャアドレス
	ID3D11ShaderResourceView* _pTexture;

	// 当たり判定の有り無し
	int _hit;

	// どの面に属しているか
	int _faceDirection;
	
	// 5x5のうち、何番目の位置か[縦][横]
	int _position[2];
};