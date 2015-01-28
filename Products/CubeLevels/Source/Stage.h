//***************************************************************************************************
//
// Stage.h					佐藤彰洋　12月16日
//
//***************************************************************************************************
#pragma once

#include "Cube.h"
#include "Stage2D.h"
#include "IStageMatrixChanger.h"
#include "SStageRotationInfo.h"

struct FaceRemderMgr
{
	int sFace;
	float sDistance;
	DirectX::SimpleMath::Vector3 sFaceCenterPosition;
};




static const int MOVE_FLAME = 10;

enum ePosition{X = 0, Y = 1, Z = 2};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ステージクラス
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Stage : public IStageMatrixChanger
{
public:
	Stage(int* map);
	~Stage();
	
	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Render(const int& playerExist);

	// キューブ回転処理
	bool RotationCube(const int& playerExistFace, const int& playerUpDirect );

	// 外部用キューブ回転処理(クラス外から回転命令を行う)
	void Rotation(const int& playerExistFace, const int& playerUpDirect);

	// 指定した面に属するキューブの面配列の先頭アドレスを返す
	CubeFace** GetAppointCubeFace(int appointDirection);

	// 2Dステージのポインタを返す
	Stage2D* GetStage2D(){return _pStage2D;}

	// マトリクスを操作する
	void ChangeMatrix(eRotationMatrix rotationDirect);

	// 回転中かどうかを取得する
	bool GetMovingFlag();

	// 回転情報フラグを取得し、フラグを倒す
	bool GetRotationInfoFlag();

	// 回転情報を渡す
	sStageRotationInfo& GetStageRotationInfo()
	{
		return _rotationInfo;
	}

private:
	// 回転中フラグ
	bool _moveFlag;

	// カウント変数
	int _time;

	// 総移動時間と今の時間の割合
	float _moveDegree;

	// ステージの面の中心点
	DirectX::SimpleMath::Vector3 _faceCenterPosition[6];

	// 全体の回転マトリクス
	DirectX::SimpleMath::Matrix _rotationMatrix;
	
	// 各頂点とカメラの距離
	float _distance[6];

	// 保持キューブ(原点から)
	Cube* _cube[3][3][3];

	// 保持する面情報(X, Yは左上から)
	CubeFace* _pCubeFace[6][3][3];

	// 指定した面のアドレスを持つ
	CubeFace* _appointCubeFace[9];

	// それぞれの面別のアドレス
	CubeFace* _pRenderFace[6][9];

	// 面の描画する順番を管理
	FaceRemderMgr _renderMgr[6];

	// 2Dステージデータ
	Stage2D* _pStage2D;

	// 回転角度情報
	float _Radian;

	// プレイヤーに送信する回転情報
	sStageRotationInfo _rotationInfo;


	// キューブ回転処理(指定軸を中心とした原点からの個数と、回転方向)
	void RotationCubeX(int num, const int& turn_direction);
	void RotationCubeY(int num, const int& turn_direction);
	void RotationCubeZ(int num, const int& turn_direction);

	// 8つの頂点の座標
	const DirectX::SimpleMath::Vector3 _vertex[8];

	// 指定面のキューブ配列を返す際に使用する総当り処理
	void AllAppoint(CubeFace** face, int appointDirection, int cubePosition, int searchPosition);

	// プレイヤーの存在面、上方向を考慮した縦回転を行う
	bool VerticalRotation(const int& playerExistFace, const int& playerUpDirect, const int& rotationNum, const int& rotationDirect);

	// 横回転を行う
	bool HorizontallyRotation(const int& playerExistFace, const int& playerUpDirect, const int& rotationNum, const int& rotationDirect);
	
	// 引数に入力された回転方向の逆を戻り値とする
	int ReverseRotationDirect(const int& rotationDirect);

	// 引数に入力されたキューブナンバーを反転させる
	// 例 : 0→2, 2→0
	int ReverseRotationNum(const int& rotationNum);
};