#include "Stage.h"
#include "Global.h"
#include "FrontFaceMatrix.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

static const Matrix s_RotationUp	= Matrix::CreateFromAxisAngle(Vector3(1.0f, 0.0f, 0.0f), XMConvertToRadians(90.0f));
static const Matrix s_RotationDown	= Matrix::CreateFromAxisAngle(Vector3(1.0f, 0.0f, 0.0f), XMConvertToRadians(-90.0f));
static const Matrix s_RotationRight = Matrix::CreateFromAxisAngle(Vector3(0.0f, 1.0f, 0.0f), XMConvertToRadians(-90.0f));
static const Matrix s_RotationLeft	= Matrix::CreateFromAxisAngle(Vector3(0.0f, 1.0f, 0.0f), XMConvertToRadians(90.0f));

//*********************************************************************
// �R���X�g���N�^
//*********************************************************************
Stage::Stage(int* map)
{
	// �L���[�u���쐬����i���Ԃ͉E����W�n�j
	for(int x = 0; x < 3; x++)
	{
		for(int y = 0; y < 3; y++)
		{
			for(int z = 0; z < 3; z++)
			{
				_cube[x][y][z] = new Cube(x, y, z);
			}
		}
	}

	// �L���[�u�ɓ\��t���邽�߂�4�����z��
	int mappingAry[6][9][5][5];

	// 1�����z���4�����z��ɓ��꒼��
	for(int v = 0; v < 6; v++)
	{
		for(int w = 0; w < 3; w++)
		{
			for(int x = 0; x < 3; x++)
			{
				for(int y = 0; y < 5; y++)
				{
					for(int z = 0; z < 5; z++)
					{
						mappingAry[v][w * 3 + x][y][z] = map[225 * v + 75 * w + 5 * x + 15 * y + z];
					}
				}
			}
		}
	}

	// �L���[�u�̖ʂ�ʂ��Ƃɏ�����
	for(int face = 0; face < 6; face++)
	{
		for(int x = 0; x < 3; x++)
		{
			for(int y = 0; y < 3; y++)
			{
				_pCubeFace[face][x][y] = new CubeFace(face, mappingAry[face][3 * x + y]);
		
			}
		}
	}

	// ��]��������������
	_rotationInfo.rotationFlag = false;

	// �e�ʂ̒��S�ʒu����������(���Ԃ͗񋓌^�Ɠ���)
	_faceCenterPosition[eTop]		= Vector3( 0.0f,  CUBE_SIZE_HALF * 3, 0.0f );
	_faceCenterPosition[eBottom]	= Vector3( 0.0f, -CUBE_SIZE_HALF * 3, 0.0f );
	_faceCenterPosition[eRight]		= Vector3(  CUBE_SIZE_HALF * 3,	0.0f, 0.0f );
	_faceCenterPosition[eLeft]		= Vector3( -CUBE_SIZE_HALF * 3,	0.0f, 0.0f );
	_faceCenterPosition[eFront]		= Vector3( 0.0f, 0.0f,  CUBE_SIZE_HALF * 3 );
	_faceCenterPosition[eBack]		= Vector3( 0.0f, 0.0f, -CUBE_SIZE_HALF * 3 );

	// �}�g���N�X�̏�����
	_rotationMatrix = Matrix::Identity();

	//�L���[�u�̃}�g���N�X�A�h���X��ʂɘA��������
	_cube[0][2][0]->EntryCubeFace(_pCubeFace[eTop][0][0]);
	_cube[1][2][0]->EntryCubeFace(_pCubeFace[eTop][0][1]);
	_cube[2][2][0]->EntryCubeFace(_pCubeFace[eTop][0][2]);
	_cube[0][2][1]->EntryCubeFace(_pCubeFace[eTop][1][0]);
	_cube[1][2][1]->EntryCubeFace(_pCubeFace[eTop][1][1]);
	_cube[2][2][1]->EntryCubeFace(_pCubeFace[eTop][1][2]);
	_cube[0][2][2]->EntryCubeFace(_pCubeFace[eTop][2][0]);
	_cube[1][2][2]->EntryCubeFace(_pCubeFace[eTop][2][1]);
	_cube[2][2][2]->EntryCubeFace(_pCubeFace[eTop][2][2]);

	_cube[0][0][2]->EntryCubeFace(_pCubeFace[eBottom][0][0]);
	_cube[1][0][2]->EntryCubeFace(_pCubeFace[eBottom][0][1]);
	_cube[2][0][2]->EntryCubeFace(_pCubeFace[eBottom][0][2]);
	_cube[0][0][1]->EntryCubeFace(_pCubeFace[eBottom][1][0]);
	_cube[1][0][1]->EntryCubeFace(_pCubeFace[eBottom][1][1]);
	_cube[2][0][1]->EntryCubeFace(_pCubeFace[eBottom][1][2]);
	_cube[0][0][0]->EntryCubeFace(_pCubeFace[eBottom][2][0]);
	_cube[1][0][0]->EntryCubeFace(_pCubeFace[eBottom][2][1]);
	_cube[2][0][0]->EntryCubeFace(_pCubeFace[eBottom][2][2]);

	_cube[2][2][2]->EntryCubeFace(_pCubeFace[eRight][0][0]);
	_cube[2][2][1]->EntryCubeFace(_pCubeFace[eRight][0][1]);
	_cube[2][2][0]->EntryCubeFace(_pCubeFace[eRight][0][2]);
	_cube[2][1][2]->EntryCubeFace(_pCubeFace[eRight][1][0]);
	_cube[2][1][1]->EntryCubeFace(_pCubeFace[eRight][1][1]);
	_cube[2][1][0]->EntryCubeFace(_pCubeFace[eRight][1][2]);
	_cube[2][0][2]->EntryCubeFace(_pCubeFace[eRight][2][0]);
	_cube[2][0][1]->EntryCubeFace(_pCubeFace[eRight][2][1]);
	_cube[2][0][0]->EntryCubeFace(_pCubeFace[eRight][2][2]);

	_cube[0][2][0]->EntryCubeFace(_pCubeFace[eLeft][0][0]);
	_cube[0][2][1]->EntryCubeFace(_pCubeFace[eLeft][0][1]);
	_cube[0][2][2]->EntryCubeFace(_pCubeFace[eLeft][0][2]);
	_cube[0][1][0]->EntryCubeFace(_pCubeFace[eLeft][1][0]);
	_cube[0][1][1]->EntryCubeFace(_pCubeFace[eLeft][1][1]);
	_cube[0][1][2]->EntryCubeFace(_pCubeFace[eLeft][1][2]);
	_cube[0][0][0]->EntryCubeFace(_pCubeFace[eLeft][2][0]);
	_cube[0][0][1]->EntryCubeFace(_pCubeFace[eLeft][2][1]);
	_cube[0][0][2]->EntryCubeFace(_pCubeFace[eLeft][2][2]);

	_cube[0][2][2]->EntryCubeFace(_pCubeFace[eFront][0][0]);
	_cube[1][2][2]->EntryCubeFace(_pCubeFace[eFront][0][1]);
	_cube[2][2][2]->EntryCubeFace(_pCubeFace[eFront][0][2]);
	_cube[0][1][2]->EntryCubeFace(_pCubeFace[eFront][1][0]);
	_cube[1][1][2]->EntryCubeFace(_pCubeFace[eFront][1][1]);
	_cube[2][1][2]->EntryCubeFace(_pCubeFace[eFront][1][2]);
	_cube[0][0][2]->EntryCubeFace(_pCubeFace[eFront][2][0]);
	_cube[1][0][2]->EntryCubeFace(_pCubeFace[eFront][2][1]);
	_cube[2][0][2]->EntryCubeFace(_pCubeFace[eFront][2][2]);

	_cube[2][2][0]->EntryCubeFace(_pCubeFace[eBack][0][0]);
	_cube[1][2][0]->EntryCubeFace(_pCubeFace[eBack][0][1]);
	_cube[0][2][0]->EntryCubeFace(_pCubeFace[eBack][0][2]);
	_cube[2][1][0]->EntryCubeFace(_pCubeFace[eBack][1][0]);
	_cube[1][1][0]->EntryCubeFace(_pCubeFace[eBack][1][1]);
	_cube[0][1][0]->EntryCubeFace(_pCubeFace[eBack][1][2]);
	_cube[2][0][0]->EntryCubeFace(_pCubeFace[eBack][2][0]);
	_cube[1][0][0]->EntryCubeFace(_pCubeFace[eBack][2][1]);
	_cube[0][0][0]->EntryCubeFace(_pCubeFace[eBack][2][2]);

	// ��]���t���O��������
	_moveFlag = false;

	// �^�C��������
	_time = 0;

	// �`�揇�Ǘ��z���������
	for(int i = 0; i < 6; i++)
	{
		_renderMgr[i].sFace = i;
		_renderMgr[i].sDistance = 0.0f;
		_renderMgr[i].sFaceCenterPosition = _faceCenterPosition[i];
	}

	// �ʂ��Ƃɐ�������
	AllAppoint(_pRenderFace[0], 0, Y, 2);
	AllAppoint(_pRenderFace[1], 1, Y, 0);
	AllAppoint(_pRenderFace[2], 2, X, 2);
	AllAppoint(_pRenderFace[3], 3, X, 0);
	AllAppoint(_pRenderFace[4], 4, Z, 2);
	AllAppoint(_pRenderFace[5], 5, Z, 0);

	// 2D�X�e�[�W�̏�����
	_pStage2D = new Stage2D();
	_pStage2D->CreateStage(map);
}

