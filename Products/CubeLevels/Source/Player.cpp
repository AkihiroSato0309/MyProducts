#include "Player.h"
#include "Cube.h"
#include "STextureMgr.h"
#include "FrontFaceMatrix.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

VertexPositionColorTexture playerVertexes[4] = {
	VertexPositionColorTexture(Vector3( -PLAYER_SIZE_HALF,  0.0f,  -PLAYER_SIZE_HALF), Vector4( 1.0f, 1.0f, 1.0f, 0.0f ), Vector2( 0.0f, 0.0f )),	// 0
	VertexPositionColorTexture(Vector3(  PLAYER_SIZE_HALF,  0.0f,  -PLAYER_SIZE_HALF), Vector4( 1.0f, 1.0f, 1.0f, 0.0f ), Vector2( 1.0f, 0.0f )),	// 1
	VertexPositionColorTexture(Vector3( -PLAYER_SIZE_HALF,  0.0f,   PLAYER_SIZE_HALF), Vector4( 1.0f, 1.0f, 1.0f, 0.0f ), Vector2( 0.0f, 1.0f )),	// 2
	VertexPositionColorTexture(Vector3(  PLAYER_SIZE_HALF,  0.0f,   PLAYER_SIZE_HALF), Vector4( 1.0f, 1.0f, 1.0f, 0.0f ), Vector2( 1.0f, 1.0f )),	// 3
};

// �ÓI�ȃ����o�ϐ��̒�`(�����v��肪�v���X)
Matrix sTopMatrix		= Matrix::CreateFromAxisAngle(Vector3(1.0f, 0.0f, 0.0f), XMConvertToRadians(-90.0f));
Matrix sBottomMatrix	= Matrix::CreateFromAxisAngle(Vector3(1.0f, 0.0f, 0.0f), XMConvertToRadians(90.0f));
Matrix sRightMatrix		= Matrix::CreateFromAxisAngle(Vector3(0.0f, 1.0f, 0.0f), XMConvertToRadians(90.0f));
Matrix sLeftMatrix		= Matrix::CreateFromAxisAngle(Vector3(0.0f, 1.0f, 0.0f), XMConvertToRadians(-90.0f));
Matrix sFrontMatrix		= Matrix::Identity();
Matrix sBackMatrix		= Matrix::CreateFromAxisAngle(Vector3(0.0f, 1.0f, 0.0f), XMConvertToRadians(-180.0f));


//******************************************************************
// �R���X�g���N�^
//******************************************************************
Player::Player()
	: _2DMoveVector(0.0f,0.0f)
	, _2Dposition(15.0f, 20.0f)
	, _jumpFlag(true)
{
	// �e�N�X�`�������[�h
	STextureMgr::GetInstance()->Load( _pTexture, L"Image/PlayerTexture.png" );

	// �v���C���[�̑��݂����
	_existFace = eFront;

	// �����̃v���C���[�A�b�v�����͏�ɒ�߂�
	_upDirection = eUpDirect;
}

