#include "Camera.h"
#include "Global.h"
#include "BlockFace.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Camera::Camera(	DirectX::SimpleMath::Vector3 eye,
				DirectX::SimpleMath::Vector3 target,
				DirectX::SimpleMath::Vector3 up)
{
	_eye = eye;
	_target = target;
	_up = up;

	_radian_X = 0.0f;
	_radian_Y = 0.0f;

	_radianX_ReverseFlag = false;
	_radianY_ReverseFlag = false;

	_cameraBackFlag = false;

	_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		DirectX::XMConvertToRadians(45.0f),
		WINDOW_W / (float)WINDOW_H, 0.1f, 3000.0f);
}

//*************************************************************************************
// �X�V����
//*************************************************************************************
void Camera::Update(const int playerExistFace, const int& playerUpDirection)
{
	// �J�����̉�]�i�R���g���[���[�j
	if(g_InputManager->IsCheckRStick())
	{
		
	}

	// �v���C���[�̑��݂���ʂɂ���āA�J�����̈ʒu��ύX������
	float LStickDegreesX = g_InputManager->GetRStickDegreesX();
	float LStickDegreesY = g_InputManager->GetRStickDegreesY();

	// �J�����ʒu�����Z�b�g����
	if( g_InputManager->IsButtonDown(XINPUT_GAMEPAD_RIGHT_THUMB) )
	{
		ResetCameraAngle();
	}

	// �v���C���[�̏�����ɂ���ăJ�����̊p�x��ւ���(���l�ɂ��Ă�player.h�Q��)
	switch(playerUpDirection)
	{
	case 0:
		_up = Vector3(0.0f, 1.0f, 0.0f);
		_radian_X += ROTATION_SPEED_MAX * LStickDegreesX;
		_radian_Y += ROTATION_SPEED_MAX * LStickDegreesY;
		break;

	case 1:
		_up = Vector3(1.0f, 0.0f, 0.0f);
		_radian_X += ROTATION_SPEED_MAX * LStickDegreesY;
		_radian_Y += ROTATION_SPEED_MAX * LStickDegreesX;
		break;

	case 2:
		_up = Vector3(0.0f, -1.0f, 0.0f);
		_radian_X += ROTATION_SPEED_MAX * LStickDegreesX;
		_radian_Y += ROTATION_SPEED_MAX * LStickDegreesY;
		break;

	case 3:
		_up = Vector3(-1.0f, 0.0f, 0.0f);
		_radian_X += ROTATION_SPEED_MAX * LStickDegreesY;
		_radian_Y += ROTATION_SPEED_MAX * LStickDegreesX;
		break;

	default:
		break;
	}

	// �A���O���̏����ݒ肷��
	if(_radian_X >= XMConvertToRadians(180.0f)) _radian_X = XMConvertToRadians(180.0f);
	if(_radian_X <= XMConvertToRadians(-180.0f)) _radian_X = XMConvertToRadians(-180.0f);
	if(_radian_Y >= XMConvertToRadians(180.0f)) _radian_Y = XMConvertToRadians(180.0f);
	if(_radian_Y <= XMConvertToRadians(-180.0f)) _radian_Y = XMConvertToRadians(-180.0f);



	// ���ɍs�����Ƃ��A�J�����̏������ύX����
	if( playerUpDirection == 0 || playerUpDirection == 2)
	{
		if( ( _radian_Y >= XMConvertToRadians(90.0f) || _radian_Y <= XMConvertToRadians(-90.0f) ) && _cameraBackFlag == false)
		{
			_up.x *= -1.0f;
			_up.y *= -1.0f;
			_up.z *= -1.0f;
			_cameraBackFlag = true;
		}else if(_cameraBackFlag == true){
			_up.x *= -1.0f;
			_up.y *= -1.0f;
			_up.z *= -1.0f;
			_cameraBackFlag = false;
		}
	}

	if( playerUpDirection == 1 || playerUpDirection == 3)
	{
		if( ( _radian_X >= XMConvertToRadians(89.9f) || _radian_X <= XMConvertToRadians(-89.9f) ) && _cameraBackFlag == false)
		{
			_up.x *= -1.0f;
			_up.y *= -1.0f;
			_up.z *= -1.0f;
			_cameraBackFlag = true;
		}else if(_cameraBackFlag == true){
			_up.x *= -1.0f;
			_up.y *= -1.0f;
			_up.z *= -1.0f;
			_cameraBackFlag = false;
		}
	}

	// �ڂ̈ʒu�ƃ^�[�Q�b�g���v���C���[�ɍ��킹��
	SetEyePosition(Vector3( DISTANCE_FROM_STAGE * cosf(_radian_Y) * sinf(_radian_X), 
							DISTANCE_FROM_STAGE * sinf(_radian_Y), 
							DISTANCE_FROM_STAGE * cosf(_radian_Y) * cosf(_radian_X)));


	// View���Z�b�g������
	_view = DirectX::SimpleMath::Matrix::CreateLookAt(_eye, _target, _up);
		
}

// �J�����A���O���̃��Z�b�g
void Camera::ResetCameraAngle()
{
	_radian_X = 0.0f;
	_radian_Y = 0.0f;
}