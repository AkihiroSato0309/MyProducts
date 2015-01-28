#pragma once

#include <Xinput.h>

//-----------------------------------------------------------------------------
// 使い方（注意：Xinput9_1_0.libを追加してください）
//
// InputManager*	g_pInputManager
//
// 初期化（入力マネージャーの生成）
// g_pInputManager = new InputManager();
//
// 入力状態の更新（メインループのUpdate()の最初で呼び出して下さい）
// g_pInputManager->Update();
//
// 終了処理（入力マネージャーの削除）
// delete g_pInputManager;
//
//=============================================================================
// 入力をチェックする方法（コントローラーのボタンの定義はXinput.hに定義してあります）
//
// // スペースキーが押されたかチェックする（押された瞬間だけ）
// if ( g_pInputManager->IsKeyPressed( VK_SPACE ) )
//
// // スペースキーが押されているかチェックする（押している間はずっと）
// if ( g_pInputManager->IsKeyDown( VK_SPACE ) )
//
//　☆XBOX360のコントローラーの場合
//
// // Aボタンが押されたかチェックする（押された瞬間だけ）
// if ( g_pInputManager->IsButtonPressed( XINPUT_GAMEPAD_A ) )
//
// // Aボタンがが押されているかチェックする（押している間はずっと）
// if ( g_pInputManager->IsButtonDown( XINPUT_GAMEPAD_A ) )
//
//-----------------------------------------------------------------------------

static const short SINPUT_GAMEPAD_OVER = 32750;
static const float DEAD_DEGREE = 0.15f;

// 入力マネージャークラス
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

	// 更新
	void Update();

	// キーが押されているかチェックする関数
	bool IsKeyDown( BYTE key )
	{
		return currentKey[key] & 0x80 ? true : false;
	}

	// キーが押されたかチェックする関数
	bool IsKeyPressed( BYTE key )
	{
		return IsKeyDown( currentKey, key ) && IsKeyUp( lastKey, key );
	}

	// コントローラーのボタンが押されているかチェックする関数
	bool IsButtonDown( WORD button, int index = 0 )
	{
		return currentState[index].Gamepad.wButtons & button ? true : false;
	}

	// コントローラーのボタンが押されたかチェックする関数
	bool IsButtonPressed( WORD button, int index = 0 )
	{
		return IsButtonDown( currentState, button, index ) && IsButtonUp( lastState, button, index );
	}

	// コントローラーが繋がれているかチェックする関数
	bool IsConnected(int index)
	{
		if( index < 0 || index >= XUSER_MAX_COUNT )
			return false;
		return isConnected[index];
	}

	// L2ボタンが押されていたらtrue
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

	// R2ボタンが押されていたらtrue
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

	// 左アナログスティックの倒した程度を取得(1が最大)
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

	// 右アナログスティックの倒した程度を取得(1が最大)
	float GetRStickDegreesX()
	{
		float degrees = (float)(currentState[0].Gamepad.sThumbRX) / (float)SINPUT_GAMEPAD_OVER;
		if(degrees > 1.0f) degrees = 1.0f;
		if(degrees < -1.0f) degrees = -1.0f; 
		// 未入力時の処理
		if(degrees < DEAD_DEGREE && degrees > -DEAD_DEGREE) degrees = 0.0f;
		return degrees;
	}
	float GetRStickDegreesY()
	{
		float degrees = (float)(currentState[0].Gamepad.sThumbRY) / (float)SINPUT_GAMEPAD_OVER;
		if(degrees > 1.0f) degrees = 1.0f;
		if(degrees < -1.0f) degrees = -1.0f; 
		// 未入力時の処理
		if(degrees < DEAD_DEGREE && degrees > -DEAD_DEGREE) degrees = 0.0f;
		return degrees;
	}

	// 左アナログスティックの角度(X_Z)を取得
	double GetLStickRadian()
	{
		return lStickRadian[0];
	}

	// 右アナログスティックの角度(X_Z)を取得
	double GetRStickRadian()
	{
		return rStickRadian[0];
	}

	// 左アナログスティックの入力確認
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

	// 右アナログスティックの入力確認
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

	// マウスの位置を取得する関数
	void GetMousePosition(POINT *pos);
};
