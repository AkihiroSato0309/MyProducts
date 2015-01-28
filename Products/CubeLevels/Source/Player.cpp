#include "Player.h"
#include "Cube.h"
#include "STextureMgr.h"
#include "FrontFaceMatrix.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

VertexPositionColorTexture playerVertexes[4] = {
	VertexPositionColorTexture(Vector3( -PLAYER_SIZE_HALF,  0.0f,  -PLAYER_SIZE_HALF), Vector4( 1.0f, 1.0f, 1.0f, 0.0f ), Vector2( 0.0f, 0.0f )),	// 0
	VertexPositionColorTexture(Vector3(  PLAYER_SIZE_HALF,  0.0f,  -PLAYER_SIZE_HALF), Vector4( 1.0f, 1.0f, 1.0f, 0.0f ), Vector2( 1.0f, 0.0f )),	// 1
	VertexPositionColorTexture(Vector3( -PLAYER_SIZE_HALF,  0.0f,   PLAYER_SIZE_HALF), Vector4( 1.0f, 1.0f, 1.0f, 0.0f ), Vector2( 0.0f, 1.0f )),	// 2
	VertexPositionColorTexture(Vector3(  PLAYER_SIZE_HALF,  0.0f,   PLAYER_SIZE_HALF), Vector4( 1.0f, 1.0f, 1.0f, 0.0f ), Vector2( 1.0f, 1.0f )),	// 3
};

// 静的なメンバ変数の定義(反時計回りがプラス)
Matrix sTopMatrix		= Matrix::CreateFromAxisAngle(Vector3(1.0f, 0.0f, 0.0f), XMConvertToRadians(-90.0f));
Matrix sBottomMatrix	= Matrix::CreateFromAxisAngle(Vector3(1.0f, 0.0f, 0.0f), XMConvertToRadians(90.0f));
Matrix sRightMatrix		= Matrix::CreateFromAxisAngle(Vector3(0.0f, 1.0f, 0.0f), XMConvertToRadians(90.0f));
Matrix sLeftMatrix		= Matrix::CreateFromAxisAngle(Vector3(0.0f, 1.0f, 0.0f), XMConvertToRadians(-90.0f));
Matrix sFrontMatrix		= Matrix::Identity();
Matrix sBackMatrix		= Matrix::CreateFromAxisAngle(Vector3(0.0f, 1.0f, 0.0f), XMConvertToRadians(-180.0f));


//******************************************************************
// コンストラクタ
//******************************************************************
Player::Player()
	: _2DMoveVector(0.0f,0.0f)
	, _2Dposition(15.0f, 20.0f)
	, _jumpFlag(true)
{
	// テクスチャをロード
	STextureMgr::GetInstance()->Load( _pTexture, L"Image/PlayerTexture.png" );

	// プレイヤーの存在する面
	_existFace = eFront;

	// 初期のプレイヤーアップ方向は上に定める
	_upDirection = eUpDirect;
}

