//***************************************************************************************************
//
// Stage.h					�������m�@12��16��
//
//***************************************************************************************************
#pragma once

#include "Cube.h"
#include "Stage2D.h"
#include "IStageMatrixChanger.h"
#include "SStageRotationInfo.h"

struct FaceRemderMgr
{
	int sFace;
	float sDistance;
	DirectX::SimpleMath::Vector3 sFaceCenterPosition;
};




static const int MOVE_FLAME = 10;

enum ePosition{X = 0, Y = 1, Z = 2};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �X�e�[�W�N���X
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Stage : public IStageMatrixChanger
{
public:
	Stage(int* map);
	~Stage();
	
	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Render(const int& playerExist);

	// �L���[�u��]����
	bool RotationCube(const int& playerExistFace, const int& playerUpDirect );

	// �O���p�L���[�u��]����(�N���X�O�����]���߂��s��)
	void Rotation(const int& playerExistFace, const int& playerUpDirect);

	// �w�肵���ʂɑ�����L���[�u�̖ʔz��̐擪�A�h���X��Ԃ�
	CubeFace** GetAppointCubeFace(int appointDirection);

	// 2D�X�e�[�W�̃|�C���^��Ԃ�
	Stage2D* GetStage2D(){return _pStage2D;}

	// �}�g���N�X�𑀍삷��
	void ChangeMatrix(eRotationMatrix rotationDirect);

	// ��]�����ǂ������擾����
	bool GetMovingFlag();

	// ��]���t���O���擾���A�t���O��|��
	bool GetRotationInfoFlag();

	// ��]����n��
	sStageRotationInfo& GetStageRotationInfo()
	{
		return _rotationInfo;
	}

private:
	// ��]���t���O
	bool _moveFlag;

	// �J�E���g�ϐ�
	int _time;

	// ���ړ����Ԃƍ��̎��Ԃ̊���
	float _moveDegree;

	// �X�e�[�W�̖ʂ̒��S�_
	DirectX::SimpleMath::Vector3 _faceCenterPosition[6];

	// �S�̂̉�]�}�g���N�X
	DirectX::SimpleMath::Matrix _rotationMatrix;
	
	// �e���_�ƃJ�����̋���
	float _distance[6];

	// �ێ��L���[�u(���_����)
	Cube* _cube[3][3][3];

	// �ێ�����ʏ��(X, Y�͍��ォ��)
	CubeFace* _pCubeFace[6][3][3];

	// �w�肵���ʂ̃A�h���X������
	CubeFace* _appointCubeFace[9];

	// ���ꂼ��̖ʕʂ̃A�h���X
	CubeFace* _pRenderFace[6][9];

	// �ʂ̕`�悷�鏇�Ԃ��Ǘ�
	FaceRemderMgr _renderMgr[6];

	// 2D�X�e�[�W�f�[�^
	Stage2D* _pStage2D;

	// ��]�p�x���
	float _Radian;

	// �v���C���[�ɑ��M�����]���
	sStageRotationInfo _rotationInfo;


	// �L���[�u��]����(�w�莲�𒆐S�Ƃ������_����̌��ƁA��]����)
	void RotationCubeX(int num, const int& turn_direction);
	void RotationCubeY(int num, const int& turn_direction);
	void RotationCubeZ(int num, const int& turn_direction);

	// 8�̒��_�̍��W
	const DirectX::SimpleMath::Vector3 _vertex[8];

	// �w��ʂ̃L���[�u�z���Ԃ��ۂɎg�p���鑍���菈��
	void AllAppoint(CubeFace** face, int appointDirection, int cubePosition, int searchPosition);

	// �v���C���[�̑��ݖʁA��������l�������c��]���s��
	bool VerticalRotation(const int& playerExistFace, const int& playerUpDirect, const int& rotationNum, const int& rotationDirect);

	// ����]���s��
	bool HorizontallyRotation(const int& playerExistFace, const int& playerUpDirect, const int& rotationNum, const int& rotationDirect);
	
	// �����ɓ��͂��ꂽ��]�����̋t��߂�l�Ƃ���
	int ReverseRotationDirect(const int& rotationDirect);

	// �����ɓ��͂��ꂽ�L���[�u�i���o�[�𔽓]������
	// �� : 0��2, 2��0
	int ReverseRotationNum(const int& rotationNum);
};