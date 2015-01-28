//******************************************************************************
//
//  ���삷��v���C���[�N���X
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
	// �R���X�g���N�^
	Player();
	// �f�X�g���N�^
	~Player();

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �X�V����
	// ������	: �w�肵���}�b�v�̏��i�����蔻��Ɏg�p�j
	// ������	: �L���[�u����]�����ǂ���
	// �߂�l	: �v���C���[���S�[�����Ă�����true
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	bool Update(int* map, const bool& cubeMovingFlag);

	// �`�揈��
	void Render();

	// ���݂��Ă���ʂ��擾����
	int GetExistFace(){return _existFace;}

	// �v���C���[�̏�������擾����
	int GetUpDirection(){return _upDirection;}

	// �E�ֈړ�
	void MoveRight();

	// ���ֈړ�
	void MoveLeft();

	// ��ֈړ�
	void MoveUp();

	// ���ֈړ�
	void MoveDown();

	// �W�����v
	void Jump();

	// �}�g���N�X�����Z����
	void GetMatrixChanger(IStageMatrixChanger* matrixChanger)
	{
		_pMatrixChanger = matrixChanger;
	}

	// �X�e�[�W�̉�]�����󂯎��A����ɉ����ăv���C���[����]������
	void PlayerRotationWithCube(const sStageRotationInfo& stageRotationInfo);

	// �v���C���[�̈ړ��t���O���擾����
	bool GetMovingFlag()
	{
		return _movingFlag;
	}

private:
	// �v���C���[�摜
	ID3D11ShaderResourceView* _pTexture;

	// ���݂��Ă���ʏ��
	int _existFace;

	// �v���C���[���L���[�u��̂ǂ��ɑ��݂��Ă��邩(X, Y, Z)
	sIntVector3 _cubeExist;

	// ���݂��Ă���ʂɑ΂��āA�v���C���[���ǂ̕�������Ƃ��Ă��邩
	int _upDirection;

	// ������̏�����]������
	void ChangeUpDirection(const eUpDirection& direction);

	// �ʒu���(2D)
	DirectX::SimpleMath::Vector2 _2Dposition;

	// �ʒu���(3D)
	DirectX::SimpleMath::Vector3 _3Dposition;

	// �ړ��x�N�g��
	DirectX::SimpleMath::Vector2 _2DMoveVector;

	// �ʕ����ɑ΂��Ẳ�]�}�g���N�X
	DirectX::SimpleMath::Matrix _rotaionMatrix;

	// �}�g���N�X��ω�������
	IStageMatrixChanger* _pMatrixChanger;

	// �v���C���[���ړ����Ă����true,�~�܂��Ă����false
	bool _movingFlag;

	// �}�b�v���l�����Ĉړ�������
	bool Move(float* x, float* y, float* jump_power, float move_x, float move_y, bool* jump_flag, int map[][15]);

	// �}�b�v�Ƃ̓����蔻��
	int MapHitCheck(float x, float y, float* move_x, float* move_y, int map[][15]);

	// �w�肵�����W�ɑ��݂���}�b�v�`�b�v�̒l��Ԃ�
	int GetChipParam(float x, float y, int map[][15]);

	// �אږʂւ̈ړ�����
	void MoveCubeFaceRight();
	void MoveCubeFaceLeft();
	void MoveCubeFaceUp();
	void MoveCubeFaceDown();

	// �W�����v�t���O
	bool _jumpFlag;

	// �W�����v�p���[
	float _jumpPower;

};