//*********************************************************************
// �f�X�g���N�^
//*********************************************************************
Stage::~Stage()
{
	// �L���[�u�̃f�X�g���N�^
	for(int x = 0; x < 3; x++)
	{
		for(int y = 0; y < 3; y++)
		{
			for(int z = 0; z < 3; z++)
			{
				delete _cube[x][y][z];
			}
		}
	}

	// �L���[�u�̖ʂ��f�X�g���N�^
	for(int face = 0; face < 6; face++)
	{
		for(int y = 0; y < 3; y++)
		{
			for(int x = 0; x < 3; x++)
			{
				delete _pCubeFace[face][y][x];
			}
		}
	}
	delete _pStage2D;
}

//*********************************************************************
// ����������
//*********************************************************************
void Stage::Initialize()
{
	
}

//*********************************************************************
// �X�V����
//*********************************************************************
void Stage::Update()
{
	// ��]���̃t���O�������Ă��鎞�̏���
	if(_moveFlag == true)
	{
		_time++;

		// ��]�̊������o��
		_moveDegree = (float)_time / MOVE_FLAME;

		// ��]�I����
		if(_time > MOVE_FLAME)
		{
			_moveFlag = false;
			_time = 0;

			for(int x = 0; x < 3; x++)
			{
				for(int y = 0; y < 3; y++)
				{
					for(int z = 0; z < 3; z++)
					{
						
						_cube[x][y][z]->ResetRotationFlag();
						
					}
				}
			}
		}
	}
	for(int x = 0; x < 3; x++)
	{
		for(int y = 0; y < 3; y++)
		{
			for(int z = 0; z < 3; z++)
			{
				_cube[x][y][z]->Update(_moveDegree);
			}
		}
	}
	
	// �J�����Ɗe�ʂ̒��S�Ƃ̋������v�Z����
	for(int i = 0; i < 6; i++)
	{
		_renderMgr[i].sDistance = Vector3::Distance( g_Camera->GetEyePosition(), _renderMgr[i].sFaceCenterPosition);
	}

	// �߂����ɕ��ёւ���
	FaceRemderMgr tmpNear;
	for(int i = 0; i < 5; i++)
	{
		for(int j = i + 1; j < 6; j++)
		{
			if(_renderMgr[i].sDistance < _renderMgr[j].sDistance)
			{
				tmpNear = _renderMgr[i];
				_renderMgr[i] = _renderMgr[j];
				_renderMgr[j] = tmpNear;
			}
		}
	}

	// �ʂ��Ƃɐ�������
	AllAppoint(_pRenderFace[0], 0, 2, Y);
	AllAppoint(_pRenderFace[1], 1, 0, Y);
	AllAppoint(_pRenderFace[2], 2, 2, X);
	AllAppoint(_pRenderFace[3], 3, 0, X);
	AllAppoint(_pRenderFace[4], 4, 2, Z);
	AllAppoint(_pRenderFace[5], 5, 0, Z);
}

