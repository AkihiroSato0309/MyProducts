#include "Stage.h"
#include "Global.h"
#include "FrontFaceMatrix.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

static const Matrix s_RotationUp	= Matrix::CreateFromAxisAngle(Vector3(1.0f, 0.0f, 0.0f), XMConvertToRadians(90.0f));
static const Matrix s_RotationDown	= Matrix::CreateFromAxisAngle(Vector3(1.0f, 0.0f, 0.0f), XMConvertToRadians(-90.0f));
static const Matrix s_RotationRight = Matrix::CreateFromAxisAngle(Vector3(0.0f, 1.0f, 0.0f), XMConvertToRadians(-90.0f));
static const Matrix s_RotationLeft	= Matrix::CreateFromAxisAngle(Vector3(0.0f, 1.0f, 0.0f), XMConvertToRadians(90.0f));

//*********************************************************************
// コンストラクタ
//*********************************************************************
Stage::Stage(int* map)
{
	// キューブを作成する（順番は右手座標系）
	for(int x = 0; x < 3; x++)
	{
		for(int y = 0; y < 3; y++)
		{
			for(int z = 0; z < 3; z++)
			{
				_cube[x][y][z] = new Cube(x, y, z);
			}
		}
	}

	// キューブに貼り付けるための4次元配列
	int mappingAry[6][9][5][5];

	// 1次元配列を4次元配列に入れ直す
	for(int v = 0; v < 6; v++)
	{
		for(int w = 0; w < 3; w++)
		{
			for(int x = 0; x < 3; x++)
			{
				for(int y = 0; y < 5; y++)
				{
					for(int z = 0; z < 5; z++)
					{
						mappingAry[v][w * 3 + x][y][z] = map[225 * v + 75 * w + 5 * x + 15 * y + z];
					}
				}
			}
		}
	}

	// キューブの面を面ごとに初期化
	for(int face = 0; face < 6; face++)
	{
		for(int x = 0; x < 3; x++)
		{
			for(int y = 0; y < 3; y++)
			{
				_pCubeFace[face][x][y] = new CubeFace(face, mappingAry[face][3 * x + y]);
		
			}
		}
	}

	// 回転情報を初期化する
	_rotationInfo.rotationFlag = false;

	// 各面の中心位置情報を初期化(順番は列挙型と同一)
	_faceCenterPosition[eTop]		= Vector3( 0.0f,  CUBE_SIZE_HALF * 3, 0.0f );
	_faceCenterPosition[eBottom]	= Vector3( 0.0f, -CUBE_SIZE_HALF * 3, 0.0f );
	_faceCenterPosition[eRight]		= Vector3(  CUBE_SIZE_HALF * 3,	0.0f, 0.0f );
	_faceCenterPosition[eLeft]		= Vector3( -CUBE_SIZE_HALF * 3,	0.0f, 0.0f );
	_faceCenterPosition[eFront]		= Vector3( 0.0f, 0.0f,  CUBE_SIZE_HALF * 3 );
	_faceCenterPosition[eBack]		= Vector3( 0.0f, 0.0f, -CUBE_SIZE_HALF * 3 );

	// マトリクスの初期化
	_rotationMatrix = Matrix::Identity();

	//キューブのマトリクスアドレスを面に連結させる
	_cube[0][2][0]->EntryCubeFace(_pCubeFace[eTop][0][0]);
	_cube[1][2][0]->EntryCubeFace(_pCubeFace[eTop][0][1]);
	_cube[2][2][0]->EntryCubeFace(_pCubeFace[eTop][0][2]);
	_cube[0][2][1]->EntryCubeFace(_pCubeFace[eTop][1][0]);
	_cube[1][2][1]->EntryCubeFace(_pCubeFace[eTop][1][1]);
	_cube[2][2][1]->EntryCubeFace(_pCubeFace[eTop][1][2]);
	_cube[0][2][2]->EntryCubeFace(_pCubeFace[eTop][2][0]);
	_cube[1][2][2]->EntryCubeFace(_pCubeFace[eTop][2][1]);
	_cube[2][2][2]->EntryCubeFace(_pCubeFace[eTop][2][2]);

	_cube[0][0][2]->EntryCubeFace(_pCubeFace[eBottom][0][0]);
	_cube[1][0][2]->EntryCubeFace(_pCubeFace[eBottom][0][1]);
	_cube[2][0][2]->EntryCubeFace(_pCubeFace[eBottom][0][2]);
	_cube[0][0][1]->EntryCubeFace(_pCubeFace[eBottom][1][0]);
	_cube[1][0][1]->EntryCubeFace(_pCubeFace[eBottom][1][1]);
	_cube[2][0][1]->EntryCubeFace(_pCubeFace[eBottom][1][2]);
	_cube[0][0][0]->EntryCubeFace(_pCubeFace[eBottom][2][0]);
	_cube[1][0][0]->EntryCubeFace(_pCubeFace[eBottom][2][1]);
	_cube[2][0][0]->EntryCubeFace(_pCubeFace[eBottom][2][2]);

	_cube[2][2][2]->EntryCubeFace(_pCubeFace[eRight][0][0]);
	_cube[2][2][1]->EntryCubeFace(_pCubeFace[eRight][0][1]);
	_cube[2][2][0]->EntryCubeFace(_pCubeFace[eRight][0][2]);
	_cube[2][1][2]->EntryCubeFace(_pCubeFace[eRight][1][0]);
	_cube[2][1][1]->EntryCubeFace(_pCubeFace[eRight][1][1]);
	_cube[2][1][0]->EntryCubeFace(_pCubeFace[eRight][1][2]);
	_cube[2][0][2]->EntryCubeFace(_pCubeFace[eRight][2][0]);
	_cube[2][0][1]->EntryCubeFace(_pCubeFace[eRight][2][1]);
	_cube[2][0][0]->EntryCubeFace(_pCubeFace[eRight][2][2]);

	_cube[0][2][0]->EntryCubeFace(_pCubeFace[eLeft][0][0]);
	_cube[0][2][1]->EntryCubeFace(_pCubeFace[eLeft][0][1]);
	_cube[0][2][2]->EntryCubeFace(_pCubeFace[eLeft][0][2]);
	_cube[0][1][0]->EntryCubeFace(_pCubeFace[eLeft][1][0]);
	_cube[0][1][1]->EntryCubeFace(_pCubeFace[eLeft][1][1]);
	_cube[0][1][2]->EntryCubeFace(_pCubeFace[eLeft][1][2]);
	_cube[0][0][0]->EntryCubeFace(_pCubeFace[eLeft][2][0]);
	_cube[0][0][1]->EntryCubeFace(_pCubeFace[eLeft][2][1]);
	_cube[0][0][2]->EntryCubeFace(_pCubeFace[eLeft][2][2]);

	_cube[0][2][2]->EntryCubeFace(_pCubeFace[eFront][0][0]);
	_cube[1][2][2]->EntryCubeFace(_pCubeFace[eFront][0][1]);
	_cube[2][2][2]->EntryCubeFace(_pCubeFace[eFront][0][2]);
	_cube[0][1][2]->EntryCubeFace(_pCubeFace[eFront][1][0]);
	_cube[1][1][2]->EntryCubeFace(_pCubeFace[eFront][1][1]);
	_cube[2][1][2]->EntryCubeFace(_pCubeFace[eFront][1][2]);
	_cube[0][0][2]->EntryCubeFace(_pCubeFace[eFront][2][0]);
	_cube[1][0][2]->EntryCubeFace(_pCubeFace[eFront][2][1]);
	_cube[2][0][2]->EntryCubeFace(_pCubeFace[eFront][2][2]);

	_cube[2][2][0]->EntryCubeFace(_pCubeFace[eBack][0][0]);
	_cube[1][2][0]->EntryCubeFace(_pCubeFace[eBack][0][1]);
	_cube[0][2][0]->EntryCubeFace(_pCubeFace[eBack][0][2]);
	_cube[2][1][0]->EntryCubeFace(_pCubeFace[eBack][1][0]);
	_cube[1][1][0]->EntryCubeFace(_pCubeFace[eBack][1][1]);
	_cube[0][1][0]->EntryCubeFace(_pCubeFace[eBack][1][2]);
	_cube[2][0][0]->EntryCubeFace(_pCubeFace[eBack][2][0]);
	_cube[1][0][0]->EntryCubeFace(_pCubeFace[eBack][2][1]);
	_cube[0][0][0]->EntryCubeFace(_pCubeFace[eBack][2][2]);

	// 回転中フラグを初期化
	_moveFlag = false;

	// タイム初期化
	_time = 0;

	// 描画順管理配列を初期化
	for(int i = 0; i < 6; i++)
	{
		_renderMgr[i].sFace = i;
		_renderMgr[i].sDistance = 0.0f;
		_renderMgr[i].sFaceCenterPosition = _faceCenterPosition[i];
	}

	// 面ごとに整理する
	AllAppoint(_pRenderFace[0], 0, Y, 2);
	AllAppoint(_pRenderFace[1], 1, Y, 0);
	AllAppoint(_pRenderFace[2], 2, X, 2);
	AllAppoint(_pRenderFace[3], 3, X, 0);
	AllAppoint(_pRenderFace[4], 4, Z, 2);
	AllAppoint(_pRenderFace[5], 5, Z, 0);

	// 2Dステージの初期化
	_pStage2D = new Stage2D();
	_pStage2D->CreateStage(map);
}

