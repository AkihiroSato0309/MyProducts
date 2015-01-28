//******************************************************************
//
// 面のステージデータ
//
//******************************************************************
#pragma once

#include "BlockFace.h"

class Stage2D
{
public:
	// コンストラクタ
	Stage2D();
	// デストラクタ
	~Stage2D();

	// ステージを構成する
	void CreateStage(int* map);

	// ステージを切り替える
	void ChangeStage(int existFace);

	// マップを返す
	int* GetMapData(int face);

	// 回転時にマップを切り替える
	void RotationCubeX(int num, const int& turnDirection);
	void RotationCubeY(int num, const int& turnDirection);
	void RotationCubeZ(int num, const int& turnDirection);

private:
	// ステージ情報[面][縦][横]
	int _stageInfo[6][15][15];
};