//*********************************************************************
// �`�揈��
//*********************************************************************
void Stage::Render(const int& playerExist)
{
	Matrix l_frontFaceMatrix;

	// �v���C���[�̑��݂���ʂɂ���āA�n���}�g���N�X��ւ���
	switch( playerExist )
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

	//�`��
	for(int i = 0; i < 6; i++)
	{
		for(int j = 0; j < 9; j++)
		{
			_pRenderFace[_renderMgr[i].sFace][j]->Render(_rotationMatrix, l_frontFaceMatrix);
		}
	}
}
//*********************************************************************
// �L���[�u��]����
//*********************************************************************
bool Stage::RotationCube(const int& playerExistFace, const int& playerUpDirect )
{
	if( g_InputManager->IsButtonDown(XINPUT_GAMEPAD_RIGHT_SHOULDER))
	{
		// �㑤�̎��v��]
		if( g_InputManager->IsButtonPressed(XINPUT_GAMEPAD_LEFT_SHOULDER))
		{
			return HorizontallyRotation(playerExistFace, playerUpDirect, 2, TURN_RIGHT);
		}

		// �E���̎��v��]
		if( g_InputManager->IsR2Pressed() )
		{
			return VerticalRotation(playerExistFace, playerUpDirect, 2, TURN_RIGHT);
		}
	}

	if( g_InputManager->IsButtonDown(XINPUT_GAMEPAD_LEFT_SHOULDER))
	{
		// �㑤�̔����v��]
		if( g_InputManager->IsButtonPressed(XINPUT_GAMEPAD_RIGHT_SHOULDER) )
		{
			return HorizontallyRotation(playerExistFace, playerUpDirect, 2, TURN_LEFT);
		}

		// �����̎��v��]
		if( g_InputManager->IsL2Pressed() )
		{
			return VerticalRotation(playerExistFace, playerUpDirect, 0, TURN_RIGHT);
		}
	}

	if( g_InputManager->IsR2Down() )
	{
		// �E���̔����v��]
		if( g_InputManager->IsButtonPressed(XINPUT_GAMEPAD_RIGHT_SHOULDER) )
		{
			return VerticalRotation(playerExistFace, playerUpDirect, 2, TURN_LEFT);
		}

		// �����̔����v��]
		if( g_InputManager->IsL2Pressed() )
		{
			return HorizontallyRotation(playerExistFace, playerUpDirect, 0, TURN_RIGHT);
		}
	}

	if( g_InputManager->IsL2Down() )
	{
		// �����̔����v��]
		if( g_InputManager->IsButtonPressed(XINPUT_GAMEPAD_LEFT_SHOULDER) )
		{
			return VerticalRotation(playerExistFace, playerUpDirect, 0, TURN_LEFT);
		}

		// �����̎��v��]
		if( g_InputManager->IsR2Pressed() )
		{
			return HorizontallyRotation(playerExistFace, playerUpDirect, 0,TURN_LEFT);
		}
	}

	// ����]����
	if( g_InputManager->IsButtonPressed(XINPUT_GAMEPAD_Y) )
	{
		RotationCubeX(1, TURN_RIGHT);
		return true;
	}

	if( g_InputManager->IsButtonPressed(XINPUT_GAMEPAD_A) )
	{
		RotationCubeX(1, TURN_LEFT);
		return true;
	}

	if( g_InputManager->IsButtonPressed(XINPUT_GAMEPAD_X) )
	{
		RotationCubeY(1, TURN_RIGHT);
		return true;
	}

	if( g_InputManager->IsButtonPressed(XINPUT_GAMEPAD_B) )
	{
		RotationCubeY(1, TURN_LEFT);
		return true;
	}

	// �f�o�b�O���̉�����
#ifdef _DEBUG


	if( g_InputManager->IsKeyDown('U'))
	{
		// �㑤�̎��v��]
		if(  g_InputManager->IsKeyPressed('T'))
		{
			RotationCubeY(2, TURN_RIGHT);
			return true;
		}

		// �E���̎��v��]
		if(  g_InputManager->IsKeyPressed('M'))
		{
			RotationCubeX(2, TURN_RIGHT);
			return true;
		}
	}

	if( g_InputManager->IsKeyDown('T'))
	{
		// �㑤�̔����v��]
		if( g_InputManager->IsKeyPressed('U') )
		{
			RotationCubeY(2, TURN_LEFT);
			return true;
		}

		// �����̎��v��]
		if( g_InputManager->IsKeyPressed('B') )
		{
			RotationCubeX(0, TURN_RIGHT);
			return true;
		}
	}

	if( g_InputManager->IsKeyDown('M') )
	{
		// �E���̔����v��]
		if( g_InputManager->IsKeyPressed('U') )
		{
			RotationCubeX(2, TURN_LEFT);
			return true;
		}

		// �����̔����v��]
		if( g_InputManager->IsKeyPressed('B') )
		{
			RotationCubeY(0, TURN_RIGHT);
			return true;
		}
	}

	if( g_InputManager->IsKeyDown('B') )
	{
		// �E���̔����v��]
		if( g_InputManager->IsKeyPressed('T') )
		{
			RotationCubeX(0, TURN_LEFT);
			return true;
		}

		// �����̔����v��]
		if( g_InputManager->IsKeyPressed('M') )
		{
			RotationCubeY(0, TURN_LEFT);
			return true;
		}
	}

	// ����]����
	if( g_InputManager->IsKeyDown('Y') )
	{
		RotationCubeX(1, TURN_RIGHT);
		return true;
	}

	if( g_InputManager->IsKeyDown('N') )
	{
		RotationCubeX(1, TURN_LEFT);
		return true;
	}

	if( g_InputManager->IsKeyDown('G') )
	{
		RotationCubeY(1, TURN_RIGHT);
		return true;
	}

	if( g_InputManager->IsKeyDown('J') )
	{
		RotationCubeY(1, TURN_LEFT);
		return true;
	}

#endif
	
	return false;
}

