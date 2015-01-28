//--------------------------------------------------------------------------------------
// File: InputManager.cpp
// Author:Imase Hideyasu
//
//--------------------------------------------------------------------------------------
#include <Windows.h>
#include "Global.h"
#include "InputManager.h"

//--------------------------------------------------------------------------------------
// 更新処理
//--------------------------------------------------------------------------------------
void InputManager::Update()
{
	DWORD dwResult;

	// キー取得
	memcpy( lastKey, currentKey, sizeof(BYTE) * 256 );
	GetKeyboardState( currentKey );

	// XBOX360用コントローラー入力情報取得
	for( DWORD i = 0; i < XUSER_MAX_COUNT; i++ )
	{
		lastState[i] = currentState[i];
		dwResult = XInputGetState( i, &currentState[i] );
        if( dwResult == ERROR_SUCCESS )
		{
			isConnected[i] = true;
			// アナログキーは十字キーに対応してしまう
			if( (currentState[i].Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && 
				 currentState[i].Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) && 
				(currentState[i].Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && 
				 currentState[i].Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) )
			{	
			   currentState[i].Gamepad.sThumbLX = 0;
			   currentState[i].Gamepad.sThumbLY = 0;
			}

			// 一定量以上は補正する
			if( currentState[i].Gamepad.sThumbLX > SINPUT_GAMEPAD_OVER )
				currentState[i].Gamepad.sThumbLX = SINPUT_GAMEPAD_OVER;
			if( currentState[i].Gamepad.sThumbLX < -SINPUT_GAMEPAD_OVER )
				currentState[i].Gamepad.sThumbLX = -SINPUT_GAMEPAD_OVER;
			if( currentState[i].Gamepad.sThumbLY > SINPUT_GAMEPAD_OVER )
				currentState[i].Gamepad.sThumbLY = SINPUT_GAMEPAD_OVER;
			if( currentState[i].Gamepad.sThumbLY < -SINPUT_GAMEPAD_OVER )
				currentState[i].Gamepad.sThumbLY = -SINPUT_GAMEPAD_OVER;

			// L2の入力修正
			if( currentState[i].Gamepad.bLeftTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD )
			{	
			   currentState[i].Gamepad.bLeftTrigger = 0;
			}

			// R2の入力修正
			if( currentState[i].Gamepad.bRightTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD )
			{	
			   currentState[i].Gamepad.bRightTrigger = 0;
			}

			// 上方向を0度、時計回りで回転させる。
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
// マウスカーソルの位置を取得
//--------------------------------------------------------------------------------------
void InputManager::GetMousePosition(POINT *pos)
{
	if( pos == nullptr ) return;

	// スクリーン上での位置を取得
	GetCursorPos( pos );

	// クライアント上の座標に変換	
	ScreenToClient( g_hWnd, pos );
}
