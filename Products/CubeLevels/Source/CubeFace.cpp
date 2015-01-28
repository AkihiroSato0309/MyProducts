#include "CubeFace.h"
#include "Cube.h"
#include "STextureMgr.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

// 静的なメンバ変数の定義
Matrix CubeFace::_sTopMatrix	= Matrix::Identity();
Matrix CubeFace::_sFrontMatrix	= Matrix::CreateFromAxisAngle(Vector3(1.0f, 0.0f, 0.0f), XMConvertToRadians(90.0f));
Matrix CubeFace::_sBottomMatrix	= Matrix::CreateFromAxisAngle(Vector3(1.0f, 0.0f, 0.0f), XMConvertToRadians(180.0f));
Matrix CubeFace::_sRightMatrix	= CubeFace::_sFrontMatrix * Matrix::CreateFromAxisAngle(Vector3(0.0f, 1.0f, 0.0f), XMConvertToRadians(90.0f));
Matrix CubeFace::_sLeftMatrix	= CubeFace::_sFrontMatrix * Matrix::CreateFromAxisAngle(Vector3(0.0f, 1.0f, 0.0f), XMConvertToRadians(-90.0f));
Matrix CubeFace::_sBackMatrix	= CubeFace::_sFrontMatrix * Matrix::CreateFromAxisAngle(Vector3(0.0f, 1.0f, 0.0f), XMConvertToRadians(180));

Matrix CubeFace::_sPushMatrix		= Matrix::CreateTranslation(0.0f, CUBE_SIZE_HALF, 0.0f);

Matrix CubeFace::_sPlusXMatrix		= Matrix::CreateTranslation( (float)BLOCK_SIZE, 0.0f, 0.0f );
Matrix CubeFace::_sPlusZMatrix		= Matrix::CreateTranslation( 0.0f, 0.0f, (float)BLOCK_SIZE );
Matrix CubeFace::_sMinusXMatrix		= Matrix::CreateTranslation( (float)-BLOCK_SIZE, 0.0f, 0.0f );
Matrix CubeFace::_sMinusZMatrix		= Matrix::CreateTranslation( 0.0f, 0.0f, (float)-BLOCK_SIZE );

//*********************************************************************
// コンストラクタ
//*********************************************************************
CubeFace::CubeFace(int faceDirection, int blockVal[][5])
{
	for(int y = 0; y < 5; y++)
	{
		for(int x = 0; x < 5; x++)
		{
			_blockFace[y][x] = new BlockFace(blockVal[y][x], faceDirection);
			_blockInfo[y* 5 + x] = blockVal[y][x];
		}
	}

	_faceDirection = faceDirection;

	// 面方向に応じて回転させる
	SetCubeFaceMatrix();
	SetCubeFaceRenderFlag();

	// ベース色を取得
	STextureMgr::GetInstance()->Load( _pTexture, L"Image/NoBlockTexture.png" );
}


//*********************************************************************
// デストラクタ
//*********************************************************************
CubeFace::~CubeFace()
{
	for(int y = 0; y < 5; y++)
	{
		for(int x = 0; x < 5; x++)
		{
			delete _blockFace[y][x];
		}
	}
}

//*********************************************************************
// 面を描画
//*********************************************************************
void CubeFace::Render(Matrix& worldMatrix, const Matrix& frontMatrix)
{
	Matrix rotaionMatrix = (*_pParentMoveFlag)?*_pParentMovingMatrix:*_pParentMatrix;

	for(int y = 0; y < 5; y++)
	{
		for(int x = 0; x < 5; x++)
		{
			// ブロックの面をキューブの面に広げるマトリクス
			Matrix blockFacePosMatrix = Matrix::Identity();;

			// ブロックの位置でどれだけ移動させるか決める
			if(x < 2)
			{
				for(int lx = x; lx < 2; lx++)
				{
					blockFacePosMatrix *= _sMinusXMatrix;
				}
			}
			if(x > 2)
			{
				for(int lx = x; lx > 2; lx--)
				{
					blockFacePosMatrix *= _sPlusXMatrix;
				}
			}
			if(y < 2)
			{
				for(int ly = y; ly < 2; ly++)
				{
					blockFacePosMatrix *= _sMinusZMatrix;
				}
			}
			if(y > 2)
			{
				for(int ly = y; ly > 2; ly--)
				{
					blockFacePosMatrix *= _sPlusZMatrix;
				}
			}

			// ブロックの位置を計算する
			Matrix position =	 blockFacePosMatrix
								* _sPushMatrix
								* (*_pDirectionMatrix)
								* Matrix::CreateTranslation((float)CUBE_SIZE * (*(_pParentPosition + 0) - 1),
															(float)CUBE_SIZE * (*(_pParentPosition + 1) - 1),
															(float)CUBE_SIZE * (*(_pParentPosition + 2) - 1))
								* rotaionMatrix
								* worldMatrix
								* frontMatrix;
								

			// ブロックの面を描画
			_blockFace[y][x]->Draw(&position);

		}
	}

}


//*********************************************************************
// キューブのポインタ取得
//*********************************************************************
void CubeFace::SetCube(Cube* cube)
{
	_pCube= cube;
}