//*********************************************************************
// デストラクタ
//*********************************************************************
Stage::~Stage()
{
	// キューブのデストラクタ
	for(int x = 0; x < 3; x++)
	{
		for(int y = 0; y < 3; y++)
		{
			for(int z = 0; z < 3; z++)
			{
				delete _cube[x][y][z];
			}
		}
	}

	// キューブの面をデストラクタ
	for(int face = 0; face < 6; face++)
	{
		for(int y = 0; y < 3; y++)
		{
			for(int x = 0; x < 3; x++)
			{
				delete _pCubeFace[face][y][x];
			}
		}
	}
	delete _pStage2D;
}

//*********************************************************************
// 初期化処理
//*********************************************************************
void Stage::Initialize()
{
	
}

//*********************************************************************
// 更新処理
//*********************************************************************
void Stage::Update()
{
	// 回転中のフラグが立っている時の処理
	if(_moveFlag == true)
	{
		_time++;

		// 回転の割合を出す
		_moveDegree = (float)_time / MOVE_FLAME;

		// 回転終了時
		if(_time > MOVE_FLAME)
		{
			_moveFlag = false;
			_time = 0;

			for(int x = 0; x < 3; x++)
			{
				for(int y = 0; y < 3; y++)
				{
					for(int z = 0; z < 3; z++)
					{
						
						_cube[x][y][z]->ResetRotationFlag();
						
					}
				}
			}
		}
	}
	for(int x = 0; x < 3; x++)
	{
		for(int y = 0; y < 3; y++)
		{
			for(int z = 0; z < 3; z++)
			{
				_cube[x][y][z]->Update(_moveDegree);
			}
		}
	}
	
	// カメラと各面の中心との距離を計算する
	for(int i = 0; i < 6; i++)
	{
		_renderMgr[i].sDistance = Vector3::Distance( g_Camera->GetEyePosition(), _renderMgr[i].sFaceCenterPosition);
	}

	// 近い順に並び替える
	FaceRemderMgr tmpNear;
	for(int i = 0; i < 5; i++)
	{
		for(int j = i + 1; j < 6; j++)
		{
			if(_renderMgr[i].sDistance < _renderMgr[j].sDistance)
			{
				tmpNear = _renderMgr[i];
				_renderMgr[i] = _renderMgr[j];
				_renderMgr[j] = tmpNear;
			}
		}
	}

	// 面ごとに整理する
	AllAppoint(_pRenderFace[0], 0, 2, Y);
	AllAppoint(_pRenderFace[1], 1, 0, Y);
	AllAppoint(_pRenderFace[2], 2, 2, X);
	AllAppoint(_pRenderFace[3], 3, 0, X);
	AllAppoint(_pRenderFace[4], 4, 2, Z);
	AllAppoint(_pRenderFace[5], 5, 0, Z);
}