//******************************************************************
// �f�X�g���N�^
//******************************************************************
Player::~Player()
{

}
//******************************************************************
// �X�V����
//******************************************************************
bool Player::Update(int* map, const bool& cubeMovingFlag)
{
	// 2���z��ɕϊ�
	int l_map[15][15];
	for(int y = 0; y < 15; y++)
	{
		for(int x = 0; x < 15; x++)
		{
			l_map[y][x] = *(map + (y * 15 + x));
		}
	}

	//�L���[�u����]���̏ꍇ�A�v���C���[�𓮂����Ȃ�
	if(cubeMovingFlag == false)
	{
		// ��������
		switch(_upDirection)
		{
		case eUpDirect:
			_2DMoveVector.y += _jumpPower;
			break;

		case eRightDirect:
			_2DMoveVector.x += _jumpPower;
			break;

		case eDownDirect:
			_2DMoveVector.y -= _jumpPower;
			break;

		case eLeftDirect:
			_2DMoveVector.x -= _jumpPower;
			break;
		}
		_jumpPower -= G;

		// �������x�ɏ����݂���
		if(_jumpPower < DOWNSPEED_MAX) _jumpPower = DOWNSPEED_MAX;

		// �ړ�����
		if(Move(&_2Dposition.x, &_2Dposition.y, &_jumpPower, _2DMoveVector.x, _2DMoveVector.y
			, &_jumpFlag, l_map) == true)
			return true;
	}

	// �ړ��\�苗�����ق�0�̂Ƃ��A�ړ����t���O��false�ɂ���
	if(_2DMoveVector.x < 0.1f && _2DMoveVector.y < 0.1f &&  _2DMoveVector.x > -0.1f && _2DMoveVector.y > -0.1f)
	{
		_movingFlag = false;
	}else{
		_movingFlag = true;
	}

	// �ړ��\�蕪�����Z�b�g
	_2DMoveVector.x = 0.0f;
	_2DMoveVector.y = 0.0f;

#pragma region �v���C���[�̃L���[�u�ʈړ�����

	// ����
	if(_2Dposition.x - PLAYER_SIZE_HALF < 0.0f)
	{
		if(_existFace == eTop)
		{
			_2Dposition.x = CUBE_SIZE * 3 - _2Dposition.y;
			_2Dposition.y = CUBE_SIZE * 3 - PLAYER_SIZE_HALF - 0.02f;
			ChangeUpDirection(eChange270);

		}else if(_existFace == eBottom)
		{
			_2Dposition.x = _2Dposition.y;
			_2Dposition.y = PLAYER_SIZE_HALF + 0.02f;
			ChangeUpDirection(eChange90);
		}else{
			_2Dposition.x = CUBE_SIZE * 3 - PLAYER_SIZE_HALF - 0.02f;
		}

		//_pMatrixChanger->ChangeMatrix(eRotationLeft);
		MoveCubeFaceLeft();
	}
	// �E��
	if(_2Dposition.x + PLAYER_SIZE_HALF > CUBE_SIZE * 3)
	{
		if(_existFace == eTop)
		{
			_2Dposition.x = _2Dposition.y;
			_2Dposition.y = CUBE_SIZE * 3 - PLAYER_SIZE_HALF - 0.02f;
			ChangeUpDirection(eChange90);
		}else if(_existFace == eBottom)
		{
			_2Dposition.x = CUBE_SIZE * 3 - _2Dposition.y;
			_2Dposition.y = PLAYER_SIZE_HALF + 0.02f;
			ChangeUpDirection(eChange270);
		}else{
			_2Dposition.x = PLAYER_SIZE_HALF;
		}
		//_pMatrixChanger->ChangeMatrix(eRotationRight);
		MoveCubeFaceRight();
	}
	// ����
	if(_2Dposition.y - PLAYER_SIZE_HALF < 0.0f)
	{
		if(_existFace == eRight)
		{
			_2Dposition.y = CUBE_SIZE * 3 - _2Dposition.x;
			_2Dposition.x = CUBE_SIZE * 3 - PLAYER_SIZE_HALF - 0.02f;
			ChangeUpDirection(eChange90);
		}else if(_existFace == eLeft)
		{
			_2Dposition.y = _2Dposition.x;
			_2Dposition.x = PLAYER_SIZE_HALF;
			ChangeUpDirection(eChange270);
		}else if(_existFace == eBottom)
		{
			_2Dposition.x = CUBE_SIZE * 3 - _2Dposition.x;
			_2Dposition.y = PLAYER_SIZE_HALF + 0.02f;
			ChangeUpDirection(eChange180);
		}else if(_existFace == eBack)
		{
			_2Dposition.x = CUBE_SIZE * 3 - _2Dposition.x;
			_2Dposition.y = PLAYER_SIZE_HALF + 0.02f;
			ChangeUpDirection(eChange180);
		}else{
			_2Dposition.y = CUBE_SIZE * 3 - PLAYER_SIZE_HALF - 0.02f;
		}
		_jumpPower = 0.0f;
		_jumpFlag = false;
		//_pMatrixChanger->ChangeMatrix(eRotationDown);
		MoveCubeFaceDown();
	}
	// ���
	if(_2Dposition.y + PLAYER_SIZE_HALF  > CUBE_SIZE * 3)
	{
		if(_existFace == eRight)
		{
			_2Dposition.y = _2Dposition.x;
			_2Dposition.x = CUBE_SIZE * 3 - PLAYER_SIZE_HALF - 0.02f;
			ChangeUpDirection(eChange270);
		}else if(_existFace == eLeft)
		{
			_2Dposition.y = CUBE_SIZE * 3 - _2Dposition.x;
			_2Dposition.x = PLAYER_SIZE_HALF;
			ChangeUpDirection(eChange90);
		}else if(_existFace == eTop)
		{
			_2Dposition.y = CUBE_SIZE * 3 - PLAYER_SIZE_HALF - 0.02f;
			_2Dposition.x = CUBE_SIZE * 3 - _2Dposition.x;
			ChangeUpDirection(eChange180);
		}else if(_existFace == eBack)
		{
			_2Dposition.y = CUBE_SIZE * 3 - PLAYER_SIZE_HALF - 0.02f;
			_2Dposition.x = CUBE_SIZE * 3 - _2Dposition.x;
			ChangeUpDirection(eChange180);
		}else{
			_2Dposition.y = PLAYER_SIZE_HALF;
		}
		_jumpPower = 0.0f;
		_jumpFlag = false;
		//_pMatrixChanger->ChangeMatrix(eRotationUp);
		MoveCubeFaceUp();
	}

#pragma endregion

	// 3D�`��ʒu�̎w��
	switch(_existFace)
	{
	case eTop:
		_rotaionMatrix = sTopMatrix;
		break;

	case eBottom:
		_rotaionMatrix = sBottomMatrix;
		break;

	case eRight:
		_rotaionMatrix = sRightMatrix;
		break;

	case eLeft:
		_rotaionMatrix = sLeftMatrix;
		break;

	case eFront:
		_rotaionMatrix = sFrontMatrix;
		break;

	case eBack:
		_rotaionMatrix = sBackMatrix;
		break;

	default:
		break;
	}

	// ���ۂ̃L���[�u��ł̃v���C���[�̈ʒu���X�V����
	if(_existFace == eTop)
	{
		_cubeExist.y = 2;
		_cubeExist.x = _2Dposition.x / CUBE_SIZE;
		_cubeExist.z = 2 - _2Dposition.y / CUBE_SIZE;
	}
	if(_existFace == eBottom)
	{
		_cubeExist.y = 0;
		_cubeExist.x = _2Dposition.x / CUBE_SIZE;
		_cubeExist.z = _2Dposition.y / CUBE_SIZE;
	}
	if(_existFace == eRight)
	{
		_cubeExist.x = 2;
		_cubeExist.y = _2Dposition.y / CUBE_SIZE;
		_cubeExist.z = 2 - _2Dposition.x / CUBE_SIZE;
	}
	if(_existFace == eLeft)
	{
		_cubeExist.x = 0;
		_cubeExist.y = _2Dposition.y / CUBE_SIZE;
		_cubeExist.z = _2Dposition.x / CUBE_SIZE;
	}
	if(_existFace == eFront)
	{
		_cubeExist.z = 2;
		_cubeExist.y = _2Dposition.y / CUBE_SIZE;
		_cubeExist.x = _2Dposition.x / CUBE_SIZE;
	}
	if(_existFace == eBack)
	{
		_cubeExist.z = 0;
		_cubeExist.y = _2Dposition.y / CUBE_SIZE;
		_cubeExist.x = 2 - _2Dposition.x / CUBE_SIZE;
	}

	// ��������
	//_rotaionMatrix = sFrontMatrix;

	return false;
}

