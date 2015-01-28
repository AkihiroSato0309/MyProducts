//***************************************************************************************************
//
// Cube.h				�������m  12��16��
//
//***************************************************************************************************
#pragma once
#include "CubeFace.h"

static const int CUBE_SIZE = BLOCK_SIZE * 5;			// �L���[�u�̃T�C�Y
static const float CUBE_SIZE_HALF = CUBE_SIZE * 0.5f;	// �L���[�u�̔���

// ���̃x�N�g��
static const DirectX::SimpleMath::Vector3 AXIS_X(1.0f, 0.0f, 0.0f);
static const DirectX::SimpleMath::Vector3 AXIS_Y(0.0f, 1.0f, 0.0f);
static const DirectX::SimpleMath::Vector3 AXIS_Z(0.0f, 0.0f, 1.0f);

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �L���[�u�N���X
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Cube
{
public:
	Cube();
	Cube(int x, int y, int z);
	~Cube();

	// �X�V
	void Update(float moveDegree);
	
	// �`��
	void Render();

	// ��]���̕`��
	void RenderMove(float moveFlame);

	// ��]�ɂ��ʒu���̉���
	void TransPosition(eRotation rotation, const int& turn_direction, DirectX::SimpleMath::Matrix& worldMatrix);


	// ���݂̈ʒu���擾
	int* GetCurrentPoaition()
	{
		return _currentPosition;
	}

	// �}�g���N�X���擾����
	const DirectX::SimpleMath::Matrix& GetCurrentMatrix(){return _rotation_matrix;}
	const DirectX::SimpleMath::Matrix& GetBeforeMatrix(){return _before_roation_matrix;}

	// �ʒu���Z�b�g����
	void SetCurrentPosition(int x, int y, int z)
	{
		_currentPosition[0] = x;
		_currentPosition[1] = y;
		_currentPosition[2] = z;
	}

	// ��]�t���O�����Z�b�g����
	void ResetRotationFlag(){_movingFlag = false;}

	// �}�g���N�X�̃A�h���X���擾
	DirectX::SimpleMath::Matrix* GetMatrix()
	{
		return &_rotation_matrix;
	}

	// �L���[�u�̓o�^
	void EntryCubeFace(CubeFace* cubeFace);

	// �w�肵���ʂɑ�����CubeFace�|�C���^��Ԃ�
	CubeFace* GetAppointCubeFace(int appointFace);

private:
	// �o�^������L���[�u�ʂ̃|�C���^�i�ő�3�j
	CubeFace* _pCubeFace[3];

	// ���f���ϐ�
	DirectX::Model* model;

	// ��]���������̃}�g���N�X��ۑ�
	DirectX::SimpleMath::Matrix _rotation_matrix;
	DirectX::SimpleMath::Matrix _before_roation_matrix;
	
	// ��]���Ɏg�p����}�g���N�X
	DirectX::SimpleMath::Matrix _moveRotationMatrix;

	// �����Ă��鎞�̃t���O
	bool _movingFlag;

	// �ʒu���Ǘ� 0�`2(���_����̌��ŕ\���@0:x, 1:y, 2:z)
	int _currentPosition[3];
	int _originPosition[3];

	// ��]�p�x���(X,Y,Z�̎��𒆐S�Ƃ����p�x)
	float _currentRadian[3];

};