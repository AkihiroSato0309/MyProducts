#include "STextureMgr.h"
#include <io.h>

using namespace DirectX;

// �R���X�g���N�^
STextureMgr::STextureMgr()
{
	// _filename��nullptr�ŏ�����
	for( int i = 0; i < MAX_TEXTURE_PTR_NUM; i++ )
	{
		_fileName[i] = nullptr;
		_texture[i] = nullptr;
	}
	
}

// �f�X�g���N�^
STextureMgr::~STextureMgr()
{
	// _filename��nullptr�ŏ�����
	for( int i = 0; i < MAX_TEXTURE_PTR_NUM; i++ )
	{
		if( _texture[i] != nullptr)
			_texture[i]->Release();
	}
}

// �B��̃A�N�Z�X�o�H
STextureMgr* STextureMgr::GetInstance()
{
	static STextureMgr _instance;
	return &_instance;
}

// ���f�������񃍁[�h����
void STextureMgr::Load( ID3D11ShaderResourceView*& _registerTexture, wchar_t* _registerName  )
{

	// ���f���������݂��Ȃ��ꍇ�x�����A���f���̃A�h���X��nullptr��Ԃ�
	if( _registerName == nullptr || _waccess_s(_registerName, 0) != 0 )
	{
		swprintf_s( cat, 100, L"�t�@�C�������݂��܂��� : %s", _registerName );
		MessageBox( NULL, cat, TEXT("�x��"), MB_OK );
		return;
	}

	// ���t�@�C���������݂��邩
	for( int i = 0; i < MAX_TEXTURE_PTR_NUM; i++ )
	{
		// _fileName[i]��nullptr�Ȃ珈�����΂�
		if( _fileName[i] == nullptr ) continue;

		// ���t�@�C���������݂���ꍇ�A���̒l���Q�Ƃ�����
		if( wcscmp( _fileName[i], _registerName ) == 0 )
		{
			_registerTexture = _texture[i];
			_pTexture[i] = &_registerTexture;
			return;
		}

	}

	// ���t�@�C�������݂��Ȃ��̂œo�^����
	for( int i = 0; i < MAX_TEXTURE_PTR_NUM; i++ )
	{
		// ���t�@�C���������݂���ꍇ�A���̒l���Q�Ƃ�����
		if( _fileName[i] == nullptr )
		{
			_fileName[i] = _registerName;
			// �e�N�X�`���ǂݍ���
			CreateWICTextureFromFile(g_pd3dDevice, g_pImmediateContext, _registerName, nullptr, &_texture[i]);
			_registerTexture = _texture[i];	// �쐬�������f������������
			_pTexture[i] = &_registerTexture;		// fileName[i]��nullptr����
			return;
		}
	}
}

//// ���f�����㏑�����A�ǂݍ���
//void STextureMgr::Change(wchar_t* _beforeName, wchar_t* _afterName, std::unique_ptr<EffectFactory>& _fxX )
//{
//	// ���f���������݂��Ȃ��ꍇ
//	if( _beforeName == nullptr || _waccess_s(_beforeName, 0) != 0 )
//	{
//		swprintf_s( cat, 100, L"�t�@�C�������݂��܂��� : %s", _beforeName );
//		MessageBox( NULL, cat, TEXT("�x��"), MB_OK );
//		return;
//	}
//
//	// ���f���������݂��Ȃ��ꍇ
//	if( _afterName == nullptr || _waccess_s(_afterName, 0) != 0 )
//	{
//		swprintf_s( cat, 100, L"�t�@�C�������݂��܂��� : %s", _afterName );
//		MessageBox( NULL, cat, TEXT("�x��"), MB_OK );
//		return;
//	}
//
//	//���t�@�C���������݂��邩
//	for( int i = 0; i < MAX_TEXTURE_PTR_NUM; i++ )
//	{
//		// _fileName[i]��nullptr�ȊO�̏ꍇ
//		if( _fileName[i] != nullptr )
//		{
//			if( wcscmp( _fileName[i], _beforeName ) == 0 && wcscmp( _fileName[i], _afterName ) != 0  )
//			{
//				_fileName[i] = _afterName;
//				*_texture[i] = *Model::CreateFromCMO( g_pd3dDevice, _afterName, *_fxX );
//				return;
//			}
//		}
//	}
//}

// ���f������������
void STextureMgr::Relese( wchar_t* _registerName )
{
	// �f�B���N�g���ɑ��݂��Ȃ��ꍇ�͌x�����Ĕ�����
	if( _waccess_s( _registerName, 0 ) != 0 )
	{
		swprintf_s( cat, 100, L"�t�@�C�������݂��܂��� : %s", _registerName );
		MessageBox( NULL, cat, TEXT("�x��"), MB_OK );
		return;
	}

	// ���t�@�C���������݂��邩
	for( int i = 0; i < MAX_TEXTURE_PTR_NUM; i++ )
	{
		// _fileName[i]��nullptr�ȊO�̏ꍇ
		if( _fileName[i] != nullptr )
		{
			//�����t�@�C���ɑΉ�����_texture[i]����������
			if( wcscmp( _fileName[i], _registerName ) == 0 )
			{
				_fileName[i] = nullptr;		// fileName[i]��nullptr����
				delete _texture[i];			//_texture[i]��reset����
				// �e�N�X�`���̃|�C���^�������Ă���S�Ẵ|�C���^��nullptr�ŏ�����
				*_pTexture[i] = nullptr;
				return;
			}
		}
	}
}