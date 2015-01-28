#pragma once

#include "Common.h"

class Player;

static const float DISTANCE_FROM_STAGE = 200.0f;
static const float DISTANCE_FROM_STAGE_Y = 20.0f;
static const float ROTATION_SPEED_CAMERA = 0.05f;
static const float ROTATION_SPEED_MAX = 0.05f;

class Camera
{
public:
	Camera(){};
	Camera(	DirectX::SimpleMath::Vector3 eye,
			DirectX::SimpleMath::Vector3 target,
			DirectX::SimpleMath::Vector3 up);
	~Camera(){}

	DirectX::SimpleMath::Matrix GetView(){return _view;}
	DirectX::SimpleMath::Matrix GetProjection(){return _projection;}

	void Update(const int playerExistFace, const int& playerUpDirection);

	// カメラアングルのリセットを行う
	void ResetCameraAngle();

	//--------------------------------------------------------------------------
	// セッター
	//--------------------------------------------------------------------------

	void SetEyePosition(DirectX::SimpleMath::Vector3 eye)
	{
		_eye = eye;
	}
	void SetTargetPosition(DirectX::SimpleMath::Vector3 target)
	{
		_target = target;
	}
	void SetUpPosition(DirectX::SimpleMath::Vector3 up)
	{
		_up = up;
	}

	//--------------------------------------------------------------------------
	// ゲッター
	//--------------------------------------------------------------------------
	DirectX::SimpleMath::Vector3 GetEyePosition()
	{
		return _eye;
	}

	//

private:

	float _radian_X;
	float _radian_Y;

	bool _cameraBackFlag;	//カメラが裏面にいったフラグ

	bool _radianX_ReverseFlag;	// ラジアン反転をしているかどうか
	bool _radianY_ReverseFlag;

	// アングルラジアンを反転してなかったら反転させる
	void RadianYReverseTrue()
	{
		if(_radianY_ReverseFlag == false)
		{
			_radian_Y *= -1.0f;
			_radianY_ReverseFlag = true;
		}
	}
	void RadianXReverseTrue()
	{
		if(_radianX_ReverseFlag == false)
		{
			_radian_X *= -1.0f;
			_radianX_ReverseFlag = true;
		}
	}

	DirectX::SimpleMath::Vector3 _eye;
	DirectX::SimpleMath::Vector3 _target;
	DirectX::SimpleMath::Vector3 _up;

	DirectX::SimpleMath::Matrix _view;
	DirectX::SimpleMath::Matrix _projection;

};