//*********************************************************************
// �w�肵���ʂɑ�����L���[�u�̖ʔz��̐擪�A�h���X��Ԃ�
//*********************************************************************
CubeFace** Stage::GetAppointCubeFace(int appointDirection)
{	
	switch(appointDirection)
	{
	case eTop:
		return _pRenderFace[0];
		break;

	case eBottom:
		return _pRenderFace[1];
		break;

	case eRight:
		return _pRenderFace[2]; 
		break;

	case eLeft:
		return _pRenderFace[3];
		break;

	case eFront:
		return _pRenderFace[4];
		break;

	case eBack:
		return _pRenderFace[5];
		break;

	default:
		break;
	}
	
	return nullptr;
}

// �w��ʂ̃L���[�u�z���Ԃ��ۂɎg�p���鑍���菈��
void Stage::AllAppoint(CubeFace** face, int appointDirection, int cubePosition, int searchPosition)
{
	int i = 0;
	for(int x = 0; x < 3; x++)
	{
		for(int y = 0; y < 3; y++)
		{
			for(int z = 0; z < 3; z++)
			{
				if(_cube[x][y][z]->GetCurrentPoaition()[searchPosition] == cubePosition)
				{
					face[i] = _cube[x][y][z]->GetAppointCubeFace(appointDirection);
					i++;
				}
			}
		}
	}
	return;
}

