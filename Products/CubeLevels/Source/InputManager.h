#pragma once

#include <Xinput.h>

//-----------------------------------------------------------------------------
// �g�����i���ӁFXinput9_1_0.lib��ǉ����Ă��������j
//
// InputManager*	g_pInputManager
//
// �������i���̓}�l�[�W���[�̐����j
// g_pInputManager = new InputManager();
//
// ���͏�Ԃ̍X�V�i���C�����[�v��Update()�̍ŏ��ŌĂяo���ĉ������j
// g_pInputManager->Update();
//
// �I�������i���̓}�l�[�W���[�̍폜�j
// delete g_pInputManager;
//
//=============================================================================
// ���͂��`�F�b�N������@�i�R���g���[���[�̃{�^���̒�`��Xinput.h�ɒ�`���Ă���܂��j
//
// // �X�y�[�X�L�[�������ꂽ���`�F�b�N����i�����ꂽ�u�Ԃ����j
// if ( g_pInputManager->IsKeyPressed( VK_SPACE ) )
//
// // �X�y�[�X�L�[��������Ă��邩�`�F�b�N����i�����Ă���Ԃ͂����Ɓj
// if ( g_pInputManager->IsKeyDown( VK_SPACE ) )
//
//�@��XBOX360�̃R���g���[���[�̏ꍇ
//
// // A�{�^���������ꂽ���`�F�b�N����i�����ꂽ�u�Ԃ����j
// if ( g_pInputManager->IsButtonPressed( XINPUT_GAMEPAD_A ) )
//
// // A�{�^������������Ă��邩�`�F�b�N����i�����Ă���Ԃ͂����Ɓj
// if ( g_pInputManager->IsButtonDown( XINPUT_GAMEPAD_A ) )
//
//-----------------------------------------------------------------------------

static const short SINPUT_GAMEPAD_OVER = 32750;
static const float DEAD_DEGREE = 0.15f;

// ���̓}�l�[�W���[�N���X
class InputManager
{
	bool isConnected[XUSER_MAX_COUNT];
	BYTE lastKey[256];
	BYTE currentKey[256];
	XINPUT_STATE lastState[XUSER_MAX_COUNT];
	XINPUT_STATE currentState[XUSER_MAX_COUNT];
	DWORD lastMouse;
	DWORD currentMouse;
	
	double lStickRadian[XUSER_MAX_COUNT];
	double rStickRadian[XUSER_MAX_COUNT];

	bool IsKeyDown( BYTE* keyArray, BYTE key )
	{
		return keyArray[key] & 0x80 ? true : false;
	}

	bool IsKeyUp( BYTE* keyArray, BYTE key )
	{
		return keyArray[key] & 0x80 ? false : true;
	}

	bool IsButtonDown( XINPUT_STATE* state, WORD button, int index = 0 )
	{
		return state[index].Gamepad.wButtons & button ? true : false;
	}

	bool IsButtonUp( XINPUT_STATE* state, WORD button, int index = 0 )
	{
		return state[index].Gamepad.wButtons & button ? false : true;
	}

public:
	InputManager() : lastMouse( 0 ), currentMouse( 0 )
	{
		for (int i = 0; i < XUSER_MAX_COUNT; i++)
		{
			isConnected[i] = false;
			ZeroMemory( lastKey, sizeof(BYTE) * 256 );
			ZeroMemory( currentKey, sizeof(BYTE) * 256 );
			ZeroMemory( lastState, sizeof(XINPUT_STATE) );
			ZeroMemory( currentState, sizeof(XINPUT_STATE) );
		}
	}

	~InputManager() {}

	// �X�V
	void Update();

	// �L�[��������Ă��邩�`�F�b�N����֐�
	bool IsKeyDown( BYTE key )
	{
		return currentKey[key] & 0x80 ? true : false;
	}

	// �L�[�������ꂽ���`�F�b�N����֐�
	bool IsKeyPressed( BYTE key )
	{
		return IsKeyDown( currentKey, key ) && IsKeyUp( lastKey, key );
	}

	// �R���g���[���[�̃{�^����������Ă��邩�`�F�b�N����֐�
	bool IsButtonDown( WORD button, int index = 0 )
	{
		return currentState[index].Gamepad.wButtons & button ? true : false;
	}

	// �R���g���[���[�̃{�^���������ꂽ���`�F�b�N����֐�
	bool IsButtonPressed( WORD button, int index = 0 )
	{
		return IsButtonDown( currentState, button, index ) && IsButtonUp( lastState, button, index );
	}

