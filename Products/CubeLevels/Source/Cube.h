//***************************************************************************************************
//
// Cube.h				佐藤彰洋  12月16日
//
//***************************************************************************************************
#pragma once
#include "CubeFace.h"

static const int CUBE_SIZE = BLOCK_SIZE * 5;			// キューブのサイズ
static const float CUBE_SIZE_HALF = CUBE_SIZE * 0.5f;	// キューブの半分

// 軸のベクトル
static const DirectX::SimpleMath::Vector3 AXIS_X(1.0f, 0.0f, 0.0f);
static const DirectX::SimpleMath::Vector3 AXIS_Y(0.0f, 1.0f, 0.0f);
static const DirectX::SimpleMath::Vector3 AXIS_Z(0.0f, 0.0f, 1.0f);

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// キューブクラス
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Cube
{
public:
	Cube();
	Cube(int x, int y, int z);
	~Cube();

	// 更新
	void Update(float moveDegree);
	
	// 描画
	void Render();

	// 回転中の描画
	void RenderMove(float moveFlame);

	// 回転による位置情報の改変
	void TransPosition(eRotation rotation, const int& turn_direction, DirectX::SimpleMath::Matrix& worldMatrix);


	// 現在の位置情報取得
	int* GetCurrentPoaition()
	{
		return _currentPosition;
	}

	// マトリクスを取得する
	const DirectX::SimpleMath::Matrix& GetCurrentMatrix(){return _rotation_matrix;}
	const DirectX::SimpleMath::Matrix& GetBeforeMatrix(){return _before_roation_matrix;}

	// 位置をセットする
	void SetCurrentPosition(int x, int y, int z)
	{
		_currentPosition[0] = x;
		_currentPosition[1] = y;
		_currentPosition[2] = z;
	}

	// 回転フラグをリセットする
	void ResetRotationFlag(){_movingFlag = false;}

	// マトリクスのアドレスを取得
	DirectX::SimpleMath::Matrix* GetMatrix()
	{
		return &_rotation_matrix;
	}

	// キューブの登録
	void EntryCubeFace(CubeFace* cubeFace);

	// 指定した面に属するCubeFaceポインタを返す
	CubeFace* GetAppointCubeFace(int appointFace);

private:
	// 登録させるキューブ面のポインタ（最大3つ）
	CubeFace* _pCubeFace[3];

	// モデル変数
	DirectX::Model* model;

	// 回転させた時のマトリクスを保存
	DirectX::SimpleMath::Matrix _rotation_matrix;
	DirectX::SimpleMath::Matrix _before_roation_matrix;
	
	// 回転中に使用するマトリクス
	DirectX::SimpleMath::Matrix _moveRotationMatrix;

	// 動いている時のフラグ
	bool _movingFlag;

	// 位置情報管理 0～2(原点からの個数で表す　0:x, 1:y, 2:z)
	int _currentPosition[3];
	int _originPosition[3];

	// 回転角度情報(X,Y,Zの軸を中心とした角度)
	float _currentRadian[3];

};