//****************************************************************************
// �L���[�u��]����(�����S�ŁA���_����̌�)
//****************************************************************************
void Stage::RotationCubeX(int num, const int& turn_direction)
{	
	// ���łɓ��͂��m�肳��Ă���ꍇ�A�X�V���Ȃ�
	if(_rotationInfo.rotationFlag == true) return;

	for(int x = 0; x < 3; x++)
	{
		for(int y = 0; y < 3; y++)
		{
			for(int z = 0; z < 3; z++)
			{
				if(_cube[x][y][z]->GetCurrentPoaition()[0] == num)  
				{
					_cube[x][y][z]->TransPosition(eRotation_x, turn_direction, _rotationMatrix);
				}
			}
		}
	}

	// �����蔻�����
	_pStage2D->RotationCubeX(num, turn_direction);

	// �v���C���[�ɉ�]������`���邽�߁A��]�����X�V����
	_rotationInfo.rotationFlag = true;
	_rotationInfo.axis = 'x';
	_rotationInfo.rotationDirect = turn_direction;
	_rotationInfo.rotationNum = num;
}
void Stage::RotationCubeY(int num, const int& turn_direction)
{
	if(_rotationInfo.rotationFlag == true) return;

	for(int x = 0; x < 3; x++)
	{
		for(int y = 0; y < 3; y++)
		{
			for(int z = 0; z < 3; z++)
			{
				if(_cube[x][y][z]->GetCurrentPoaition()[1] == num)  
					_cube[x][y][z]->TransPosition(eRotation_y, turn_direction, _rotationMatrix);
			}
		}
	}

	// �����蔻�����
	_pStage2D->RotationCubeY(num, turn_direction);

	// �v���C���[�ɉ�]������`���邽�߁A��]�����X�V����
	_rotationInfo.rotationFlag = true;
	_rotationInfo.axis = 'y';
	_rotationInfo.rotationDirect = turn_direction;
	_rotationInfo.rotationNum = num;
}
void Stage::RotationCubeZ(int num, const int& turn_direction)
{
	if(_rotationInfo.rotationFlag == true) return;

	for(int x = 0; x < 3; x++)
	{
		for(int y = 0; y < 3; y++)
		{
			for(int z = 0; z < 3; z++)
			{
				if(_cube[x][y][z]->GetCurrentPoaition()[2] == num)  
					_cube[x][y][z]->TransPosition(eRotation_z, turn_direction, _rotationMatrix);
			}
		}
	}

	// �����蔻�����
	_pStage2D->RotationCubeZ(num, turn_direction);

	// �v���C���[�ɉ�]������`���邽�߁A��]�����X�V����
	_rotationInfo.rotationFlag = true;
	_rotationInfo.axis = 'z';
	_rotationInfo.rotationDirect = turn_direction;
	_rotationInfo.rotationNum = num;
}

