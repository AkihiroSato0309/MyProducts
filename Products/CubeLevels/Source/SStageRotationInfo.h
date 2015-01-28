#pragma once

// プレイヤー上方向に関する列挙型
enum eUpDirection
{
	eUpDirect = 0,
	eRightDirect,
	eDownDirect,
	eLeftDirect,

	//以下、上方向変更の際に使用(時計回り)
	eChange90,
	eChange270,
	eChange180
};

// int型のVector3
struct sIntVector3
{
	int x;
	int y;
	int z;
};

// プレイヤーに回転の有無、その方向を送信する構造体
struct sStageRotationInfo
{
	bool rotationFlag;	// ステージクラスの回転フラグとは用途が違う
	char axis;			// x,y,zのどれを軸とするか
	int rotationNum;	// 原点から何番目のキューブを回転させるか
	int rotationDirect;	// 右回転か左回転か
};