//*********************************************************************
// 描画処理
//*********************************************************************
void Stage::Render(const int& playerExist)
{
	Matrix l_frontFaceMatrix;

	// プレイヤーの存在する面によって、渡すマトリクスを替える
	switch( playerExist )
	{
	case eTop:
		l_frontFaceMatrix = FRONT_TOP;
		break;

	case eBottom:
		l_frontFaceMatrix = FRONT_BOTTOM;
		break;
		
	case eRight:
		l_frontFaceMatrix = FRONT_RIGHT;
		break;

	case eLeft:
		l_frontFaceMatrix = FRONT_LEFT;
		break;

	case eFront:
		l_frontFaceMatrix = FRONT_FRONT;
		break;

	case eBack:
		l_frontFaceMatrix = FRONT_BACK;
		break;

	default:
		break;

	}

	//描画
	for(int i = 0; i < 6; i++)
	{
		for(int j = 0; j < 9; j++)
		{
			_pRenderFace[_renderMgr[i].sFace][j]->Render(_rotationMatrix, l_frontFaceMatrix);
		}
	}
}
//*********************************************************************
// キューブ回転処理
//*********************************************************************
bool Stage::RotationCube(const int& playerExistFace, const int& playerUpDirect )
{
	if( g_InputManager->IsButtonDown(XINPUT_GAMEPAD_RIGHT_SHOULDER))
	{
		// 上側の時計回転
		if( g_InputManager->IsButtonPressed(XINPUT_GAMEPAD_LEFT_SHOULDER))
		{
			return HorizontallyRotation(playerExistFace, playerUpDirect, 2, TURN_RIGHT);
		}

		// 右側の時計回転
		if( g_InputManager->IsR2Pressed() )
		{
			return VerticalRotation(playerExistFace, playerUpDirect, 2, TURN_RIGHT);
		}
	}

	if( g_InputManager->IsButtonDown(XINPUT_GAMEPAD_LEFT_SHOULDER))
	{
		// 上側の反時計回転
		if( g_InputManager->IsButtonPressed(XINPUT_GAMEPAD_RIGHT_SHOULDER) )
		{
			return HorizontallyRotation(playerExistFace, playerUpDirect, 2, TURN_LEFT);
		}

		// 左側の時計回転
		if( g_InputManager->IsL2Pressed() )
		{
			return VerticalRotation(playerExistFace, playerUpDirect, 0, TURN_RIGHT);
		}
	}

	if( g_InputManager->IsR2Down() )
	{
		// 右側の反時計回転
		if( g_InputManager->IsButtonPressed(XINPUT_GAMEPAD_RIGHT_SHOULDER) )
		{
			return VerticalRotation(playerExistFace, playerUpDirect, 2, TURN_LEFT);
		}

		// 下側の反時計回転
		if( g_InputManager->IsL2Pressed() )
		{
			return HorizontallyRotation(playerExistFace, playerUpDirect, 0, TURN_RIGHT);
		}
	}

	if( g_InputManager->IsL2Down() )
	{
		// 左側の反時計回転
		if( g_InputManager->IsButtonPressed(XINPUT_GAMEPAD_LEFT_SHOULDER) )
		{
			return VerticalRotation(playerExistFace, playerUpDirect, 0, TURN_LEFT);
		}

		// 下側の時計回転
		if( g_InputManager->IsR2Pressed() )
		{
			return HorizontallyRotation(playerExistFace, playerUpDirect, 0,TURN_LEFT);
		}
	}

	// 仮回転処理
	if( g_InputManager->IsButtonPressed(XINPUT_GAMEPAD_Y) )
	{
		RotationCubeX(1, TURN_RIGHT);
		return true;
	}

	if( g_InputManager->IsButtonPressed(XINPUT_GAMEPAD_A) )
	{
		RotationCubeX(1, TURN_LEFT);
		return true;
	}

	if( g_InputManager->IsButtonPressed(XINPUT_GAMEPAD_X) )
	{
		RotationCubeY(1, TURN_RIGHT);
		return true;
	}

	if( g_InputManager->IsButtonPressed(XINPUT_GAMEPAD_B) )
	{
		RotationCubeY(1, TURN_LEFT);
		return true;
	}

	// デバッグ時の仮操作
#ifdef _DEBUG


	if( g_InputManager->IsKeyDown('U'))
	{
		// 上側の時計回転
		if(  g_InputManager->IsKeyPressed('T'))
		{
			RotationCubeY(2, TURN_RIGHT);
			return true;
		}

		// 右側の時計回転
		if(  g_InputManager->IsKeyPressed('M'))
		{
			RotationCubeX(2, TURN_RIGHT);
			return true;
		}
	}

	if( g_InputManager->IsKeyDown('T'))
	{
		// 上側の反時計回転
		if( g_InputManager->IsKeyPressed('U') )
		{
			RotationCubeY(2, TURN_LEFT);
			return true;
		}

		// 左側の時計回転
		if( g_InputManager->IsKeyPressed('B') )
		{
			RotationCubeX(0, TURN_RIGHT);
			return true;
		}
	}

	if( g_InputManager->IsKeyDown('M') )
	{
		// 右側の反時計回転
		if( g_InputManager->IsKeyPressed('U') )
		{
			RotationCubeX(2, TURN_LEFT);
			return true;
		}

		// 下側の半時計回転
		if( g_InputManager->IsKeyPressed('B') )
		{
			RotationCubeY(0, TURN_RIGHT);
			return true;
		}
	}

	if( g_InputManager->IsKeyDown('B') )
	{
		// 右側の反時計回転
		if( g_InputManager->IsKeyPressed('T') )
		{
			RotationCubeX(0, TURN_LEFT);
			return true;
		}

		// 左側の半時計回転
		if( g_InputManager->IsKeyPressed('M') )
		{
			RotationCubeY(0, TURN_LEFT);
			return true;
		}
	}

	// 仮回転処理
	if( g_InputManager->IsKeyDown('Y') )
	{
		RotationCubeX(1, TURN_RIGHT);
		return true;
	}

	if( g_InputManager->IsKeyDown('N') )
	{
		RotationCubeX(1, TURN_LEFT);
		return true;
	}

	if( g_InputManager->IsKeyDown('G') )
	{
		RotationCubeY(1, TURN_RIGHT);
		return true;
	}

	if( g_InputManager->IsKeyDown('J') )
	{
		RotationCubeY(1, TURN_LEFT);
		return true;
	}

#endif
	
	return false;
}

