//***************************************************************************************************
//
// CubeFace.h				佐藤彰洋  12月16日
//
//***************************************************************************************************
#pragma once

#include "BlockFace.h"
class Cube;

// 各頂点のビットフラグ(トップの左奥から)
static const int VERTEX_FLAG[8] = 
{
	1 << 0,
	1 << 1,
	1 << 2,
	1 << 3,
	1 << 4,
	1 << 5,
	1 << 6,
	1 << 7,
};

class CubeFace
{
public:
	// コンストラクタ
	CubeFace(int faceDirection, int blockVal[][5]);

	// デストラクタ
	~CubeFace();

	// 面を描画
	void Render(DirectX::SimpleMath::Matrix& worldMatrix, const DirectX::SimpleMath::Matrix& frontMatrix);

	// マトリクスを設定する
	void SetCube(Cube* cube);

	// 面の描画フラグを取得する
	int GetRenderFlag(){return _renderFlag;}

	// 所属面を取得する
	int GetFaceDirection(){return _faceDirection;}

	// ブロック情報を取得する
	int* GetBlockInfo(){return _blockInfo;}

	// 面による角度マトリクス
	static DirectX::SimpleMath::Matrix _sTopMatrix;
	static DirectX::SimpleMath::Matrix _sBottomMatrix;
	static DirectX::SimpleMath::Matrix _sRightMatrix;
	static DirectX::SimpleMath::Matrix _sLeftMatrix;
	static DirectX::SimpleMath::Matrix _sFrontMatrix;
	static DirectX::SimpleMath::Matrix _sBackMatrix;

	// 面を押し出すマトリクス
	static DirectX::SimpleMath::Matrix _sPushMatrix;

	// 面を1ブロック移動させるマトリクス
	static DirectX::SimpleMath::Matrix _sPlusXMatrix;
	static DirectX::SimpleMath::Matrix _sPlusZMatrix;
	static DirectX::SimpleMath::Matrix _sMinusXMatrix;
	static DirectX::SimpleMath::Matrix _sMinusZMatrix;

	// 面の向き情報を変更させる
	void ChangeFaceDirectionX(int rotaDirection);
	void ChangeFaceDirectionY(int rotaDirection);
	void ChangeFaceDirectionZ(int rotaDirection);

	// 面の描画フラグを更新する
	void SetCubeFaceRenderFlag();

	// 面ごとにマトリクスを設定する
	void SetCubeFaceMatrix();

	// 親のマトリクスを取得する
	void SetParentMatrix(const DirectX::SimpleMath::Matrix* parentMatrix);

	// 親の初期位置ポインタを取得する
	void SetParentPosition(const int* parentPosition);

	// 親の回転フラグを取得する
	void SetParentRotaionFlag(const bool* parentMoveFlag);

	// 親の回転中に使用するフラグを取得する
	void SetParentMovingMatrix(const DirectX::SimpleMath::Matrix* parentMatrix);

private:
	// 面のブロックの数
	BlockFace* _blockFace[5][5];

	// ベースの色を取得
	ID3D11ShaderResourceView*	_pTexture;

	// ブロックの情報
	int _blockInfo[25];

	// 面の方向
	int _faceDirection;

	// 描画の有無フラグ(ビット管理)
	int _renderFlag;

	// 連動元のキューブのアドレス
	Cube* _pCube;

	// 面を回転させて表示させるマトリクス
	DirectX::SimpleMath::Matrix* _pDirectionMatrix;

	// 親キューブの回転マトリクスポインタ
	const DirectX::SimpleMath::Matrix* _pParentMatrix;

	// 親キューブの回転中に使用するマトリクスポインタ
	const DirectX::SimpleMath::Matrix* _pParentMovingMatrix;

	// 親キューブの位置情報配列の先頭アドレス
	const int* _pParentPosition;

	// 親キューブの回転フラグ
	const bool* _pParentMoveFlag;	
};