//******************************************************************
// �`�揈��
//******************************************************************
void Player::Render()
{
	Matrix l_frontFaceMatrix;

	// �v���C���[�̑��݂���ʂɂ���āA�n���}�g���N�X��ւ���
	// (�v���C���[����ɐ��ʂɉf��悤�ɂ��邽��)
	switch( _existFace )
	{
	case eTop:
		l_frontFaceMatrix = FRONT_TOP;
		break;

	case eBottom:
		l_frontFaceMatrix = FRONT_BOTTOM;
		break;
		
	case eRight:
		l_frontFaceMatrix = FRONT_RIGHT;
		break;

	case eLeft:
		l_frontFaceMatrix = FRONT_LEFT;
		break;

	case eFront:
		l_frontFaceMatrix = FRONT_FRONT;
		break;

	case eBack:
		l_frontFaceMatrix = FRONT_BACK;
		break;

	default:
		break;

	}

	// �쐬�����s����G�t�F�N�g�ɐݒ�
	Matrix position = Matrix::CreateFromAxisAngle(Vector3(1.0f, 0.0f, 0.0f), XMConvertToRadians(90.0f))
		* Matrix::CreateTranslation(_2Dposition.x - CUBE_SIZE_HALF * 3, _2Dposition.y - CUBE_SIZE_HALF * 3, CUBE_SIZE_HALF * 3 + 0.1f)
		* _rotaionMatrix
		* l_frontFaceMatrix;

	g_BatchEffect->SetWorld( position );
	g_BatchEffect->SetView( g_Camera->GetView() );
	g_BatchEffect->SetProjection( g_Camera->GetProjection() );

	g_BatchEffect->SetVertexColorEnabled(FALSE);

	// �e�N�X�`���̐ݒ�
	g_BatchEffect->SetTexture( _pTexture );

	// �G�t�F�N�g�̐ݒ�i�e�s���e�N�X�`���Ȃǂ�ݒ肵�Ă���j
	g_BatchEffect->Apply( g_pImmediateContext );


	// �[�x�X�e���V�� �X�e�[�g��ݒ肷��
	g_pImmediateContext->OMSetDepthStencilState( g_States->DepthDefault(), 0 );

	// �u�����f�B���O �X�e�[�g��ݒ肷��
	g_pImmediateContext->OMSetBlendState( g_pBlendState, nullptr, 0xFFFFFFFF ); 

	// �T���v���[�X�e�[�g��ݒ肷��
	auto samplerState = g_States->PointWrap();
	g_pImmediateContext->PSSetSamplers(0, 1, &samplerState);

	// ���̓��C�A�E�g��ݒ肷��
	g_pImmediateContext->IASetInputLayout( g_pBatchInputLayout );

	g_Batch->Begin();

	// ���_�f�[�^
	uint16_t indexes[4] = { 0,  1,  2,  3 };

	// �O�p�`�v���~�e�B�u�̕`��
	g_Batch->DrawIndexed(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, indexes, 4, playerVertexes, 4);

	g_Batch->End();
	
}