//*********************************************************************
// 指定した面に属するキューブの面配列の先頭アドレスを返す
//*********************************************************************
CubeFace** Stage::GetAppointCubeFace(int appointDirection)
{	
	switch(appointDirection)
	{
	case eTop:
		return _pRenderFace[0];
		break;

	case eBottom:
		return _pRenderFace[1];
		break;

	case eRight:
		return _pRenderFace[2]; 
		break;

	case eLeft:
		return _pRenderFace[3];
		break;

	case eFront:
		return _pRenderFace[4];
		break;

	case eBack:
		return _pRenderFace[5];
		break;

	default:
		break;
	}
	
	return nullptr;
}

// 指定面のキューブ配列を返す際に使用する総当り処理
void Stage::AllAppoint(CubeFace** face, int appointDirection, int cubePosition, int searchPosition)
{
	int i = 0;
	for(int x = 0; x < 3; x++)
	{
		for(int y = 0; y < 3; y++)
		{
			for(int z = 0; z < 3; z++)
			{
				if(_cube[x][y][z]->GetCurrentPoaition()[searchPosition] == cubePosition)
				{
					face[i] = _cube[x][y][z]->GetAppointCubeFace(appointDirection);
					i++;
				}
			}
		}
	}
	return;
}

//****************************************************************************
// キューブ回転処理(軸中心で、原点からの個数)
//****************************************************************************
void Stage::RotationCubeX(int num, const int& turn_direction)
{	
	// すでに入力が確定されている場合、更新しない
	if(_rotationInfo.rotationFlag == true) return;

	for(int x = 0; x < 3; x++)
	{
		for(int y = 0; y < 3; y++)
		{
			for(int z = 0; z < 3; z++)
			{
				if(_cube[x][y][z]->GetCurrentPoaition()[0] == num)  
				{
					_cube[x][y][z]->TransPosition(eRotation_x, turn_direction, _rotationMatrix);
				}
			}
		}
	}

	// 当たり判定を回す
	_pStage2D->RotationCubeX(num, turn_direction);

	// プレイヤーに回転処理を伝えるため、回転情報を更新する
	_rotationInfo.rotationFlag = true;
	_rotationInfo.axis = 'x';
	_rotationInfo.rotationDirect = turn_direction;
	_rotationInfo.rotationNum = num;
}
void Stage::RotationCubeY(int num, const int& turn_direction)
{
	if(_rotationInfo.rotationFlag == true) return;

	for(int x = 0; x < 3; x++)
	{
		for(int y = 0; y < 3; y++)
		{
			for(int z = 0; z < 3; z++)
			{
				if(_cube[x][y][z]->GetCurrentPoaition()[1] == num)  
					_cube[x][y][z]->TransPosition(eRotation_y, turn_direction, _rotationMatrix);
			}
		}
	}

	// 当たり判定を回す
	_pStage2D->RotationCubeY(num, turn_direction);

	// プレイヤーに回転処理を伝えるため、回転情報を更新する
	_rotationInfo.rotationFlag = true;
	_rotationInfo.axis = 'y';
	_rotationInfo.rotationDirect = turn_direction;
	_rotationInfo.rotationNum = num;
}
void Stage::RotationCubeZ(int num, const int& turn_direction)
{
	if(_rotationInfo.rotationFlag == true) return;

	for(int x = 0; x < 3; x++)
	{
		for(int y = 0; y < 3; y++)
		{
			for(int z = 0; z < 3; z++)
			{
				if(_cube[x][y][z]->GetCurrentPoaition()[2] == num)  
					_cube[x][y][z]->TransPosition(eRotation_z, turn_direction, _rotationMatrix);
			}
		}
	}

	// 当たり判定を回す
	_pStage2D->RotationCubeZ(num, turn_direction);

	// プレイヤーに回転処理を伝えるため、回転情報を更新する
	_rotationInfo.rotationFlag = true;
	_rotationInfo.axis = 'z';
	_rotationInfo.rotationDirect = turn_direction;
	_rotationInfo.rotationNum = num;
}