// �}�g���N�X�𑀍삷��
void Stage::ChangeMatrix(eRotationMatrix rotationDirect)
{
	switch(rotationDirect)
	{
	case eRotationUp:
		_rotationMatrix *= s_RotationUp;
		break;

	case eRotationDown:
		_rotationMatrix *= s_RotationDown;
		break;

	case eRotationRight:
		_rotationMatrix *= s_RotationRight;
		break;

	case eRotationLeft:
		_rotationMatrix *= s_RotationLeft;
		break;

	default:
		break;
	}
}

// ��]�����ǂ������擾����
bool Stage::GetMovingFlag()
{
	return _moveFlag;
}

// ��]���t���O���擾���A�t���O��|��
bool Stage::GetRotationInfoFlag()
{
	bool tmpFlag = _rotationInfo.rotationFlag;
	_rotationInfo.rotationFlag = false;
	return tmpFlag;
}

// �O���p�L���[�u��]����
void Stage::Rotation(const int& playerExistFace, const int& playerUpDirect)
{
	if(_moveFlag == false)
	{
		_moveFlag = RotationCube(playerExistFace, playerUpDirect);
	}
}

// �v���C���[�̑��ݖʁA��������l�������c��]���s��(���ʁA������A�E���E��]�����Ƃ���)
bool Stage::VerticalRotation(const int& playerExistFace, const int& playerUpDirect, const int& rotationNum, const int& rotationDirect)
{
	if( playerUpDirect == eUpDirect &&
		(playerExistFace == eTop ||
		playerExistFace == eFront ||
		playerExistFace == eBottom) )
	{
		RotationCubeX(rotationNum, rotationDirect);
		return true;
	}
	if( playerUpDirect == eDownDirect &&
		(playerExistFace == eTop ||
		playerExistFace == eFront ||
		playerExistFace == eBottom) )
	{
		RotationCubeX(ReverseRotationNum(rotationNum), ReverseRotationDirect(rotationDirect));
		return true;
	}
			
	if( playerUpDirect == eRightDirect &&
		(playerExistFace == eFront ||
		playerExistFace == eRight ||
		playerExistFace == eBack ||
		playerExistFace == eLeft) )
	{
		RotationCubeY(ReverseRotationNum(rotationNum), ReverseRotationDirect(rotationDirect));
		return true;
	}
	if( playerUpDirect == eLeftDirect &&
		(playerExistFace == eFront ||
		playerExistFace == eRight ||
		playerExistFace == eBack ||
		playerExistFace == eLeft) )
	{
		RotationCubeY(rotationNum, rotationDirect);
		return true;
	}

	// ��C�ɂł��Ȃ������c��̏���
	if(playerExistFace == eTop)
	{
		if(playerUpDirect == eRightDirect) RotationCubeZ(rotationNum, rotationDirect);
		if(playerUpDirect == eLeftDirect) RotationCubeZ(ReverseRotationNum(rotationNum), ReverseRotationDirect(rotationDirect));
		return true;
	}
	if(playerExistFace == eBottom)
	{
		if(playerUpDirect == eRightDirect) RotationCubeZ(ReverseRotationNum(rotationNum), ReverseRotationDirect(rotationDirect));
		if(playerUpDirect == eLeftDirect) RotationCubeZ(rotationNum, rotationDirect);
		return true;
	}
	if(playerExistFace == eBack)
	{
		if(playerUpDirect == eUpDirect) RotationCubeX(ReverseRotationNum(rotationNum), ReverseRotationDirect(rotationDirect));
		if(playerUpDirect == eDownDirect ) RotationCubeX(rotationNum, rotationDirect);
		return true;
	}
	if(playerExistFace == eRight)
	{
		if(playerUpDirect == eUpDirect) RotationCubeZ(ReverseRotationNum(rotationNum), ReverseRotationDirect(rotationDirect));
		if(playerUpDirect == eDownDirect) RotationCubeZ(rotationNum, rotationDirect);
		return true;
	}
	if(playerExistFace == eLeft)
	{
		if(playerUpDirect == eUpDirect) RotationCubeZ(rotationNum, rotationDirect);
		if(playerUpDirect == eDownDirect) RotationCubeZ(ReverseRotationNum(rotationNum), ReverseRotationDirect(rotationDirect));
		return true;
	}
}