//******************************************************************
// デストラクタ
//******************************************************************
Player::~Player()
{

}
//******************************************************************
// 更新処理
//******************************************************************
bool Player::Update(int* map, const bool& cubeMovingFlag)
{
	// 2次配列に変換
	int l_map[15][15];
	for(int y = 0; y < 15; y++)
	{
		for(int x = 0; x < 15; x++)
		{
			l_map[y][x] = *(map + (y * 15 + x));
		}
	}

	//キューブが回転中の場合、プレイヤーを動かさない
	if(cubeMovingFlag == false)
	{
		// 落下処理
		switch(_upDirection)
		{
		case eUpDirect:
			_2DMoveVector.y += _jumpPower;
			break;

		case eRightDirect:
			_2DMoveVector.x += _jumpPower;
			break;

		case eDownDirect:
			_2DMoveVector.y -= _jumpPower;
			break;

		case eLeftDirect:
			_2DMoveVector.x -= _jumpPower;
			break;
		}
		_jumpPower -= G;

		// 落下速度に上限を設ける
		if(_jumpPower < DOWNSPEED_MAX) _jumpPower = DOWNSPEED_MAX;

		// 移動処理
		if(Move(&_2Dposition.x, &_2Dposition.y, &_jumpPower, _2DMoveVector.x, _2DMoveVector.y
			, &_jumpFlag, l_map) == true)
			return true;
	}

	// 移動予定距離がほぼ0のとき、移動中フラグをfalseにする
	if(_2DMoveVector.x < 0.1f && _2DMoveVector.y < 0.1f &&  _2DMoveVector.x > -0.1f && _2DMoveVector.y > -0.1f)
	{
		_movingFlag = false;
	}else{
		_movingFlag = true;
	}

	// 移動予定分をリセット
	_2DMoveVector.x = 0.0f;
	_2DMoveVector.y = 0.0f;

#pragma region プレイヤーのキューブ面移動処理

	// 左へ
	if(_2Dposition.x - PLAYER_SIZE_HALF < 0.0f)
	{
		if(_existFace == eTop)
		{
			_2Dposition.x = CUBE_SIZE * 3 - _2Dposition.y;
			_2Dposition.y = CUBE_SIZE * 3 - PLAYER_SIZE_HALF - 0.02f;
			ChangeUpDirection(eChange270);

		}else if(_existFace == eBottom)
		{
			_2Dposition.x = _2Dposition.y;
			_2Dposition.y = PLAYER_SIZE_HALF + 0.02f;
			ChangeUpDirection(eChange90);
		}else{
			_2Dposition.x = CUBE_SIZE * 3 - PLAYER_SIZE_HALF - 0.02f;
		}

		//_pMatrixChanger->ChangeMatrix(eRotationLeft);
		MoveCubeFaceLeft();
	}
	// 右へ
	if(_2Dposition.x + PLAYER_SIZE_HALF > CUBE_SIZE * 3)
	{
		if(_existFace == eTop)
		{
			_2Dposition.x = _2Dposition.y;
			_2Dposition.y = CUBE_SIZE * 3 - PLAYER_SIZE_HALF - 0.02f;
			ChangeUpDirection(eChange90);
		}else if(_existFace == eBottom)
		{
			_2Dposition.x = CUBE_SIZE * 3 - _2Dposition.y;
			_2Dposition.y = PLAYER_SIZE_HALF + 0.02f;
			ChangeUpDirection(eChange270);
		}else{
			_2Dposition.x = PLAYER_SIZE_HALF;
		}
		//_pMatrixChanger->ChangeMatrix(eRotationRight);
		MoveCubeFaceRight();
	}
	// 下へ
	if(_2Dposition.y - PLAYER_SIZE_HALF < 0.0f)
	{
		if(_existFace == eRight)
		{
			_2Dposition.y = CUBE_SIZE * 3 - _2Dposition.x;
			_2Dposition.x = CUBE_SIZE * 3 - PLAYER_SIZE_HALF - 0.02f;
			ChangeUpDirection(eChange90);
		}else if(_existFace == eLeft)
		{
			_2Dposition.y = _2Dposition.x;
			_2Dposition.x = PLAYER_SIZE_HALF;
			ChangeUpDirection(eChange270);
		}else if(_existFace == eBottom)
		{
			_2Dposition.x = CUBE_SIZE * 3 - _2Dposition.x;
			_2Dposition.y = PLAYER_SIZE_HALF + 0.02f;
			ChangeUpDirection(eChange180);
		}else if(_existFace == eBack)
		{
			_2Dposition.x = CUBE_SIZE * 3 - _2Dposition.x;
			_2Dposition.y = PLAYER_SIZE_HALF + 0.02f;
			ChangeUpDirection(eChange180);
		}else{
			_2Dposition.y = CUBE_SIZE * 3 - PLAYER_SIZE_HALF - 0.02f;
		}
		_jumpPower = 0.0f;
		_jumpFlag = false;
		//_pMatrixChanger->ChangeMatrix(eRotationDown);
		MoveCubeFaceDown();
	}
	// 上へ
	if(_2Dposition.y + PLAYER_SIZE_HALF  > CUBE_SIZE * 3)
	{
		if(_existFace == eRight)
		{
			_2Dposition.y = _2Dposition.x;
			_2Dposition.x = CUBE_SIZE * 3 - PLAYER_SIZE_HALF - 0.02f;
			ChangeUpDirection(eChange270);
		}else if(_existFace == eLeft)
		{
			_2Dposition.y = CUBE_SIZE * 3 - _2Dposition.x;
			_2Dposition.x = PLAYER_SIZE_HALF;
			ChangeUpDirection(eChange90);
		}else if(_existFace == eTop)
		{
			_2Dposition.y = CUBE_SIZE * 3 - PLAYER_SIZE_HALF - 0.02f;
			_2Dposition.x = CUBE_SIZE * 3 - _2Dposition.x;
			ChangeUpDirection(eChange180);
		}else if(_existFace == eBack)
		{
			_2Dposition.y = CUBE_SIZE * 3 - PLAYER_SIZE_HALF - 0.02f;
			_2Dposition.x = CUBE_SIZE * 3 - _2Dposition.x;
			ChangeUpDirection(eChange180);
		}else{
			_2Dposition.y = PLAYER_SIZE_HALF;
		}
		_jumpPower = 0.0f;
		_jumpFlag = false;
		//_pMatrixChanger->ChangeMatrix(eRotationUp);
		MoveCubeFaceUp();
	}

#pragma endregion

	// 3D描画位置の指定
	switch(_existFace)
	{
	case eTop:
		_rotaionMatrix = sTopMatrix;
		break;

	case eBottom:
		_rotaionMatrix = sBottomMatrix;
		break;

	case eRight:
		_rotaionMatrix = sRightMatrix;
		break;

	case eLeft:
		_rotaionMatrix = sLeftMatrix;
		break;

	case eFront:
		_rotaionMatrix = sFrontMatrix;
		break;

	case eBack:
		_rotaionMatrix = sBackMatrix;
		break;

	default:
		break;
	}

	// 実際のキューブ上でのプレイヤーの位置を更新する
	if(_existFace == eTop)
	{
		_cubeExist.y = 2;
		_cubeExist.x = _2Dposition.x / CUBE_SIZE;
		_cubeExist.z = 2 - _2Dposition.y / CUBE_SIZE;
	}
	if(_existFace == eBottom)
	{
		_cubeExist.y = 0;
		_cubeExist.x = _2Dposition.x / CUBE_SIZE;
		_cubeExist.z = _2Dposition.y / CUBE_SIZE;
	}
	if(_existFace == eRight)
	{
		_cubeExist.x = 2;
		_cubeExist.y = _2Dposition.y / CUBE_SIZE;
		_cubeExist.z = 2 - _2Dposition.x / CUBE_SIZE;
	}
	if(_existFace == eLeft)
	{
		_cubeExist.x = 0;
		_cubeExist.y = _2Dposition.y / CUBE_SIZE;
		_cubeExist.z = _2Dposition.x / CUBE_SIZE;
	}
	if(_existFace == eFront)
	{
		_cubeExist.z = 2;
		_cubeExist.y = _2Dposition.y / CUBE_SIZE;
		_cubeExist.x = _2Dposition.x / CUBE_SIZE;
	}
	if(_existFace == eBack)
	{
		_cubeExist.z = 0;
		_cubeExist.y = _2Dposition.y / CUBE_SIZE;
		_cubeExist.x = 2 - _2Dposition.x / CUBE_SIZE;
	}

	// 強引改変
	//_rotaionMatrix = sFrontMatrix;

	return false;
}

