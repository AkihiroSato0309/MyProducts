#pragma once

// �v���C���[������Ɋւ���񋓌^
enum eUpDirection
{
	eUpDirect = 0,
	eRightDirect,
	eDownDirect,
	eLeftDirect,

	//�ȉ��A������ύX�̍ۂɎg�p(���v���)
	eChange90,
	eChange270,
	eChange180
};

// int�^��Vector3
struct sIntVector3
{
	int x;
	int y;
	int z;
};

// �v���C���[�ɉ�]�̗L���A���̕����𑗐M����\����
struct sStageRotationInfo
{
	bool rotationFlag;	// �X�e�[�W�N���X�̉�]�t���O�Ƃ͗p�r���Ⴄ
	char axis;			// x,y,z�̂ǂ�����Ƃ��邩
	int rotationNum;	// ���_���牽�Ԗڂ̃L���[�u����]�����邩
	int rotationDirect;	// �E��]������]��
};