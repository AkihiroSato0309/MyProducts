/*------------------------------------------------------------------------------------------

 �� ���O : SModelMgr.h

 �� �쐬 : �����a�P

 AhoStyleEngine�Ɏg�p�B����������

------------------------------------------------------------------------------------------*/

#pragma once

#include "Global.h"
#include <Model.h>

using namespace DirectX;

//���f���ۑ��ő吔
static const unsigned char MAX_MODEL_PTR_NUM = 128;

/*******************************************************************************************

 �� -- Class�� ----- ��

�@�ESModelMgr�isingleton�j

 �� -- �g�p�p�r ----- ��

�@�E3D���f���̕ۊǂƓǂݎ��A���������A���������܂�

 �� -- �g�p���@ ----- ��
�@
�@�E���f���������O�ɓo�^���Ă������ƂŁA�ēx�A���f�����g�p����ۂɁA�Q�Ɠn���ł���B

  �E���f�����̃|�C���^���g�p���̏ꍇ�A���������͕s�\�B�G���[�����ɂȂ�܂��B

  �E���f���̃|�C���^���������l�ɁA�g�p���͎g�p�ł��܂���B

  �E128�ȏ�̃��f����ǂݍ��܂��Ă����Ƃ͂ł��܂���B

�@�E���f���̃|�C���^�𑼂ŕێ�����ꍇ�A���̃|�C���^���f���[�g���Ă͂����܂���B

�@�E���f����qnique_ptr�Ő錾�ł͂Ȃ�Model*�Ő錾���Ďg�p���Ă��������B

********************************************************************************************/

class SModelMgr
{
public:

	// �B��̃A�N�Z�X�o�H
	static SModelMgr* GetInstance();

	// ���f�������񃍁[�h����
	void Load( Model*&, wchar_t*, std::unique_ptr<EffectFactory>& );
	// ���f�����㏑�����A�ǂݍ���
	void Change( wchar_t*, wchar_t*, std::unique_ptr<EffectFactory>& );
	// ���f������������
	void Relese( wchar_t* );

private:

	// �R���X�g���N�^
	SModelMgr();

	// �A�N�Z�X�֎~
	SModelMgr( const SModelMgr& rhs );
	SModelMgr& operator=( const SModelMgr& rhs );	

	wchar_t* _fileName[MAX_MODEL_PTR_NUM];		// ���f�����ۑ�
	wchar_t cat[100];							// �A��������ۑ�

	// ���f���f�[�^�ۑ��p
	std::unique_ptr<Model> _model[MAX_MODEL_PTR_NUM];
	// ���f���|�C���^��ێ����Ă���|�C���^�̃|�C���^
	Model**				   _pModel[MAX_MODEL_PTR_NUM];

};