#include "SModelMgr.h"
#include <io.h>

// �R���X�g���N�^
SModelMgr::SModelMgr()
{
	// _filename��nullptr�ŏ�����
	for( int i = 0; i < MAX_MODEL_PTR_NUM; i++ )
	{
		_fileName[i] = nullptr;
	}
}



// �B��̃A�N�Z�X�o�H
SModelMgr* SModelMgr::GetInstance()
{
	static SModelMgr _instance;
	return &_instance;
}

// ���f�������񃍁[�h����
void SModelMgr::Load( Model*& _registerModel, wchar_t* _registerName, std::unique_ptr<EffectFactory>& _fxX  )
{

	// ���f���������݂��Ȃ��ꍇ�x�����A���f���̃A�h���X��nullptr��Ԃ�
	if( _registerName == nullptr || _waccess_s(_registerName, 0) != 0 )
	{
		swprintf_s( cat, 100, L"�t�@�C�������݂��܂��� : %s", _registerName );
		MessageBox( NULL, cat, TEXT("�x��"), MB_OK );
		return;
	}

	// ���t�@�C���������݂��邩
	for( int i = 0; i < MAX_MODEL_PTR_NUM; i++ )
	{
		// _fileName[i]��nullptr�Ȃ珈�����΂�
		if( _fileName[i] == nullptr ) continue;

		// ���t�@�C���������݂���ꍇ�A���̒l���Q�Ƃ�����
		if( wcscmp( _fileName[i], _registerName ) == 0 )
		{
			_registerModel = _model[i].get();
			_pModel[i] = &_registerModel;
			return;
		}

	}

	// ���t�@�C�������݂��Ȃ��̂œo�^����
	for( int i = 0; i < MAX_MODEL_PTR_NUM; i++ )
	{
		// ���t�@�C���������݂���ꍇ�A���̒l���Q�Ƃ�����
		if( _fileName[i] == nullptr )
		{
			_fileName[i] = _registerName;
			_model[i] = Model::CreateFromCMO( g_pd3dDevice, _registerName, *_fxX );
			_registerModel = _model[i].get();	// �쐬�������f������������
			_pModel[i] = &_registerModel;		// fileName[i]��nullptr����
			return;
		}
	}
}

// ���f�����㏑�����A�ǂݍ���
void SModelMgr::Change(wchar_t* _beforeName, wchar_t* _afterName, std::unique_ptr<EffectFactory>& _fxX )
{
	// ���f���������݂��Ȃ��ꍇ
	if( _beforeName == nullptr || _waccess_s(_beforeName, 0) != 0 )
	{
		swprintf_s( cat, 100, L"�t�@�C�������݂��܂��� : %s", _beforeName );
		MessageBox( NULL, cat, TEXT("�x��"), MB_OK );
		return;
	}

	// ���f���������݂��Ȃ��ꍇ
	if( _afterName == nullptr || _waccess_s(_afterName, 0) != 0 )
	{
		swprintf_s( cat, 100, L"�t�@�C�������݂��܂��� : %s", _afterName );
		MessageBox( NULL, cat, TEXT("�x��"), MB_OK );
		return;
	}

	//���t�@�C���������݂��邩
	for( int i = 0; i < MAX_MODEL_PTR_NUM; i++ )
	{
		// _fileName[i]��nullptr�ȊO�̏ꍇ
		if( _fileName[i] != nullptr )
		{
			if( wcscmp( _fileName[i], _beforeName ) == 0 && wcscmp( _fileName[i], _afterName ) != 0  )
			{
				_fileName[i] = _afterName;
				*_model[i] = *Model::CreateFromCMO( g_pd3dDevice, _afterName, *_fxX );
				return;
			}
		}
	}
}

// ���f������������
void SModelMgr::Relese( wchar_t* _registerName )
{
	// �f�B���N�g���ɑ��݂��Ȃ��ꍇ�͌x�����Ĕ�����
	if( _waccess_s( _registerName, 0 ) != 0 )
	{
		swprintf_s( cat, 100, L"�t�@�C�������݂��܂��� : %s", _registerName );
		MessageBox( NULL, cat, TEXT("�x��"), MB_OK );
		return;
	}

	// ���t�@�C���������݂��邩
	for( int i = 0; i < MAX_MODEL_PTR_NUM; i++ )
	{
		// _fileName[i]��nullptr�ȊO�̏ꍇ
		if( _fileName[i] != nullptr )
		{
			//�����t�@�C���ɑΉ�����_model[i]����������
			if( wcscmp( _fileName[i], _registerName ) == 0 )
			{
				_fileName[i] = nullptr;		// fileName[i]��nullptr����
				_model[i].reset();			//_model[i]��reset����
				// ���f���̃|�C���^�������Ă���S�Ẵ|�C���^��nullptr�ŏ�����
				*_pModel[i] = nullptr;
				return;
			}
		}
	}
}