//******************************************************************
// 描画処理
//******************************************************************
void Player::Render()
{
	Matrix l_frontFaceMatrix;

	// プレイヤーの存在する面によって、渡すマトリクスを替える
	// (プレイヤーが常に正面に映るようにするため)
	switch( _existFace )
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

	// 作成した行列をエフェクトに設定
	Matrix position = Matrix::CreateFromAxisAngle(Vector3(1.0f, 0.0f, 0.0f), XMConvertToRadians(90.0f))
		* Matrix::CreateTranslation(_2Dposition.x - CUBE_SIZE_HALF * 3, _2Dposition.y - CUBE_SIZE_HALF * 3, CUBE_SIZE_HALF * 3 + 0.1f)
		* _rotaionMatrix
		* l_frontFaceMatrix;

	g_BatchEffect->SetWorld( position );
	g_BatchEffect->SetView( g_Camera->GetView() );
	g_BatchEffect->SetProjection( g_Camera->GetProjection() );

	g_BatchEffect->SetVertexColorEnabled(FALSE);

	// テクスチャの設定
	g_BatchEffect->SetTexture( _pTexture );

	// エフェクトの設定（各行列やテクスチャなどを設定している）
	g_BatchEffect->Apply( g_pImmediateContext );


	// 深度ステンシル ステートを設定する
	g_pImmediateContext->OMSetDepthStencilState( g_States->DepthDefault(), 0 );

	// ブレンディング ステートを設定する
	g_pImmediateContext->OMSetBlendState( g_pBlendState, nullptr, 0xFFFFFFFF ); 

	// サンプラーステートを設定する
	auto samplerState = g_States->PointWrap();
	g_pImmediateContext->PSSetSamplers(0, 1, &samplerState);

	// 入力レイアウトを設定する
	g_pImmediateContext->IASetInputLayout( g_pBatchInputLayout );

	g_Batch->Begin();

	// 頂点データ
	uint16_t indexes[4] = { 0,  1,  2,  3 };

	// 三角形プリミティブの描画
	g_Batch->DrawIndexed(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, indexes, 4, playerVertexes, 4);

	g_Batch->End();
	
}