#pragma region �ړ��Ɋւ��鏈��

//******************************************************************
// �E�ֈړ�
//******************************************************************
void Player::MoveRight()
{
	switch(_upDirection)
	{
	case eUpDirect:
		_2DMoveVector.x += PLAYER_SPEED;
		break;

	case eRightDirect:
		_2DMoveVector.y -= PLAYER_SPEED;
		break;

	case eDownDirect:
		_2DMoveVector.x -= PLAYER_SPEED;
		break;

	case eLeftDirect:
		_2DMoveVector.y += PLAYER_SPEED;
		break;
	}
}

//******************************************************************
// ���ֈړ�
//******************************************************************
void Player::MoveLeft()
{
	switch(_upDirection)
	{
	case eUpDirect:
		_2DMoveVector.x -= PLAYER_SPEED;
		break;

	case eRightDirect:
		_2DMoveVector.y += PLAYER_SPEED;
		break;

	case eDownDirect:
		_2DMoveVector.x += PLAYER_SPEED;
		break;

	case eLeftDirect:
		_2DMoveVector.y -= PLAYER_SPEED;
		break;
	}
}

//******************************************************************
// ��ֈړ�
//******************************************************************
void Player::MoveUp()
{
	switch(_upDirection)
	{
	case eUpDirect:
		_2DMoveVector.y += PLAYER_SPEED;
		break;

	case eRightDirect:
		_2DMoveVector.x += PLAYER_SPEED;
		break;

	case eDownDirect:
		_2DMoveVector.y -= PLAYER_SPEED;
		break;

	case eLeftDirect:
		_2DMoveVector.x -= PLAYER_SPEED;
		break;
	}
}


//******************************************************************
// ���ֈړ�
//******************************************************************
void Player::MoveDown()
{
	switch(_upDirection)
	{
	case eUpDirect:
		_2DMoveVector.y -= PLAYER_SPEED;
		break;

	case eRightDirect:
		_2DMoveVector.x -= PLAYER_SPEED;
		break;

	case eDownDirect:
		_2DMoveVector.y += PLAYER_SPEED;
		break;

	case eLeftDirect:
		_2DMoveVector.x += PLAYER_SPEED;
		break;
	}
}


//******************************************************************
// �W�����v����
//******************************************************************
void Player::Jump()
{
	if(_jumpFlag == false)
	{
		_jumpFlag = true;
		_jumpPower = JUMP_POWER;
	}
}

#pragma endregion

#pragma region �����蔻��Ɋւ��鏈��

// �}�b�v���l�����Ĉړ�������
bool Player::Move(float* x, float* y, float* jump_power, float move_x, float move_y, bool* jump_flag, int map[][15])
{
	 float Dummy = 0.0f ;
	 int returnMapHitCheck;

    // �L�����N�^�̍���A�E��A�����A�E�������������蔻��̂���
    // �}�b�v�ɏՓ˂��Ă��邩���ׁA�Փ˂��Ă�����␳����

    // �悸�㉺�ړ����������Ń`�F�b�N
    {
        // �����̃`�F�b�N�A�����u���b�N�̏�ӂɒ����Ă����痎�����~�߂�
		returnMapHitCheck = MapHitCheck( *x - PLAYER_SIZE_HALF, *y + PLAYER_SIZE_HALF, &Dummy, &move_y, map);
		if( returnMapHitCheck == 3 && (_upDirection == eUpDirect || _upDirection == eDownDirect) )
			_jumpPower = 0.0F;
		if( returnMapHitCheck == GOAL ) return true;

        // �E���̃`�F�b�N�A�����u���b�N�̏�ӂɒ����Ă����痎�����~�߂�
		returnMapHitCheck = MapHitCheck( *x + PLAYER_SIZE_HALF, *y + PLAYER_SIZE_HALF, &Dummy, &move_y, map );
        if( returnMapHitCheck == 3 && (_upDirection == eUpDirect || _upDirection == eDownDirect) ) 
			_jumpPower = 0.0F;
		if( returnMapHitCheck == GOAL ) return true;

        // ����̃`�F�b�N�A�����u���b�N�̉��ӂɓ������Ă����痎��������
		returnMapHitCheck = MapHitCheck( *x - PLAYER_SIZE_HALF, *y - PLAYER_SIZE_HALF, &Dummy, &move_y, map);
        if( returnMapHitCheck == 4 && (_upDirection == eUpDirect || _upDirection == eDownDirect) ) 
			_jumpPower = 0.0F;
		if( returnMapHitCheck == GOAL ) return true;
		
        // �E��̃`�F�b�N�A�����u���b�N�̉��ӂɓ������Ă����痎��������
		returnMapHitCheck = MapHitCheck( *x + PLAYER_SIZE_HALF, *y - PLAYER_SIZE_HALF, &Dummy, &move_y, map);
        if( returnMapHitCheck == 4 && (_upDirection == eUpDirect || _upDirection == eDownDirect) ) 
			_jumpPower = 0.0F;
		if( returnMapHitCheck == GOAL ) return true;

        // �㉺�ړ����������Z
        *y += move_y ;
    }

    // ��ɍ��E�ړ����������Ń`�F�b�N
    {
        // �����̃`�F�b�N
        returnMapHitCheck = MapHitCheck( *x - PLAYER_SIZE_HALF, *y + PLAYER_SIZE_HALF, &move_x, &Dummy, map );
		if( returnMapHitCheck > 0 && (_upDirection == eRightDirect || _upDirection == eLeftDirect) ) 
			_jumpPower = 0.0F;
		if( returnMapHitCheck == GOAL ) return true;

        // �E��̃`�F�b�N
         returnMapHitCheck = MapHitCheck( *x + PLAYER_SIZE_HALF, *y + PLAYER_SIZE_HALF, &move_x, &Dummy, map );
		if( returnMapHitCheck > 0 && (_upDirection == eRightDirect || _upDirection == eLeftDirect) ) 
			_jumpPower = 0.0F;
		if( returnMapHitCheck == GOAL ) return true;

        // ����̃`�F�b�N
         returnMapHitCheck = MapHitCheck( *x - PLAYER_SIZE_HALF, *y - PLAYER_SIZE_HALF, &move_x, &Dummy, map );
		if( returnMapHitCheck > 0 && (_upDirection == eRightDirect || _upDirection == eLeftDirect) ) 
			_jumpPower = 0.0F;
		if( returnMapHitCheck == GOAL ) return true;

        // �E���̃`�F�b�N
         returnMapHitCheck = MapHitCheck( *x + PLAYER_SIZE_HALF, *y - PLAYER_SIZE_HALF, &move_x, &Dummy, map );
		if( returnMapHitCheck > 0 && (_upDirection == eRightDirect || _upDirection == eLeftDirect) ) 
			_jumpPower = 0.0F;
		if( returnMapHitCheck == GOAL ) return true;

        // ���E�ړ����������Z
        *x += move_x ;
    }

#pragma region  �ڒn����
   
	// �v���C���[�̏�����ɂ���āA�n�ʂ̔����ւ���
    // �L�����N�^�̍����ƉE���̉��ɒn�ʂ����邩���ׂ�
	if(_upDirection == eUpDirect)
	{
		if( GetChipParam( *x - PLAYER_SIZE_HALF, *y - PLAYER_SIZE_HALF - 0.3F , map) == 0 && 
			GetChipParam( *x + PLAYER_SIZE_HALF, *y - PLAYER_SIZE_HALF - 0.3F , map) == 0 )
			// ���ꂪ����������W�����v���ɂ���
			_jumpFlag = true ;
		else
			// ���ꂪ�݂�����ڒn���ɂ���
			_jumpFlag = false ;
	}
	if(_upDirection == eDownDirect)
	{
		if( GetChipParam( *x - PLAYER_SIZE_HALF, *y + PLAYER_SIZE_HALF + 0.3F , map) == 0 && 
			GetChipParam( *x + PLAYER_SIZE_HALF, *y + PLAYER_SIZE_HALF + 0.3F , map) == 0 )
			// ���ꂪ����������W�����v���ɂ���
			_jumpFlag = true ;
		else
			// ���ꂪ�݂�����ڒn���ɂ���
			_jumpFlag = false ;
	}
	if(_upDirection == eRightDirect)
	{
		if( GetChipParam( *x - PLAYER_SIZE_HALF - 0.3f, *y - PLAYER_SIZE_HALF, map) == 0 && 
			GetChipParam( *x - PLAYER_SIZE_HALF - 0.3f, *y + PLAYER_SIZE_HALF, map) == 0 )
			// ���ꂪ����������W�����v���ɂ���
			_jumpFlag = true ;
		else
			// ���ꂪ�݂�����ڒn���ɂ���
			_jumpFlag = false ;
	}
	if(_upDirection == eLeftDirect)
	{
		if( GetChipParam( *x + PLAYER_SIZE_HALF + 0.3f, *y - PLAYER_SIZE_HALF , map) == 0 && 
			GetChipParam( *x + PLAYER_SIZE_HALF + 0.3f, *y + PLAYER_SIZE_HALF , map) == 0 )
			// ���ꂪ����������W�����v���ɂ���
			_jumpFlag = true ;
		else
			// ���ꂪ�݂�����ڒn���ɂ���
			_jumpFlag = false ;
	}
#pragma endregion

    // �I��
    return false;
}