// マトリクスを操作する
void Stage::ChangeMatrix(eRotationMatrix rotationDirect)
{
	switch(rotationDirect)
	{
	case eRotationUp:
		_rotationMatrix *= s_RotationUp;
		break;

	case eRotationDown:
		_rotationMatrix *= s_RotationDown;
		break;

	case eRotationRight:
		_rotationMatrix *= s_RotationRight;
		break;

	case eRotationLeft:
		_rotationMatrix *= s_RotationLeft;
		break;

	default:
		break;
	}
}

// 回転中かどうかを取得する
bool Stage::GetMovingFlag()
{
	return _moveFlag;
}

// 回転情報フラグを取得し、フラグを倒す
bool Stage::GetRotationInfoFlag()
{
	bool tmpFlag = _rotationInfo.rotationFlag;
	_rotationInfo.rotationFlag = false;
	return tmpFlag;
}

// 外部用キューブ回転処理
void Stage::Rotation(const int& playerExistFace, const int& playerUpDirect)
{
	if(_moveFlag == false)
	{
		_moveFlag = RotationCube(playerExistFace, playerUpDirect);
	}
}

// プレイヤーの存在面、上方向を考慮した縦回転を行う(正面、上方向、右側右回転を元として)
bool Stage::VerticalRotation(const int& playerExistFace, const int& playerUpDirect, const int& rotationNum, const int& rotationDirect)
{
	if( playerUpDirect == eUpDirect &&
		(playerExistFace == eTop ||
		playerExistFace == eFront ||
		playerExistFace == eBottom) )
	{
		RotationCubeX(rotationNum, rotationDirect);
		return true;
	}
	if( playerUpDirect == eDownDirect &&
		(playerExistFace == eTop ||
		playerExistFace == eFront ||
		playerExistFace == eBottom) )
	{
		RotationCubeX(ReverseRotationNum(rotationNum), ReverseRotationDirect(rotationDirect));
		return true;
	}
			
	if( playerUpDirect == eRightDirect &&
		(playerExistFace == eFront ||
		playerExistFace == eRight ||
		playerExistFace == eBack ||
		playerExistFace == eLeft) )
	{
		RotationCubeY(ReverseRotationNum(rotationNum), ReverseRotationDirect(rotationDirect));
		return true;
	}
	if( playerUpDirect == eLeftDirect &&
		(playerExistFace == eFront ||
		playerExistFace == eRight ||
		playerExistFace == eBack ||
		playerExistFace == eLeft) )
	{
		RotationCubeY(rotationNum, rotationDirect);
		return true;
	}

	// 一気にできなかった残りの処理
	if(playerExistFace == eTop)
	{
		if(playerUpDirect == eRightDirect) RotationCubeZ(rotationNum, rotationDirect);
		if(playerUpDirect == eLeftDirect) RotationCubeZ(ReverseRotationNum(rotationNum), ReverseRotationDirect(rotationDirect));
		return true;
	}
	if(playerExistFace == eBottom)
	{
		if(playerUpDirect == eRightDirect) RotationCubeZ(ReverseRotationNum(rotationNum), ReverseRotationDirect(rotationDirect));
		if(playerUpDirect == eLeftDirect) RotationCubeZ(rotationNum, rotationDirect);
		return true;
	}
	if(playerExistFace == eBack)
	{
		if(playerUpDirect == eUpDirect) RotationCubeX(ReverseRotationNum(rotationNum), ReverseRotationDirect(rotationDirect));
		if(playerUpDirect == eDownDirect ) RotationCubeX(rotationNum, rotationDirect);
		return true;
	}
	if(playerExistFace == eRight)
	{
		if(playerUpDirect == eUpDirect) RotationCubeZ(ReverseRotationNum(rotationNum), ReverseRotationDirect(rotationDirect));
		if(playerUpDirect == eDownDirect) RotationCubeZ(rotationNum, rotationDirect);
		return true;
	}
	if(playerExistFace == eLeft)
	{
		if(playerUpDirect == eUpDirect) RotationCubeZ(rotationNum, rotationDirect);
		if(playerUpDirect == eDownDirect) RotationCubeZ(ReverseRotationNum(rotationNum), ReverseRotationDirect(rotationDirect));
		return true;
	}
}

