//******************************************************************
//
// �ʂ̃X�e�[�W�f�[�^
//
//******************************************************************
#pragma once

#include "BlockFace.h"

class Stage2D
{
public:
	// �R���X�g���N�^
	Stage2D();
	// �f�X�g���N�^
	~Stage2D();

	// �X�e�[�W���\������
	void CreateStage(int* map);

	// �X�e�[�W��؂�ւ���
	void ChangeStage(int existFace);

	// �}�b�v��Ԃ�
	int* GetMapData(int face);

	// ��]���Ƀ}�b�v��؂�ւ���
	void RotationCubeX(int num, const int& turnDirection);
	void RotationCubeY(int num, const int& turnDirection);
	void RotationCubeZ(int num, const int& turnDirection);

private:
	// �X�e�[�W���[��][�c][��]
	int _stageInfo[6][15][15];
};