// �}�b�v�Ƃ̓����蔻��
int Player::MapHitCheck(float x, float y, float* move_x, float* move_y, int map[][15])
{
	float afX, afY ;

    // �ړ��ʂ𑫂�
    afX = x + *move_x ;
    afY = y + *move_y ;

    // �����蔻��̂���u���b�N�ɓ������Ă��邩�`�F�b�N
    if( GetChipParam( afX, afY, map ) )
    {
        float blx, bty, brx, bby ;

		// �S�[����������
		if( GetChipParam( afX, afY, map ) == GOAL)
		{
			return GOAL;
		}

        // �������Ă�����ǂ��痣���������s��

        // �u���b�N�̏㉺���E�̍��W���Z�o
		blx = (float)( (int)afX / BLOCK_SIZE ) * BLOCK_SIZE ;        // ���ӂ� X ���W
        brx = (float)( (int)afX / BLOCK_SIZE + 1 ) * BLOCK_SIZE ;    // �E�ӂ� X ���W

        bty = (float)( (int)afY / BLOCK_SIZE ) * BLOCK_SIZE ;        // ��ӂ� Y ���W
        bby = (float)( (int)afY / BLOCK_SIZE + 1 ) * BLOCK_SIZE ;    // ���ӂ� Y ���W

        // ��ӂɓ������Ă����ꍇ
        if( *move_y > 0.0F )
        {
            // �ړ��ʂ�␳����
            *move_y = bty - y - 0.03F;

            // ��ӂɓ��������ƕԂ�
            return 3 ;
        }

        // ���ӂɓ������Ă����ꍇ
        if( *move_y < 0.0F )
        {
            // �ړ��ʂ�␳����
            *move_y = bby - y + 0.03F ;

            // ���ӂɓ��������ƕԂ�
            return 4 ;
        }

        // ���ӂɓ������Ă����ꍇ
        if( *move_x > 0.0F )
        {
            // �ړ��ʂ�␳����
            *move_x = blx - x - 0.03F ;

            // ���ӂɓ��������ƕԂ�
            return 1 ;
        }

        // �E�ӂɓ������Ă����ꍇ
        if( *move_x < 0.0F )
        {
            // �ړ��ʂ�␳����
            *move_x = brx - x + 0.03F ;

            // �E�ӂɓ��������ƕԂ�
            return 2 ;
        }

        // �����ɗ�����K���Ȓl��Ԃ�
        return 4 ;
    }

    // �ǂ��ɂ�������Ȃ������ƕԂ�
    return 0 ;
}