	// �R���g���[���[���q����Ă��邩�`�F�b�N����֐�
	bool IsConnected(int index)
	{
		if( index < 0 || index >= XUSER_MAX_COUNT )
			return false;
		return isConnected[index];
	}

	// L2�{�^����������Ă�����true
	bool IsL2Down()
	{
		if(currentState[0].Gamepad.bLeftTrigger > 0)
		{
			return true;
		}

		return false;
	}
	bool IsL2Pressed()
	{
		if(currentState[0].Gamepad.bLeftTrigger > 0 && lastState[0].Gamepad.bLeftTrigger == 0)
		{
			return true;
		}
		return false;
	}

	// R2�{�^����������Ă�����true
	bool IsR2Down()
	{
		if(currentState[0].Gamepad.bRightTrigger > 0)
		{
			return true;
		}

		return false;
	}
	bool IsR2Pressed()
	{
		if(currentState[0].Gamepad.bRightTrigger > 0 && lastState[0].Gamepad.bRightTrigger == 0)
		{
			return true;
		}
		return false;
	}	

	// ���A�i���O�X�e�B�b�N�̓|�������x���擾(1���ő�)
	float GetLStickDegreesX()
	{
		float degrees = (float)(currentState[0].Gamepad.sThumbLX) / (float)SINPUT_GAMEPAD_OVER;
		if(degrees > 1.0f) degrees = 1.0f;
		if(degrees < -1.0f) degrees = -1.0f;
		if(degrees < 0.1f || degrees > -0.1f) degrees = 0.0f;
		return degrees;
	}
	float GetLStickDegreesY()
	{
		float degrees = (float)(currentState[0].Gamepad.sThumbLY) / (float)SINPUT_GAMEPAD_OVER;
		if(degrees > 1.0f) degrees = 1.0f;
		if(degrees < -1.0f) degrees = -1.0f; 
		return degrees;
	}

	// �E�A�i���O�X�e�B�b�N�̓|�������x���擾(1���ő�)
	float GetRStickDegreesX()
	{
		float degrees = (float)(currentState[0].Gamepad.sThumbRX) / (float)SINPUT_GAMEPAD_OVER;
		if(degrees > 1.0f) degrees = 1.0f;
		if(degrees < -1.0f) degrees = -1.0f; 
		// �����͎��̏���
		if(degrees < DEAD_DEGREE && degrees > -DEAD_DEGREE) degrees = 0.0f;
		return degrees;
	}
	float GetRStickDegreesY()
	{
		float degrees = (float)(currentState[0].Gamepad.sThumbRY) / (float)SINPUT_GAMEPAD_OVER;
		if(degrees > 1.0f) degrees = 1.0f;
		if(degrees < -1.0f) degrees = -1.0f; 
		// �����͎��̏���
		if(degrees < DEAD_DEGREE && degrees > -DEAD_DEGREE) degrees = 0.0f;
		return degrees;
	}

	// ���A�i���O�X�e�B�b�N�̊p�x(X_Z)���擾
	double GetLStickRadian()
	{
		return lStickRadian[0];
	}

	// �E�A�i���O�X�e�B�b�N�̊p�x(X_Z)���擾
	double GetRStickRadian()
	{
		return rStickRadian[0];
	}

	// ���A�i���O�X�e�B�b�N�̓��͊m�F
	bool IsCheckLStick(short DeadZone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		if( (currentState[0].Gamepad.sThumbLX < DeadZone && 
				 currentState[0].Gamepad.sThumbLX > -DeadZone) && 
				(currentState[0].Gamepad.sThumbLY < DeadZone && 
				 currentState[0].Gamepad.sThumbLY > -DeadZone) )
		{	
			return false;
		}

		return true;
	}

	// �E�A�i���O�X�e�B�b�N�̓��͊m�F
	bool IsCheckRStick(short DeadZone = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		if( (currentState[0].Gamepad.sThumbRX < DeadZone && 
				 currentState[0].Gamepad.sThumbRX > -DeadZone) && 
				(currentState[0].Gamepad.sThumbRY < DeadZone && 
				 currentState[0].Gamepad.sThumbRY > -DeadZone) )
		{	
			return false;
		}

		return true;
	}

	float CheckRStick()
	{
		
	}

	// �}�E�X�̈ʒu���擾����֐�
	void GetMousePosition(POINT *pos);
};
