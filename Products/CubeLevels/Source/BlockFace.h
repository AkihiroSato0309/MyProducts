//***************************************************************************************************
//
//	BlockFace.h				�������m  12��16��
//
//	�u���b�N��̖ʂɂ��ẴN���X
//
//***************************************************************************************************
#pragma once

#include "Global.h"

// �u���b�N�̑傫��
static const int BLOCK_SIZE = 5;
static const float BLOCK_SIZE_HALF = BLOCK_SIZE * 0.5f;

// ���𒆐S�Ƃ������v���
static const int TURN_RIGHT = -1;
static const int TURN_LEFT = 1;

static const int GOAL = 9;

// �����w�肵����]�̗񋓌^
enum eRotation
{
	eRotation_x,
	eRotation_y,
	eRotation_z,
};

// �X�e�[�W�S�̂ŁA�ǂ̖ʂɑ����Ă��邩
enum eFaceDirection
{
	eTop,
	eBottom,
	eRight,
	eLeft,
	eFront,
	eBack,
};


// �u���b�N�̖ʃN���X
class BlockFace
{
public:
	BlockFace(int hit, int faceDirection);
	~BlockFace();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw(DirectX::SimpleMath::Matrix* matrix);

	// �����蔻��̗L�����擾
	int IsHitState(){return _hit;}

	// ��]�ɂ���Ĕ̌�����ς���
	void Rotation(eRotation rotation, int turn_direction);


private:
	// ���f���̃A�h���X
	DirectX::Model* _model;

	// �e�N�X�`���A�h���X
	ID3D11ShaderResourceView* _pTexture;

	// �����蔻��̗L�薳��
	int _hit;

	// �ǂ̖ʂɑ����Ă��邩
	int _faceDirection;
	
	// 5x5�̂����A���Ԗڂ̈ʒu��[�c][��]
	int _position[2];
};