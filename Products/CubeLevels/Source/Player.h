//******************************************************************************
//
//  操作するプレイヤークラス
//
//******************************************************************************

#pragma once

#include "Common.h"
#include "SStageRotationInfo.h"
#include "IStageMatrixChanger.h"

static const float PLAYER_SPEED = 0.5f;
static const float PLAYER_SIZE = 4.0f;
static const float PLAYER_SIZE_HALF = PLAYER_SIZE / 2.0f;

static const float G = 0.06f;
static const float JUMP_POWER = 0.8f;
static const float DOWNSPEED_MAX = -3.0f;


class Player
{
public:
	// コンストラクタ
	Player();
	// デストラクタ
	~Player();

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 更新処理
	// 第一引数	: 指定したマップの情報（当たり判定に使用）
	// 第二引数	: キューブが回転中かどうか
	// 戻り値	: プレイヤーがゴールしていたらtrue
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	bool Update(int* map, const bool& cubeMovingFlag);

	// 描画処理
	void Render();

	// 存在している面を取得する
	int GetExistFace(){return _existFace;}

	// プレイヤーの上方向を取得する
	int GetUpDirection(){return _upDirection;}

	// 右へ移動
	void MoveRight();

	// 左へ移動
	void MoveLeft();

	// 上へ移動
	void MoveUp();

	// 下へ移動
	void MoveDown();

	// ジャンプ
	void Jump();

	// マトリクスを加算する
	void GetMatrixChanger(IStageMatrixChanger* matrixChanger)
	{
		_pMatrixChanger = matrixChanger;
	}

	// ステージの回転情報を受け取り、それに応じてプレイヤーを回転させる
	void PlayerRotationWithCube(const sStageRotationInfo& stageRotationInfo);

	// プレイヤーの移動フラグを取得する
	bool GetMovingFlag()
	{
		return _movingFlag;
	}

private:
	// プレイヤー画像
	ID3D11ShaderResourceView* _pTexture;

	// 存在している面情報
	int _existFace;

	// プレイヤーがキューブ上のどこに存在しているか(X, Y, Z)
	sIntVector3 _cubeExist;

	// 存在している面に対して、プレイヤーがどの方向を上としているか
	int _upDirection;

	// 上方向の情報を回転させる
	void ChangeUpDirection(const eUpDirection& direction);

	// 位置情報(2D)
	DirectX::SimpleMath::Vector2 _2Dposition;

	// 位置情報(3D)
	DirectX::SimpleMath::Vector3 _3Dposition;

	// 移動ベクトル
	DirectX::SimpleMath::Vector2 _2DMoveVector;

	// 面方向に対しての回転マトリクス
	DirectX::SimpleMath::Matrix _rotaionMatrix;

	// マトリクスを変化させる
	IStageMatrixChanger* _pMatrixChanger;

	// プレイヤーが移動しているとtrue,止まっているとfalse
	bool _movingFlag;

	// マップを考慮して移動させる
	bool Move(float* x, float* y, float* jump_power, float move_x, float move_y, bool* jump_flag, int map[][15]);

	// マップとの当たり判定
	int MapHitCheck(float x, float y, float* move_x, float* move_y, int map[][15]);

	// 指定した座標に存在するマップチップの値を返す
	int GetChipParam(float x, float y, int map[][15]);

	// 隣接面への移動処理
	void MoveCubeFaceRight();
	void MoveCubeFaceLeft();
	void MoveCubeFaceUp();
	void MoveCubeFaceDown();

	// ジャンプフラグ
	bool _jumpFlag;

	// ジャンプパワー
	float _jumpPower;

};