// �w�肵�����W�ɑ��݂���}�b�v�`�b�v�̒l��Ԃ�
int Player::GetChipParam(float x, float y, int map[][15])
{
	int X = (int)x / BLOCK_SIZE;
	int Y = (int)y / BLOCK_SIZE;

	if(X < 0) X = 0;
	if(X > 14) X = 14;
	if(Y < 0) Y = 0;
	if(Y > 14) Y = 14;

	return map[14 - Y][X];
}
#pragma endregion

//****************************************************************
// �אږʂւ̈ړ�����
//****************************************************************
void Player::MoveCubeFaceUp()
{
	switch(_existFace)
	{
	case eTop:
		_existFace = eBack;
		break;

	case eBottom:
		_existFace = eFront;
		break;

	case eRight:
	case eLeft:
	case eFront:
	case eBack:
		_existFace = eTop;
		break;

	default:
		break;
	}
}

void Player::MoveCubeFaceDown()
{
	switch(_existFace)
	{
	case eTop:
		_existFace = eFront;
		break;

	case eBottom:
		_existFace = eBack;
		break;

	case eRight:
	case eLeft:
	case eFront:
	case eBack:
		_existFace = eBottom;
		break;

	default:
		break;
	}
}

void Player::MoveCubeFaceRight()
{
	switch(_existFace)
	{
	case eTop:
		_existFace = eRight;
		break;

	case eBottom:
		_existFace = eRight;
		break;

	case eRight:
		_existFace = eBack;
		break;

	case eLeft:
		_existFace = eFront;
		break;

	case eFront:
		_existFace = eRight;
		break;

	case eBack:
		_existFace = eLeft;
		break;

	default:
		break;
	}
}

void Player::MoveCubeFaceLeft()
{
	switch(_existFace)
	{
	case eTop:
		_existFace = eLeft;
		break;

	case eBottom:
		_existFace = eLeft;
		break;

	case eRight:
		_existFace = eFront;
		break;

	case eLeft:
		_existFace = eBack;
		break;

	case eFront:
		_existFace = eLeft;
		break;

	case eBack:
		_existFace = eRight;
		break;

	default:
		break;
	}
}

//****************************************************************
// �v���C���[�̖ʂɑ΂��������̕ύX�֐�
//****************************************************************
void Player::ChangeUpDirection(const eUpDirection& direction)
{
	switch(direction)
	{
	case eChange90:
		_upDirection++;
		break;

	case eChange270:
		_upDirection--;
		break;

	case eChange180:
		_upDirection += 2;
		break;

	default:
		break;
	}

	if( _upDirection < 0 )
	{
		_upDirection += 4;
	}

	if( _upDirection > 3 )
	{
		_upDirection -= 4;
	}
}