#pragma region 移動に関する処理

//******************************************************************
// 右へ移動
//******************************************************************
void Player::MoveRight()
{
	switch(_upDirection)
	{
	case eUpDirect:
		_2DMoveVector.x += PLAYER_SPEED;
		break;

	case eRightDirect:
		_2DMoveVector.y -= PLAYER_SPEED;
		break;

	case eDownDirect:
		_2DMoveVector.x -= PLAYER_SPEED;
		break;

	case eLeftDirect:
		_2DMoveVector.y += PLAYER_SPEED;
		break;
	}
}

//******************************************************************
// 左へ移動
//******************************************************************
void Player::MoveLeft()
{
	switch(_upDirection)
	{
	case eUpDirect:
		_2DMoveVector.x -= PLAYER_SPEED;
		break;

	case eRightDirect:
		_2DMoveVector.y += PLAYER_SPEED;
		break;

	case eDownDirect:
		_2DMoveVector.x += PLAYER_SPEED;
		break;

	case eLeftDirect:
		_2DMoveVector.y -= PLAYER_SPEED;
		break;
	}
}

//******************************************************************
// 上へ移動
//******************************************************************
void Player::MoveUp()
{
	switch(_upDirection)
	{
	case eUpDirect:
		_2DMoveVector.y += PLAYER_SPEED;
		break;

	case eRightDirect:
		_2DMoveVector.x += PLAYER_SPEED;
		break;

	case eDownDirect:
		_2DMoveVector.y -= PLAYER_SPEED;
		break;

	case eLeftDirect:
		_2DMoveVector.x -= PLAYER_SPEED;
		break;
	}
}


//******************************************************************
// 下へ移動
//******************************************************************
void Player::MoveDown()
{
	switch(_upDirection)
	{
	case eUpDirect:
		_2DMoveVector.y -= PLAYER_SPEED;
		break;

	case eRightDirect:
		_2DMoveVector.x -= PLAYER_SPEED;
		break;

	case eDownDirect:
		_2DMoveVector.y += PLAYER_SPEED;
		break;

	case eLeftDirect:
		_2DMoveVector.x += PLAYER_SPEED;
		break;
	}
}


//******************************************************************
// ジャンプ処理
//******************************************************************
void Player::Jump()
{
	if(_jumpFlag == false)
	{
		_jumpFlag = true;
		_jumpPower = JUMP_POWER;
	}
}

#pragma endregion

#pragma region 当たり判定に関する処理