// 横回転を行う
bool Stage::HorizontallyRotation(const int& playerExistFace, const int& playerUpDirect, const int& rotationNum, const int& rotationDirect)
{
	if( playerUpDirect == eUpDirect && 
		(playerExistFace == eFront ||
		 playerExistFace == eRight ||
		 playerExistFace == eBack ||
		 playerExistFace == eLeft ))
	{
		//				2           right
		RotationCubeY(rotationNum, rotationDirect);
		return true;
	}
	if( playerUpDirect == eDownDirect && 
		(playerExistFace == eFront ||
			playerExistFace == eRight ||
			playerExistFace == eBack ||
			playerExistFace == eLeft))
	{
		RotationCubeY(ReverseRotationNum(rotationNum), ReverseRotationDirect(rotationDirect));
		return true;
	}
	if( playerExistFace == eFront )
	{
		if(playerUpDirect == eRightDirect) RotationCubeX(rotationNum, rotationDirect); 
		if(playerUpDirect == eLeftDirect ) RotationCubeX(ReverseRotationNum(rotationNum), ReverseRotationDirect(rotationDirect));
	}
	if( playerExistFace == eRight )
	{
		if(playerUpDirect == eRightDirect) RotationCubeZ(ReverseRotationNum(rotationNum), ReverseRotationDirect(rotationDirect)); 
		if(playerUpDirect == eLeftDirect ) RotationCubeZ(rotationNum, rotationDirect);
	}
	if( playerExistFace == eBack )
	{
		if(playerUpDirect == eRightDirect) RotationCubeX(ReverseRotationNum(rotationNum), ReverseRotationDirect(rotationDirect)); 
		if(playerUpDirect == eLeftDirect ) RotationCubeX(rotationNum, rotationDirect);
	}
	if( playerExistFace == eLeft )
	{
		if(playerUpDirect == eRightDirect) RotationCubeZ(rotationNum, rotationDirect);
		if(playerUpDirect == eLeftDirect ) RotationCubeZ(ReverseRotationNum(rotationNum), ReverseRotationDirect(rotationDirect));
	}
	if( playerExistFace == eTop )
	{
		if(playerUpDirect == eUpDirect ) RotationCubeZ(ReverseRotationNum(rotationNum), ReverseRotationDirect(rotationDirect));
		if(playerUpDirect == eRightDirect) RotationCubeX(rotationNum, rotationDirect);
		if(playerUpDirect == eDownDirect ) RotationCubeZ(rotationNum, rotationDirect);
		if(playerUpDirect == eLeftDirect ) RotationCubeX(ReverseRotationNum(rotationNum), ReverseRotationDirect(rotationDirect));
	}
	if( playerExistFace == eBottom )
	{
		if(playerUpDirect == eUpDirect ) RotationCubeZ(rotationNum, rotationDirect);
		if(playerUpDirect == eRightDirect) RotationCubeX(rotationNum, rotationDirect);
		if(playerUpDirect == eDownDirect ) RotationCubeZ(ReverseRotationNum(rotationNum), ReverseRotationDirect(rotationDirect));
		if(playerUpDirect == eLeftDirect ) RotationCubeX(ReverseRotationNum(rotationNum), ReverseRotationDirect(rotationDirect));
	}
	return true;
}

// 引数に入力された回転方向の逆を戻り値とする
int Stage::ReverseRotationDirect(const int& rotationDirect)
{
	return rotationDirect * -1;
}

// 引数に入力されたキューブナンバーを反転させる
int Stage::ReverseRotationNum(const int& rotationNum)
{
	return 2 - rotationNum;
}