//****************************************************************
// �X�e�[�W�̉�]�����󂯎��A����ɉ����ăv���C���[����]������
//****************************************************************
void Player::PlayerRotationWithCube(const sStageRotationInfo& stageRotationInfo)
{
	float tmp;	// �ꎞ�ۑ��p�ϐ�

	if(stageRotationInfo.axis == 'x')
	{
		if(stageRotationInfo.rotationNum == _cubeExist.x)
		{
			if(stageRotationInfo.rotationDirect == TURN_RIGHT)
			{
				switch(_existFace)
				{
				case eTop:
					_existFace = eBack;
					ChangeUpDirection(eChange180);
					_2Dposition.x = CUBE_SIZE * 3.0f - _2Dposition.x;
					_2Dposition.y = CUBE_SIZE * 3.0f - _2Dposition.y;
					break;
				case eFront:
					_existFace = eTop;
					break;
				case eBottom:
					_existFace = eFront;
					break;
				case eBack:
					_existFace = eBottom;
					ChangeUpDirection(eChange180);
					_2Dposition.x = CUBE_SIZE * 3.0f - _2Dposition.x;
					_2Dposition.y = CUBE_SIZE * 3.0f - _2Dposition.y;
					break;

				case eRight:
					tmp = _2Dposition.x;
					_2Dposition.x = _2Dposition.y;
					_2Dposition.y = CUBE_SIZE * 3.0f - tmp;
					break;
				case eLeft:
					tmp = _2Dposition.x;
					_2Dposition.x = CUBE_SIZE * 3.0f - _2Dposition.y;
					_2Dposition.y = tmp;
					break;

				default:
					break;
				}
			}
			if(stageRotationInfo.rotationDirect == TURN_LEFT)
			{
				switch(_existFace)
				{
				case eTop:
					_existFace = eFront;
					break;
				case eFront:
					_existFace = eBottom;
					break;
				case eBottom:
					_existFace = eBack;
					ChangeUpDirection(eChange180);
					_2Dposition.x = CUBE_SIZE * 3.0f - _2Dposition.x;
					_2Dposition.y = CUBE_SIZE * 3.0f - _2Dposition.y;
					break;
				case eBack:
					_existFace = eTop;
					ChangeUpDirection(eChange180);
					_2Dposition.x = CUBE_SIZE * 3.0f - _2Dposition.x;
					_2Dposition.y = CUBE_SIZE * 3.0f - _2Dposition.y;
					break;

				case eRight:
					tmp = _2Dposition.x;
					_2Dposition.x = CUBE_SIZE * 3.0f - _2Dposition.y;
					_2Dposition.y = tmp;
					break;
				case eLeft:
					tmp = _2Dposition.x;
					_2Dposition.x = _2Dposition.y;
					_2Dposition.y = CUBE_SIZE * 3.0f - tmp;
					break;
				default:
					break;
				}
			}
		}
	}
	if(stageRotationInfo.axis == 'y')
	{
		if(stageRotationInfo.rotationNum == _cubeExist.y)
		{
			if(stageRotationInfo.rotationDirect == TURN_RIGHT)
			{
				switch(_existFace)
				{
				case eFront:
					_existFace = eLeft;
					break;
				case eRight:
					_existFace = eFront;
					break;
				case eBack:
					_existFace = eRight;
					break;
				case eLeft:
					_existFace = eBack;
					break;

				case eTop:
					tmp = _2Dposition.x;
					_2Dposition.x = _2Dposition.y;
					_2Dposition.y = CUBE_SIZE * 3.0f - tmp;
					break;
				case eBottom:
					tmp = _2Dposition.x;
					_2Dposition.x = CUBE_SIZE * 3.0f - _2Dposition.y;
					_2Dposition.y = tmp;
					break;

				default:
					break;
				}
			}
			if(stageRotationInfo.rotationDirect == TURN_LEFT)
			{
				switch(_existFace)
				{
				case eFront:
					_existFace = eRight;
					break;
				case eRight:
					_existFace = eBack;
					break;
				case eBack:
					_existFace = eLeft;
					break;
				case eLeft:
					_existFace = eFront;
					break;

				case eTop:
					tmp = _2Dposition.x;
					_2Dposition.x = CUBE_SIZE * 3.0f - _2Dposition.y;
					_2Dposition.y = tmp;
					break;
				case eBottom:
					tmp = _2Dposition.x;
					_2Dposition.x = _2Dposition.y;
					_2Dposition.y = CUBE_SIZE * 3.0f - tmp;
					break;
				default:
					break;
				}
			}
		}
	}
	if(stageRotationInfo.axis == 'z')
	{
		if(stageRotationInfo.rotationNum == _cubeExist.z)
		{
			if(stageRotationInfo.rotationDirect == TURN_RIGHT)
			{
				switch(_existFace)
				{
				case eTop:
					_existFace = eRight;
					ChangeUpDirection(eChange90);
					break;
				case eRight:
					_existFace = eBottom;
					ChangeUpDirection(eChange90);
					break;
				case eBottom:
					_existFace = eLeft;
					ChangeUpDirection(eChange90);
					break;
				case eLeft:
					_existFace = eTop;
					ChangeUpDirection(eChange90);
					break;
				default:
					break;
				}
				tmp = _2Dposition.x;
				_2Dposition.x = _2Dposition.y;
				_2Dposition.y = CUBE_SIZE * 3.0f - tmp;
			}
			if(stageRotationInfo.rotationDirect == TURN_LEFT)
			{
				switch(_existFace)
				{
				case eTop:
					_existFace = eLeft;
					ChangeUpDirection(eChange270);
					break;
				case eRight:
					_existFace = eTop;
					ChangeUpDirection(eChange270);
					break;
				case eBottom:
					_existFace = eRight;
					ChangeUpDirection(eChange270);
					break;
				case eLeft:
					_existFace = eBottom;
					ChangeUpDirection(eChange270);
					break;
				default:
					break;
				}
				tmp = _2Dposition.x;
				_2Dposition.x = CUBE_SIZE * 3.0f - _2Dposition.y;
				_2Dposition.y = tmp;
			}
		}
	}
}