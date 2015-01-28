#include "CubeFace.h"
#include "Cube.h"
#include "STextureMgr.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

// �ÓI�ȃ����o�ϐ��̒�`
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
// �R���X�g���N�^
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

	// �ʕ����ɉ����ĉ�]������
	SetCubeFaceMatrix();
	SetCubeFaceRenderFlag();

	// �x�[�X�F���擾
	STextureMgr::GetInstance()->Load( _pTexture, L"Image/NoBlockTexture.png" );
}


//*********************************************************************
// �f�X�g���N�^
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
// �ʂ�`��
//*********************************************************************
void CubeFace::Render(Matrix& worldMatrix, const Matrix& frontMatrix)
{
	Matrix rotaionMatrix = (*_pParentMoveFlag)?*_pParentMovingMatrix:*_pParentMatrix;

	for(int y = 0; y < 5; y++)
	{
		for(int x = 0; x < 5; x++)
		{
			// �u���b�N�̖ʂ��L���[�u�̖ʂɍL����}�g���N�X
			Matrix blockFacePosMatrix = Matrix::Identity();;

			// �u���b�N�̈ʒu�łǂꂾ���ړ������邩���߂�
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

			// �u���b�N�̈ʒu���v�Z����
			Matrix position =	 blockFacePosMatrix
								* _sPushMatrix
								* (*_pDirectionMatrix)
								* Matrix::CreateTranslation((float)CUBE_SIZE * (*(_pParentPosition + 0) - 1),
															(float)CUBE_SIZE * (*(_pParentPosition + 1) - 1),
															(float)CUBE_SIZE * (*(_pParentPosition + 2) - 1))
								* rotaionMatrix
								* worldMatrix
								* frontMatrix;
								

			// �u���b�N�̖ʂ�`��
			_blockFace[y][x]->Draw(&position);

		}
	}

}


//*********************************************************************
// �L���[�u�̃|�C���^�擾
//*********************************************************************
void CubeFace::SetCube(Cube* cube)
{
	_pCube= cube;
}

//*********************************************************************
// �ʂ̕`��t���O���X�V����
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
// �ʂ��ƂɃ}�g���N�X��ݒ肷��
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
// �ʂ̌�������ύX������
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
// �e�̃}�g���N�X���擾����
//*********************************************************************
void CubeFace::SetParentMatrix(const Matrix* parentMatrix)
{
	_pParentMatrix = parentMatrix;
	return;
}

//*********************************************************************
// �e�̏����ʒu�|�C���^���擾����
//*********************************************************************
void CubeFace::SetParentPosition(const int* parentPosition)
{
	_pParentPosition = parentPosition;
	return;
}

//*********************************************************************
// �e�̉�]�t���O���擾����
//*********************************************************************
void CubeFace::SetParentRotaionFlag(const bool* parentMoveFlag)
{
	_pParentMoveFlag = parentMoveFlag;
	return;
}

//*********************************************************************
// �e�̉�]���Ɏg�p����}�g���N�X���擾����
//*********************************************************************
void CubeFace::SetParentMovingMatrix(const Matrix* parentMovingMatrix)
{
	_pParentMovingMatrix = parentMovingMatrix;
	return;
}