// マップを考慮して移動させる
bool Player::Move(float* x, float* y, float* jump_power, float move_x, float move_y, bool* jump_flag, int map[][15])
{
	 float Dummy = 0.0f ;
	 int returnMapHitCheck;

    // キャラクタの左上、右上、左下、右下部分が当たり判定のある
    // マップに衝突しているか調べ、衝突していたら補正する

    // 先ず上下移動成分だけでチェック
    {
        // 左下のチェック、もしブロックの上辺に着いていたら落下を止める
		returnMapHitCheck = MapHitCheck( *x - PLAYER_SIZE_HALF, *y + PLAYER_SIZE_HALF, &Dummy, &move_y, map);
		if( returnMapHitCheck == 3 && (_upDirection == eUpDirect || _upDirection == eDownDirect) )
			_jumpPower = 0.0F;
		if( returnMapHitCheck == GOAL ) return true;

        // 右下のチェック、もしブロックの上辺に着いていたら落下を止める
		returnMapHitCheck = MapHitCheck( *x + PLAYER_SIZE_HALF, *y + PLAYER_SIZE_HALF, &Dummy, &move_y, map );
        if( returnMapHitCheck == 3 && (_upDirection == eUpDirect || _upDirection == eDownDirect) ) 
			_jumpPower = 0.0F;
		if( returnMapHitCheck == GOAL ) return true;

        // 左上のチェック、もしブロックの下辺に当たっていたら落下させる
		returnMapHitCheck = MapHitCheck( *x - PLAYER_SIZE_HALF, *y - PLAYER_SIZE_HALF, &Dummy, &move_y, map);
        if( returnMapHitCheck == 4 && (_upDirection == eUpDirect || _upDirection == eDownDirect) ) 
			_jumpPower = 0.0F;
		if( returnMapHitCheck == GOAL ) return true;
		
        // 右上のチェック、もしブロックの下辺に当たっていたら落下させる
		returnMapHitCheck = MapHitCheck( *x + PLAYER_SIZE_HALF, *y - PLAYER_SIZE_HALF, &Dummy, &move_y, map);
        if( returnMapHitCheck == 4 && (_upDirection == eUpDirect || _upDirection == eDownDirect) ) 
			_jumpPower = 0.0F;
		if( returnMapHitCheck == GOAL ) return true;

        // 上下移動成分を加算
        *y += move_y ;
    }

    // 後に左右移動成分だけでチェック
    {
        // 左下のチェック
        returnMapHitCheck = MapHitCheck( *x - PLAYER_SIZE_HALF, *y + PLAYER_SIZE_HALF, &move_x, &Dummy, map );
		if( returnMapHitCheck > 0 && (_upDirection == eRightDirect || _upDirection == eLeftDirect) ) 
			_jumpPower = 0.0F;
		if( returnMapHitCheck == GOAL ) return true;

        // 右上のチェック
         returnMapHitCheck = MapHitCheck( *x + PLAYER_SIZE_HALF, *y + PLAYER_SIZE_HALF, &move_x, &Dummy, map );
		if( returnMapHitCheck > 0 && (_upDirection == eRightDirect || _upDirection == eLeftDirect) ) 
			_jumpPower = 0.0F;
		if( returnMapHitCheck == GOAL ) return true;

        // 左上のチェック
         returnMapHitCheck = MapHitCheck( *x - PLAYER_SIZE_HALF, *y - PLAYER_SIZE_HALF, &move_x, &Dummy, map );
		if( returnMapHitCheck > 0 && (_upDirection == eRightDirect || _upDirection == eLeftDirect) ) 
			_jumpPower = 0.0F;
		if( returnMapHitCheck == GOAL ) return true;

        // 右下のチェック
         returnMapHitCheck = MapHitCheck( *x + PLAYER_SIZE_HALF, *y - PLAYER_SIZE_HALF, &move_x, &Dummy, map );
		if( returnMapHitCheck > 0 && (_upDirection == eRightDirect || _upDirection == eLeftDirect) ) 
			_jumpPower = 0.0F;
		if( returnMapHitCheck == GOAL ) return true;

        // 左右移動成分を加算
        *x += move_x ;
    }

#pragma region  接地判定
   
	// プレイヤーの上方向によって、地面の判定を替える
    // キャラクタの左下と右下の下に地面があるか調べる
	if(_upDirection == eUpDirect)
	{
		if( GetChipParam( *x - PLAYER_SIZE_HALF, *y - PLAYER_SIZE_HALF - 0.3F , map) == 0 && 
			GetChipParam( *x + PLAYER_SIZE_HALF, *y - PLAYER_SIZE_HALF - 0.3F , map) == 0 )
			// 足場が無かったらジャンプ中にする
			_jumpFlag = true ;
		else
			// 足場が在ったら接地中にする
			_jumpFlag = false ;
	}
	if(_upDirection == eDownDirect)
	{
		if( GetChipParam( *x - PLAYER_SIZE_HALF, *y + PLAYER_SIZE_HALF + 0.3F , map) == 0 && 
			GetChipParam( *x + PLAYER_SIZE_HALF, *y + PLAYER_SIZE_HALF + 0.3F , map) == 0 )
			// 足場が無かったらジャンプ中にする
			_jumpFlag = true ;
		else
			// 足場が在ったら接地中にする
			_jumpFlag = false ;
	}
	if(_upDirection == eRightDirect)
	{
		if( GetChipParam( *x - PLAYER_SIZE_HALF - 0.3f, *y - PLAYER_SIZE_HALF, map) == 0 && 
			GetChipParam( *x - PLAYER_SIZE_HALF - 0.3f, *y + PLAYER_SIZE_HALF, map) == 0 )
			// 足場が無かったらジャンプ中にする
			_jumpFlag = true ;
		else
			// 足場が在ったら接地中にする
			_jumpFlag = false ;
	}
	if(_upDirection == eLeftDirect)
	{
		if( GetChipParam( *x + PLAYER_SIZE_HALF + 0.3f, *y - PLAYER_SIZE_HALF , map) == 0 && 
			GetChipParam( *x + PLAYER_SIZE_HALF + 0.3f, *y + PLAYER_SIZE_HALF , map) == 0 )
			// 足場が無かったらジャンプ中にする
			_jumpFlag = true ;
		else
			// 足場が在ったら接地中にする
			_jumpFlag = false ;
	}
#pragma endregion

    // 終了
    return false;
}

