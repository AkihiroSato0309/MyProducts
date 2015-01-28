/*------------------------------------------------------------------------------------------

 �� ���O : STextureMgr.h

 �� �쐬 : �����a�P

 AhoStyleEngine�Ɏg�p�B����������

------------------------------------------------------------------------------------------*/

#pragma once

#include "Global.h"
#include "Common.h"

//�e�N�X�`���ۑ��ő吔
static const unsigned char MAX_TEXTURE_PTR_NUM = 128;

/*******************************************************************************************

 �� -- Class�� ----- ��

�@�ESTextureMgr�isingleton�j

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

class STextureMgr
{
public:

	// �B��̃A�N�Z�X�o�H
	static STextureMgr* GetInstance();

	// ���f�������񃍁[�h����
	void Load( ID3D11ShaderResourceView*&, wchar_t* );
	// ���f�����㏑�����A�ǂݍ���
	//void Change( wchar_t*, wchar_t*, std::unique_ptr<DirectX::EffectFactory>& );
	// ���f������������
	void Relese( wchar_t* );

private:

	// �R���X�g���N�^
	STextureMgr();

	// �f�X�g���N�^
	~STextureMgr();

	// �A�N�Z�X�֎~
	STextureMgr( const STextureMgr& rhs );
	STextureMgr& operator=( const STextureMgr& rhs );	

	wchar_t* _fileName[MAX_TEXTURE_PTR_NUM];		// ���f�����ۑ�
	wchar_t cat[100];							// �A��������ۑ�

	// �e�N�X�`���f�[�^�ۑ��p
	ID3D11ShaderResourceView*	_texture[MAX_TEXTURE_PTR_NUM];
	// �e�N�X�`���|�C���^��ێ����Ă���|�C���^�̃|�C���^
	ID3D11ShaderResourceView**	 _pTexture[MAX_TEXTURE_PTR_NUM];

};