#include "Cube.h"
#include "Global.h"
#include "SModelMgr.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

// float����int�֌덷���C�����ĕϊ�����
int TransIntFromFloat(float f)
{
	if(f < 0.0f)
	{
		return (int)(f - 0.2f);
	}

	if(f > 0.0f)
	{
		return (int)(f + 0.2f);
	}

	return 0;
}

//*********************************************************************
// �R���X�g���N�^
//*********************************************************************
Cube::Cube()
{
	
}

Cube::Cube(int x, int y, int z) :
	_movingFlag(false)
{
	// �L���[�u�ʃ|�C���^�̏�����
	for(int n = 0; n < 3; n++)
	{
		_pCubeFace[n] = nullptr;
	}

	// �ʒu�̏�����
	_currentPosition[0] = _originPosition[0] = x;
	_currentPosition[1] = _originPosition[1] = y;
	_currentPosition[2] = _originPosition[2] = z;

	// �p�x�̏�����
	for(int i = 0; i < 3; i++)
	{
		_currentRadian[i] = 0.0f;
	}

	// �}�g���N�X�̏�����
	_rotation_matrix = Matrix::Identity();
}

//*********************************************************************
// �f�X�g���N�^
//*********************************************************************
Cube::~Cube()
{

}


//*********************************************************************
// �X�V����
//*********************************************************************
void Cube::Update(float moveDegree)
{
	if(_movingFlag == true)
	{
		Quaternion q1 = Quaternion::CreateFromRotationMatrix(_before_roation_matrix);
		Quaternion q2 = Quaternion::CreateFromRotationMatrix(_rotation_matrix);
		Quaternion q3 = Quaternion::Slerp(q1, q2, moveDegree);

		_moveRotationMatrix = Matrix::CreateFromQuaternion(q3);
	}
}


//*********************************************************************
// �`�揈��
//*********************************************************************
// �~�܂��Ă��鎞�̕`��
void Cube::Render()
{
	
	// �o�^����Ă���L���[�u��`�悷��
	for(int i = 0; i < 3; i++)
	{
		// �L���[�u���o�^����Ă��Ȃ������甲����
		if(_pCubeFace[i] == nullptr) break;

		// �L���[�u�`��
		//_pCubeFace[i]->Render();
	}
	
}

// �����Ă��鎞�̕`��
void Cube::RenderMove(float moveDegree)
{
	if(_movingFlag == true)
	{
		// �o�^����Ă���L���[�u��`�悷��
		for(int i = 0; i < 3; i++)
		{
			// �L���[�u���o�^����Ă��Ȃ������甲����
			if(_pCubeFace[i] == nullptr) break;

			// �L���[�u�`��
			//_pCubeFace[i]->Render();
		}
	}else{
		Render();
	}
}

//*********************************************************************
// �L���[�u�ʂ̓o�^
//*********************************************************************
void Cube::EntryCubeFace(CubeFace* cubeFace)
{
	for(int n = 0; n < 3; n++)
	{
		if(_pCubeFace[n] == nullptr)
		{
			//�@�K�v�ȏ��̃|�C���^��n��
			_pCubeFace[n] = cubeFace;
			_pCubeFace[n]->SetParentMatrix(&_rotation_matrix);
			_pCubeFace[n]->SetParentMovingMatrix(&_moveRotationMatrix);
			_pCubeFace[n]->SetParentPosition(_originPosition);
			_pCubeFace[n]->SetParentRotaionFlag(&_movingFlag);
			break;
		}
	}
}

//*********************************************************************
// �w�肵���ʂɑ�����CubeFace�|�C���^��Ԃ�
//*********************************************************************
CubeFace* Cube::GetAppointCubeFace(int appointFace)
{
	for(int i = 0; i < 3, _pCubeFace[i] != nullptr; i++)
	{
		if(_pCubeFace[i]->GetFaceDirection() == appointFace)
		{
			return _pCubeFace[i];
		}
	}

	return nullptr;
}

//*********************************************************************
// ��]�ɂ��ʒu���̉���
//*********************************************************************
void Cube::TransPosition(eRotation rotation, const int& turn_direction, Matrix& worldMatrix)
{
	// ��]�̒��S
	Vector3 tmp, axis(0.0f, 0.0f, 0.0f);

	switch(rotation)
	{
	case eRotation_x:
		_currentRadian[0] += XMConvertToRadians(90.0f);
		// �ύX�O�̃}�g���N�X��ێ�
		_before_roation_matrix = _rotation_matrix;
		_rotation_matrix *= Matrix::CreateFromAxisAngle(AXIS_X, XMConvertToRadians(90.0f) * turn_direction);
		axis.x = 1.0f;
		// �A�����Ă���L���[�u�̖ʏ���ύX
		for(int i = 0; i < 3, _pCubeFace[i] != nullptr; i++)
		{
			_pCubeFace[i]->ChangeFaceDirectionX(turn_direction);
			_pCubeFace[i]->SetCubeFaceRenderFlag();
		}
		break;

	case eRotation_y:
		_currentRadian[1] += XMConvertToRadians(90.0f);
		// �ύX�O�̃}�g���N�X��ێ�
		_before_roation_matrix = _rotation_matrix;
		_rotation_matrix *= Matrix::CreateFromAxisAngle(AXIS_Y, XMConvertToRadians(90.0f) * turn_direction);
		axis.y = 1.0f;
		// �A�����Ă���L���[�u�̖ʏ���ύX
		for(int i = 0; i < 3, _pCubeFace[i] != nullptr; i++)
		{
			_pCubeFace[i]->ChangeFaceDirectionY(turn_direction);
			_pCubeFace[i]->SetCubeFaceRenderFlag();
		}
		break;

	case eRotation_z:
		_currentRadian[2] += XMConvertToRadians(90.0f);
		// �ύX�O�̃}�g���N�X��ێ�
		_before_roation_matrix = _rotation_matrix;
		_rotation_matrix *= Matrix::CreateFromAxisAngle(AXIS_Z, XMConvertToRadians(90.0f) * turn_direction);
		axis.z = 1.0f;
		// �A�����Ă���L���[�u�̖ʏ���ύX
		for(int i = 0; i < 3, _pCubeFace[i] != nullptr; i++)
		{
			_pCubeFace[i]->ChangeFaceDirectionZ(turn_direction);
			_pCubeFace[i]->SetCubeFaceRenderFlag();
		}
		break;

	default:
		break;
	}

	// �ʒu�����X�V
	Vector3::Transform(	Vector3(_currentPosition[0] - 1.0f, _currentPosition[1] - 1.0f, _currentPosition[2] - 1.0f), 
			Matrix::CreateFromAxisAngle(axis, XMConvertToRadians(90.0f * turn_direction)),
			tmp);

	//Vector3::Transform( tmp, worldMatrix, tmp );

	// float��int�Ɋm���ɕϊ�����
	_currentPosition[0] = TransIntFromFloat(tmp.x) + 1;
	_currentPosition[1] = TransIntFromFloat(tmp.y) + 1;
	_currentPosition[2] = TransIntFromFloat(tmp.z) + 1;

	_movingFlag = true;

	Matrix position = Matrix::CreateTranslation(	(float)CUBE_SIZE * (_currentPosition[0] - 1),
													(float)CUBE_SIZE * (_currentPosition[1] - 1),
													(float)CUBE_SIZE * (_currentPosition[2] - 1));

}