// マップとの当たり判定
int Player::MapHitCheck(float x, float y, float* move_x, float* move_y, int map[][15])
{
	float afX, afY ;

    // 移動量を足す
    afX = x + *move_x ;
    afY = y + *move_y ;

    // 当たり判定のあるブロックに当たっているかチェック
    if( GetChipParam( afX, afY, map ) )
    {
        float blx, bty, brx, bby ;

		// ゴールだったら
		if( GetChipParam( afX, afY, map ) == GOAL)
		{
			return GOAL;
		}

        // 当たっていたら壁から離す処理を行う

        // ブロックの上下左右の座標を算出
		blx = (float)( (int)afX / BLOCK_SIZE ) * BLOCK_SIZE ;        // 左辺の X 座標
        brx = (float)( (int)afX / BLOCK_SIZE + 1 ) * BLOCK_SIZE ;    // 右辺の X 座標

        bty = (float)( (int)afY / BLOCK_SIZE ) * BLOCK_SIZE ;        // 上辺の Y 座標
        bby = (float)( (int)afY / BLOCK_SIZE + 1 ) * BLOCK_SIZE ;    // 下辺の Y 座標

        // 上辺に当たっていた場合
        if( *move_y > 0.0F )
        {
            // 移動量を補正する
            *move_y = bty - y - 0.03F;

            // 上辺に当たったと返す
            return 3 ;
        }

        // 下辺に当たっていた場合
        if( *move_y < 0.0F )
        {
            // 移動量を補正する
            *move_y = bby - y + 0.03F ;

            // 下辺に当たったと返す
            return 4 ;
        }

        // 左辺に当たっていた場合
        if( *move_x > 0.0F )
        {
            // 移動量を補正する
            *move_x = blx - x - 0.03F ;

            // 左辺に当たったと返す
            return 1 ;
        }

        // 右辺に当たっていた場合
        if( *move_x < 0.0F )
        {
            // 移動量を補正する
            *move_x = brx - x + 0.03F ;

            // 右辺に当たったと返す
            return 2 ;
        }

        // ここに来たら適当な値を返す
        return 4 ;
    }

    // どこにも当たらなかったと返す
    return 0 ;
}

// 指定した座標に存在するマップチップの値を返す
int Player::GetChipParam(float x, float y, int map[][15])
{
	int X = (int)x / BLOCK_SIZE;
	int Y = (int)y / BLOCK_SIZE;

	if(X < 0) X = 0;
	if(X > 14) X = 14;
	if(Y < 0) Y = 0;
	if(Y > 14) Y = 14;

	return map[14 - Y][X];
}
#pragma endregion