// ����]���s��
bool Stage::HorizontallyRotation(const int& playerExistFace, const int& playerUpDirect, const int& rotationNum, const int& rotationDirect)
{
	if( playerUpDirect == eUpDirect && 
		(playerExistFace == eFront ||
		 playerExistFace == eRight ||
		 playerExistFace == eBack ||
		 playerExistFace == eLeft ))
	{
		//				2           right
		RotationCubeY(rotationNum, rotationDirect);
		return true;
	}
	if( playerUpDirect == eDownDirect && 
		(playerExistFace == eFront ||
			playerExistFace == eRight ||
			playerExistFace == eBack ||
			playerExistFace == eLeft))
	{
		RotationCubeY(ReverseRotationNum(rotationNum), ReverseRotationDirect(rotationDirect));
		return true;
	}
	if( playerExistFace == eFront )
	{
		if(playerUpDirect == eRightDirect) RotationCubeX(rotationNum, rotationDirect); 
		if(playerUpDirect == eLeftDirect ) RotationCubeX(ReverseRotationNum(rotationNum), ReverseRotationDirect(rotationDirect));
	}
	if( playerExistFace == eRight )
	{
		if(playerUpDirect == eRightDirect) RotationCubeZ(ReverseRotationNum(rotationNum), ReverseRotationDirect(rotationDirect)); 
		if(playerUpDirect == eLeftDirect ) RotationCubeZ(rotationNum, rotationDirect);
	}
	if( playerExistFace == eBack )
	{
		if(playerUpDirect == eRightDirect) RotationCubeX(ReverseRotationNum(rotationNum), ReverseRotationDirect(rotationDirect)); 
		if(playerUpDirect == eLeftDirect ) RotationCubeX(rotationNum, rotationDirect);
	}
	if( playerExistFace == eLeft )
	{
		if(playerUpDirect == eRightDirect) RotationCubeZ(rotationNum, rotationDirect);
		if(playerUpDirect == eLeftDirect ) RotationCubeZ(ReverseRotationNum(rotationNum), ReverseRotationDirect(rotationDirect));
	}
	if( playerExistFace == eTop )
	{
		if(playerUpDirect == eUpDirect ) RotationCubeZ(ReverseRotationNum(rotationNum), ReverseRotationDirect(rotationDirect));
		if(playerUpDirect == eRightDirect) RotationCubeX(rotationNum, rotationDirect);
		if(playerUpDirect == eDownDirect ) RotationCubeZ(rotationNum, rotationDirect);
		if(playerUpDirect == eLeftDirect ) RotationCubeX(ReverseRotationNum(rotationNum), ReverseRotationDirect(rotationDirect));
	}
	if( playerExistFace == eBottom )
	{
		if(playerUpDirect == eUpDirect ) RotationCubeZ(rotationNum, rotationDirect);
		if(playerUpDirect == eRightDirect) RotationCubeX(rotationNum, rotationDirect);
		if(playerUpDirect == eDownDirect ) RotationCubeZ(ReverseRotationNum(rotationNum), ReverseRotationDirect(rotationDirect));
		if(playerUpDirect == eLeftDirect ) RotationCubeX(ReverseRotationNum(rotationNum), ReverseRotationDirect(rotationDirect));
	}
	return true;
}

// �����ɓ��͂��ꂽ��]�����̋t��߂�l�Ƃ���
int Stage::ReverseRotationDirect(const int& rotationDirect)
{
	return rotationDirect * -1;
}

// �����ɓ��͂��ꂽ�L���[�u�i���o�[�𔽓]������
int Stage::ReverseRotationNum(const int& rotationNum)
{
	return 2 - rotationNum;
}