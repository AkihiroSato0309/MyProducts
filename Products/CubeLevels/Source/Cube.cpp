#include "Cube.h"
#include "Global.h"
#include "SModelMgr.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

// floatからintへ誤差を修正して変換する
int TransIntFromFloat(float f)
{
	if(f < 0.0f)
	{
		return (int)(f - 0.2f);
	}

	if(f > 0.0f)
	{
		return (int)(f + 0.2f);
	}

	return 0;
}

//*********************************************************************
// コンストラクタ
//*********************************************************************
Cube::Cube()
{
	
}

Cube::Cube(int x, int y, int z) :
	_movingFlag(false)
{
	// キューブ面ポインタの初期化
	for(int n = 0; n < 3; n++)
	{
		_pCubeFace[n] = nullptr;
	}

	// 位置の初期化
	_currentPosition[0] = _originPosition[0] = x;
	_currentPosition[1] = _originPosition[1] = y;
	_currentPosition[2] = _originPosition[2] = z;

	// 角度の初期化
	for(int i = 0; i < 3; i++)
	{
		_currentRadian[i] = 0.0f;
	}

	// マトリクスの初期化
	_rotation_matrix = Matrix::Identity();
}

//*********************************************************************
// デストラクタ
//*********************************************************************
Cube::~Cube()
{

}


//*********************************************************************
// 更新処理
//*********************************************************************
void Cube::Update(float moveDegree)
{
	if(_movingFlag == true)
	{
		Quaternion q1 = Quaternion::CreateFromRotationMatrix(_before_roation_matrix);
		Quaternion q2 = Quaternion::CreateFromRotationMatrix(_rotation_matrix);
		Quaternion q3 = Quaternion::Slerp(q1, q2, moveDegree);

		_moveRotationMatrix = Matrix::CreateFromQuaternion(q3);
	}
}


//*********************************************************************
// 描画処理
//*********************************************************************
// 止まっている時の描画
void Cube::Render()
{
	
	// 登録されているキューブを描画する
	for(int i = 0; i < 3; i++)
	{
		// キューブが登録されていなかったら抜ける
		if(_pCubeFace[i] == nullptr) break;

		// キューブ描画
		//_pCubeFace[i]->Render();
	}
	
}

// 動いている時の描画
void Cube::RenderMove(float moveDegree)
{
	if(_movingFlag == true)
	{
		// 登録されているキューブを描画する
		for(int i = 0; i < 3; i++)
		{
			// キューブが登録されていなかったら抜ける
			if(_pCubeFace[i] == nullptr) break;

			// キューブ描画
			//_pCubeFace[i]->Render();
		}
	}else{
		Render();
	}
}

//*********************************************************************
// キューブ面の登録
//*********************************************************************
void Cube::EntryCubeFace(CubeFace* cubeFace)
{
	for(int n = 0; n < 3; n++)
	{
		if(_pCubeFace[n] == nullptr)
		{
			//　必要な情報のポインタを渡す
			_pCubeFace[n] = cubeFace;
			_pCubeFace[n]->SetParentMatrix(&_rotation_matrix);
			_pCubeFace[n]->SetParentMovingMatrix(&_moveRotationMatrix);
			_pCubeFace[n]->SetParentPosition(_originPosition);
			_pCubeFace[n]->SetParentRotaionFlag(&_movingFlag);
			break;
		}
	}
}

//*********************************************************************
// 指定した面に属するCubeFaceポインタを返す
//*********************************************************************
CubeFace* Cube::GetAppointCubeFace(int appointFace)
{
	for(int i = 0; i < 3, _pCubeFace[i] != nullptr; i++)
	{
		if(_pCubeFace[i]->GetFaceDirection() == appointFace)
		{
			return _pCubeFace[i];
		}
	}

	return nullptr;
}

//*********************************************************************
// 回転による位置情報の改変
//*********************************************************************
void Cube::TransPosition(eRotation rotation, const int& turn_direction, Matrix& worldMatrix)
{
	// 回転の中心
	Vector3 tmp, axis(0.0f, 0.0f, 0.0f);

	switch(rotation)
	{
	case eRotation_x:
		_currentRadian[0] += XMConvertToRadians(90.0f);
		// 変更前のマトリクスを保持
		_before_roation_matrix = _rotation_matrix;
		_rotation_matrix *= Matrix::CreateFromAxisAngle(AXIS_X, XMConvertToRadians(90.0f) * turn_direction);
		axis.x = 1.0f;
		// 連結しているキューブの面情報を変更
		for(int i = 0; i < 3, _pCubeFace[i] != nullptr; i++)
		{
			_pCubeFace[i]->ChangeFaceDirectionX(turn_direction);
			_pCubeFace[i]->SetCubeFaceRenderFlag();
		}
		break;

	case eRotation_y:
		_currentRadian[1] += XMConvertToRadians(90.0f);
		// 変更前のマトリクスを保持
		_before_roation_matrix = _rotation_matrix;
		_rotation_matrix *= Matrix::CreateFromAxisAngle(AXIS_Y, XMConvertToRadians(90.0f) * turn_direction);
		axis.y = 1.0f;
		// 連結しているキューブの面情報を変更
		for(int i = 0; i < 3, _pCubeFace[i] != nullptr; i++)
		{
			_pCubeFace[i]->ChangeFaceDirectionY(turn_direction);
			_pCubeFace[i]->SetCubeFaceRenderFlag();
		}
		break;

	case eRotation_z:
		_currentRadian[2] += XMConvertToRadians(90.0f);
		// 変更前のマトリクスを保持
		_before_roation_matrix = _rotation_matrix;
		_rotation_matrix *= Matrix::CreateFromAxisAngle(AXIS_Z, XMConvertToRadians(90.0f) * turn_direction);
		axis.z = 1.0f;
		// 連結しているキューブの面情報を変更
		for(int i = 0; i < 3, _pCubeFace[i] != nullptr; i++)
		{
			_pCubeFace[i]->ChangeFaceDirectionZ(turn_direction);
			_pCubeFace[i]->SetCubeFaceRenderFlag();
		}
		break;

	default:
		break;
	}

	// 位置情報を更新
	Vector3::Transform(	Vector3(_currentPosition[0] - 1.0f, _currentPosition[1] - 1.0f, _currentPosition[2] - 1.0f), 
			Matrix::CreateFromAxisAngle(axis, XMConvertToRadians(90.0f * turn_direction)),
			tmp);

	//Vector3::Transform( tmp, worldMatrix, tmp );

	// floatをintに確実に変換する
	_currentPosition[0] = TransIntFromFloat(tmp.x) + 1;
	_currentPosition[1] = TransIntFromFloat(tmp.y) + 1;
	_currentPosition[2] = TransIntFromFloat(tmp.z) + 1;

	_movingFlag = true;

	Matrix position = Matrix::CreateTranslation(	(float)CUBE_SIZE * (_currentPosition[0] - 1),
													(float)CUBE_SIZE * (_currentPosition[1] - 1),
													(float)CUBE_SIZE * (_currentPosition[2] - 1));

}