//****************************************************************
// 隣接面への移動処理
//****************************************************************
void Player::MoveCubeFaceUp()
{
	switch(_existFace)
	{
	case eTop:
		_existFace = eBack;
		break;

	case eBottom:
		_existFace = eFront;
		break;

	case eRight:
	case eLeft:
	case eFront:
	case eBack:
		_existFace = eTop;
		break;

	default:
		break;
	}
}

void Player::MoveCubeFaceDown()
{
	switch(_existFace)
	{
	case eTop:
		_existFace = eFront;
		break;

	case eBottom:
		_existFace = eBack;
		break;

	case eRight:
	case eLeft:
	case eFront:
	case eBack:
		_existFace = eBottom;
		break;

	default:
		break;
	}
}

void Player::MoveCubeFaceRight()
{
	switch(_existFace)
	{
	case eTop:
		_existFace = eRight;
		break;

	case eBottom:
		_existFace = eRight;
		break;

	case eRight:
		_existFace = eBack;
		break;

	case eLeft:
		_existFace = eFront;
		break;

	case eFront:
		_existFace = eRight;
		break;

	case eBack:
		_existFace = eLeft;
		break;

	default:
		break;
	}
}

void Player::MoveCubeFaceLeft()
{
	switch(_existFace)
	{
	case eTop:
		_existFace = eLeft;
		break;

	case eBottom:
		_existFace = eLeft;
		break;

	case eRight:
		_existFace = eFront;
		break;

	case eLeft:
		_existFace = eBack;
		break;

	case eFront:
		_existFace = eLeft;
		break;

	case eBack:
		_existFace = eRight;
		break;

	default:
		break;
	}
}

//****************************************************************
// プレイヤーの面に対する上方向の変更関数
//****************************************************************
void Player::ChangeUpDirection(const eUpDirection& direction)
{
	switch(direction)
	{
	case eChange90:
		_upDirection++;
		break;

	case eChange270:
		_upDirection--;
		break;

	case eChange180:
		_upDirection += 2;
		break;

	default:
		break;
	}

	if( _upDirection < 0 )
	{
		_upDirection += 4;
	}

	if( _upDirection > 3 )
	{
		_upDirection -= 4;
	}
}

