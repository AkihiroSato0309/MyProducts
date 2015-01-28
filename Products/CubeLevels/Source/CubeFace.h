//***************************************************************************************************
//
// CubeFace.h				�������m  12��16��
//
//***************************************************************************************************
#pragma once

#include "BlockFace.h"
class Cube;

// �e���_�̃r�b�g�t���O(�g�b�v�̍�������)
static const int VERTEX_FLAG[8] = 
{
	1 << 0,
	1 << 1,
	1 << 2,
	1 << 3,
	1 << 4,
	1 << 5,
	1 << 6,
	1 << 7,
};

class CubeFace
{
public:
	// �R���X�g���N�^
	CubeFace(int faceDirection, int blockVal[][5]);

	// �f�X�g���N�^
	~CubeFace();

	// �ʂ�`��
	void Render(DirectX::SimpleMath::Matrix& worldMatrix, const DirectX::SimpleMath::Matrix& frontMatrix);

	// �}�g���N�X��ݒ肷��
	void SetCube(Cube* cube);

	// �ʂ̕`��t���O���擾����
	int GetRenderFlag(){return _renderFlag;}

	// �����ʂ��擾����
	int GetFaceDirection(){return _faceDirection;}

	// �u���b�N�����擾����
	int* GetBlockInfo(){return _blockInfo;}

	// �ʂɂ��p�x�}�g���N�X
	static DirectX::SimpleMath::Matrix _sTopMatrix;
	static DirectX::SimpleMath::Matrix _sBottomMatrix;
	static DirectX::SimpleMath::Matrix _sRightMatrix;
	static DirectX::SimpleMath::Matrix _sLeftMatrix;
	static DirectX::SimpleMath::Matrix _sFrontMatrix;
	static DirectX::SimpleMath::Matrix _sBackMatrix;

	// �ʂ������o���}�g���N�X
	static DirectX::SimpleMath::Matrix _sPushMatrix;

	// �ʂ�1�u���b�N�ړ�������}�g���N�X
	static DirectX::SimpleMath::Matrix _sPlusXMatrix;
	static DirectX::SimpleMath::Matrix _sPlusZMatrix;
	static DirectX::SimpleMath::Matrix _sMinusXMatrix;
	static DirectX::SimpleMath::Matrix _sMinusZMatrix;

	// �ʂ̌�������ύX������
	void ChangeFaceDirectionX(int rotaDirection);
	void ChangeFaceDirectionY(int rotaDirection);
	void ChangeFaceDirectionZ(int rotaDirection);

	// �ʂ̕`��t���O���X�V����
	void SetCubeFaceRenderFlag();

	// �ʂ��ƂɃ}�g���N�X��ݒ肷��
	void SetCubeFaceMatrix();

	// �e�̃}�g���N�X���擾����
	void SetParentMatrix(const DirectX::SimpleMath::Matrix* parentMatrix);

	// �e�̏����ʒu�|�C���^���擾����
	void SetParentPosition(const int* parentPosition);

	// �e�̉�]�t���O���擾����
	void SetParentRotaionFlag(const bool* parentMoveFlag);

	// �e�̉�]���Ɏg�p����t���O���擾����
	void SetParentMovingMatrix(const DirectX::SimpleMath::Matrix* parentMatrix);

private:
	// �ʂ̃u���b�N�̐�
	BlockFace* _blockFace[5][5];

	// �x�[�X�̐F���擾
	ID3D11ShaderResourceView*	_pTexture;

	// �u���b�N�̏��
	int _blockInfo[25];

	// �ʂ̕���
	int _faceDirection;

	// �`��̗L���t���O(�r�b�g�Ǘ�)
	int _renderFlag;

	// �A�����̃L���[�u�̃A�h���X
	Cube* _pCube;

	// �ʂ���]�����ĕ\��������}�g���N�X
	DirectX::SimpleMath::Matrix* _pDirectionMatrix;

	// �e�L���[�u�̉�]�}�g���N�X�|�C���^
	const DirectX::SimpleMath::Matrix* _pParentMatrix;

	// �e�L���[�u�̉�]���Ɏg�p����}�g���N�X�|�C���^
	const DirectX::SimpleMath::Matrix* _pParentMovingMatrix;

	// �e�L���[�u�̈ʒu���z��̐擪�A�h���X
	const int* _pParentPosition;

	// �e�L���[�u�̉�]�t���O
	const bool* _pParentMoveFlag;	
};