//*********************************************************************
// 面の描画フラグを更新する
//*********************************************************************
void CubeFace::SetCubeFaceRenderFlag()
{
	_renderFlag = 0;
	switch(_faceDirection)
	{
	case eTop:
		_renderFlag |=	VERTEX_FLAG[0] |
						VERTEX_FLAG[1] |
						VERTEX_FLAG[2] |
						VERTEX_FLAG[3];
		break;

	case eBottom:
		_renderFlag |=	VERTEX_FLAG[4] |
						VERTEX_FLAG[5] |
						VERTEX_FLAG[6] |
						VERTEX_FLAG[7];
		break;

	case eRight:
		_renderFlag |=	VERTEX_FLAG[1] |
						VERTEX_FLAG[3] |
						VERTEX_FLAG[5] |
						VERTEX_FLAG[7];
		break;

	case eLeft:
		_renderFlag |=	VERTEX_FLAG[0] |
						VERTEX_FLAG[2] |
						VERTEX_FLAG[4] |
						VERTEX_FLAG[6];
		break;

	case eFront:
		_renderFlag |=	VERTEX_FLAG[2] |
						VERTEX_FLAG[3] |
						VERTEX_FLAG[6] |
						VERTEX_FLAG[7];
		break;

	case eBack:
		_renderFlag |=	VERTEX_FLAG[0] |
						VERTEX_FLAG[1] |
						VERTEX_FLAG[4] |
						VERTEX_FLAG[5];
		break;

	default:
		break;
	}
}

//*********************************************************************
// 面ごとにマトリクスを設定する
//*********************************************************************
void CubeFace::SetCubeFaceMatrix()
{
	switch(_faceDirection)
	{
	case eTop:
		_pDirectionMatrix = &_sTopMatrix;
		break;

	case eBottom:
		_pDirectionMatrix = &_sBottomMatrix;
		break;

	case eRight:
		_pDirectionMatrix = &_sRightMatrix;		
		break;

	case eLeft:
		_pDirectionMatrix = &_sLeftMatrix;
		break;

	case eFront:
		_pDirectionMatrix = &_sFrontMatrix;
		break;

	case eBack:
		_pDirectionMatrix = &_sBackMatrix;
		break;

	default:
		break;
	}
}


//*********************************************************************
// 面の向き情報を変更させる
//*********************************************************************
void CubeFace::ChangeFaceDirectionX(int rotaDirection)
{
	switch(_faceDirection)
	{
	case eTop:
		if(rotaDirection == TURN_RIGHT) _faceDirection = eBack;
		else							_faceDirection = eFront;
		break;

	case eBottom:
		if(rotaDirection == TURN_RIGHT) _faceDirection = eFront;
		else							_faceDirection = eBack;
		break;

	case eFront:
		if(rotaDirection == TURN_RIGHT) _faceDirection = eTop;
		else							_faceDirection = eBottom;
		break;

	case eBack:
		if(rotaDirection == TURN_RIGHT) _faceDirection = eBottom;
		else							_faceDirection = eTop;
		break;

	default:
		break;
	}
}

void CubeFace::ChangeFaceDirectionY(int rotaDirection)
{
	switch(_faceDirection)
	{
	case eRight:
		if(rotaDirection == TURN_RIGHT) _faceDirection = eFront;
		else							_faceDirection = eBack;
		break;

	case eLeft:
		if(rotaDirection == TURN_RIGHT) _faceDirection = eBack;
		else							_faceDirection = eFront;
		break;

	case eFront:
		if(rotaDirection == TURN_RIGHT) _faceDirection = eLeft;
		else							_faceDirection = eRight;
		break;

	case eBack:
		if(rotaDirection == TURN_RIGHT) _faceDirection = eRight;
		else							_faceDirection = eLeft;
		break;

	default:
		break;
	}
}

void CubeFace::ChangeFaceDirectionZ(int rotaDirection)
{
	switch(_faceDirection)
	{
	case eTop:
		if(rotaDirection == TURN_RIGHT) _faceDirection = eRight;
		else							_faceDirection = eLeft;
		break;

	case eBottom:
		if(rotaDirection == TURN_RIGHT) _faceDirection = eLeft;
		else							_faceDirection = eRight;
		break;

	case eRight:
		if(rotaDirection == TURN_RIGHT) _faceDirection = eBottom;
		else							_faceDirection = eTop;
		break;

	case eLeft:
		if(rotaDirection == TURN_RIGHT) _faceDirection = eTop;
		else							_faceDirection = eBottom;
		break;

	default:
		break;
	}
}

//*********************************************************************
// 親のマトリクスを取得する
//*********************************************************************
void CubeFace::SetParentMatrix(const Matrix* parentMatrix)
{
	_pParentMatrix = parentMatrix;
	return;
}

//*********************************************************************
// 親の初期位置ポインタを取得する
//*********************************************************************
void CubeFace::SetParentPosition(const int* parentPosition)
{
	_pParentPosition = parentPosition;
	return;
}

//*********************************************************************
// 親の回転フラグを取得する
//*********************************************************************
void CubeFace::SetParentRotaionFlag(const bool* parentMoveFlag)
{
	_pParentMoveFlag = parentMoveFlag;
	return;
}

//*********************************************************************
// 親の回転中に使用するマトリクスを取得する
//*********************************************************************
void CubeFace::SetParentMovingMatrix(const Matrix* parentMovingMatrix)
{
	_pParentMovingMatrix = parentMovingMatrix;
	return;
}