//****************************************************************
// ステージの回転情報を受け取り、それに応じてプレイヤーを回転させる
//****************************************************************
void Player::PlayerRotationWithCube(const sStageRotationInfo& stageRotationInfo)
{
	float tmp;	// 一時保存用変数

	if(stageRotationInfo.axis == 'x')
	{
		if(stageRotationInfo.rotationNum == _cubeExist.x)
		{
			if(stageRotationInfo.rotationDirect == TURN_RIGHT)
			{
				switch(_existFace)
				{
				case eTop:
					_existFace = eBack;
					ChangeUpDirection(eChange180);
					_2Dposition.x = CUBE_SIZE * 3.0f - _2Dposition.x;
					_2Dposition.y = CUBE_SIZE * 3.0f - _2Dposition.y;
					break;
				case eFront:
					_existFace = eTop;
					break;
				case eBottom:
					_existFace = eFront;
					break;
				case eBack:
					_existFace = eBottom;
					ChangeUpDirection(eChange180);
					_2Dposition.x = CUBE_SIZE * 3.0f - _2Dposition.x;
					_2Dposition.y = CUBE_SIZE * 3.0f - _2Dposition.y;
					break;

				case eRight:
					tmp = _2Dposition.x;
					_2Dposition.x = _2Dposition.y;
					_2Dposition.y = CUBE_SIZE * 3.0f - tmp;
					break;
				case eLeft:
					tmp = _2Dposition.x;
					_2Dposition.x = CUBE_SIZE * 3.0f - _2Dposition.y;
					_2Dposition.y = tmp;
					break;

				default:
					break;
				}
			}
			if(stageRotationInfo.rotationDirect == TURN_LEFT)
			{
				switch(_existFace)
				{
				case eTop:
					_existFace = eFront;
					break;
				case eFront:
					_existFace = eBottom;
					break;
				case eBottom:
					_existFace = eBack;
					ChangeUpDirection(eChange180);
					_2Dposition.x = CUBE_SIZE * 3.0f - _2Dposition.x;
					_2Dposition.y = CUBE_SIZE * 3.0f - _2Dposition.y;
					break;
				case eBack:
					_existFace = eTop;
					ChangeUpDirection(eChange180);
					_2Dposition.x = CUBE_SIZE * 3.0f - _2Dposition.x;
					_2Dposition.y = CUBE_SIZE * 3.0f - _2Dposition.y;
					break;

				case eRight:
					tmp = _2Dposition.x;
					_2Dposition.x = CUBE_SIZE * 3.0f - _2Dposition.y;
					_2Dposition.y = tmp;
					break;
				case eLeft:
					tmp = _2Dposition.x;
					_2Dposition.x = _2Dposition.y;
					_2Dposition.y = CUBE_SIZE * 3.0f - tmp;
					break;
				default:
					break;
				}
			}
		}
	}
	if(stageRotationInfo.axis == 'y')
	{
		if(stageRotationInfo.rotationNum == _cubeExist.y)
		{
			if(stageRotationInfo.rotationDirect == TURN_RIGHT)
			{
				switch(_existFace)
				{
				case eFront:
					_existFace = eLeft;
					break;
				case eRight:
					_existFace = eFront;
					break;
				case eBack:
					_existFace = eRight;
					break;
				case eLeft:
					_existFace = eBack;
					break;

				case eTop:
					tmp = _2Dposition.x;
					_2Dposition.x = _2Dposition.y;
					_2Dposition.y = CUBE_SIZE * 3.0f - tmp;
					break;
				case eBottom:
					tmp = _2Dposition.x;
					_2Dposition.x = CUBE_SIZE * 3.0f - _2Dposition.y;
					_2Dposition.y = tmp;
					break;

				default:
					break;
				}
			}
			if(stageRotationInfo.rotationDirect == TURN_LEFT)
			{
				switch(_existFace)
				{
				case eFront:
					_existFace = eRight;
					break;
				case eRight:
					_existFace = eBack;
					break;
				case eBack:
					_existFace = eLeft;
					break;
				case eLeft:
					_existFace = eFront;
					break;

				case eTop:
					tmp = _2Dposition.x;
					_2Dposition.x = CUBE_SIZE * 3.0f - _2Dposition.y;
					_2Dposition.y = tmp;
					break;
				case eBottom:
					tmp = _2Dposition.x;
					_2Dposition.x = _2Dposition.y;
					_2Dposition.y = CUBE_SIZE * 3.0f - tmp;
					break;
				default:
					break;
				}
			}
		}
	}
	if(stageRotationInfo.axis == 'z')
	{
		if(stageRotationInfo.rotationNum == _cubeExist.z)
		{
			if(stageRotationInfo.rotationDirect == TURN_RIGHT)
			{
				switch(_existFace)
				{
				case eTop:
					_existFace = eRight;
					ChangeUpDirection(eChange90);
					break;
				case eRight:
					_existFace = eBottom;
					ChangeUpDirection(eChange90);
					break;
				case eBottom:
					_existFace = eLeft;
					ChangeUpDirection(eChange90);
					break;
				case eLeft:
					_existFace = eTop;
					ChangeUpDirection(eChange90);
					break;
				default:
					break;
				}
				tmp = _2Dposition.x;
				_2Dposition.x = _2Dposition.y;
				_2Dposition.y = CUBE_SIZE * 3.0f - tmp;
			}
			if(stageRotationInfo.rotationDirect == TURN_LEFT)
			{
				switch(_existFace)
				{
				case eTop:
					_existFace = eLeft;
					ChangeUpDirection(eChange270);
					break;
				case eRight:
					_existFace = eTop;
					ChangeUpDirection(eChange270);
					break;
				case eBottom:
					_existFace = eRight;
					ChangeUpDirection(eChange270);
					break;
				case eLeft:
					_existFace = eBottom;
					ChangeUpDirection(eChange270);
					break;
				default:
					break;
				}
				tmp = _2Dposition.x;
				_2Dposition.x = CUBE_SIZE * 3.0f - _2Dposition.y;
				_2Dposition.y = tmp;
			}
		}
	}
}