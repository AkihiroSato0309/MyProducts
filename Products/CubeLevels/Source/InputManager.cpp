//--------------------------------------------------------------------------------------
// File: InputManager.cpp
// Author:Imase Hideyasu
//
//--------------------------------------------------------------------------------------
#include <Windows.h>
#include "Global.h"
#include "InputManager.h"

//--------------------------------------------------------------------------------------
// �X�V����
//--------------------------------------------------------------------------------------
void InputManager::Update()
{
	DWORD dwResult;

	// �L�[�擾
	memcpy( lastKey, currentKey, sizeof(BYTE) * 256 );
	GetKeyboardState( currentKey );

	// XBOX360�p�R���g���[���[���͏��擾
	for( DWORD i = 0; i < XUSER_MAX_COUNT; i++ )
	{
		lastState[i] = currentState[i];
		dwResult = XInputGetState( i, &currentState[i] );
        if( dwResult == ERROR_SUCCESS )
		{
			isConnected[i] = true;
			// �A�i���O�L�[�͏\���L�[�ɑΉ����Ă��܂�
			if( (currentState[i].Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && 
				 currentState[i].Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) && 
				(currentState[i].Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && 
				 currentState[i].Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) )
			{	
			   currentState[i].Gamepad.sThumbLX = 0;
			   currentState[i].Gamepad.sThumbLY = 0;
			}

			// ���ʈȏ�͕␳����
			if( currentState[i].Gamepad.sThumbLX > SINPUT_GAMEPAD_OVER )
				currentState[i].Gamepad.sThumbLX = SINPUT_GAMEPAD_OVER;
			if( currentState[i].Gamepad.sThumbLX < -SINPUT_GAMEPAD_OVER )
				currentState[i].Gamepad.sThumbLX = -SINPUT_GAMEPAD_OVER;
			if( currentState[i].Gamepad.sThumbLY > SINPUT_GAMEPAD_OVER )
				currentState[i].Gamepad.sThumbLY = SINPUT_GAMEPAD_OVER;
			if( currentState[i].Gamepad.sThumbLY < -SINPUT_GAMEPAD_OVER )
				currentState[i].Gamepad.sThumbLY = -SINPUT_GAMEPAD_OVER;

			// L2�̓��͏C��
			if( currentState[i].Gamepad.bLeftTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD )
			{	
			   currentState[i].Gamepad.bLeftTrigger = 0;
			}

			// R2�̓��͏C��
			if( currentState[i].Gamepad.bRightTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD )
			{	
			   currentState[i].Gamepad.bRightTrigger = 0;
			}

			// �������0�x�A���v���ŉ�]������B
			lStickRadian[i] = atan2(currentState[i].Gamepad.sThumbLY, currentState[i].Gamepad.sThumbLX);
			rStickRadian[i] = atan2(currentState[i].Gamepad.sThumbRY, currentState[i].Gamepad.sThumbRX);
		}
		else
		{
			isConnected[i] = false;
		}
	}
}


//--------------------------------------------------------------------------------------
// �}�E�X�J�[�\���̈ʒu���擾
//--------------------------------------------------------------------------------------
void InputManager::GetMousePosition(POINT *pos)
{
	if( pos == nullptr ) return;

	// �X�N���[����ł̈ʒu���擾
	GetCursorPos( pos );

	// �N���C�A���g��̍��W�ɕϊ�	
	ScreenToClient( g_hWnd, pos );
}
