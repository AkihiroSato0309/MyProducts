#include "Utility.h"
#include "BlockFace.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//***************************************************************
// 回転による変動処理の補助関数
//***************************************************************
// 配列全体を90度回転させる
void ArrayRotaion(int val[][15], int num)
{
	int tmp[15][15];
	if(num == TURN_RIGHT)
	{
		for(int i = 0; i < 15; i++)
		{
			for(int j = 0; j < 15; j++)
			{
				tmp[i][j] = val[j][14 - i];
			}
		}
	}else{
		for(int i = 0; i < 15; i++)
		{
			for(int j = 0; j < 15; j++)
			{
				tmp[i][j] = val[14 - j][i];
			}
		}
	}

	// 回転した配列を元の配列に入れ直す
	for(int i = 0; i < 15; i++)
	{
		for(int j = 0; j < 15; j++)
		{
			val[i][j] = tmp[i][j];
		}
	}
}

// 配列全体を180度回転させる
void ArrayRotation180(int val[][15])
{
	int tmp[15][15];

	for(int i = 0; i < 15; i++)
	{
		for(int j = 0; j < 15; j++)
		{
			tmp[i][j] = val[14 - i][14 - j];
		}
	}

	// 回転した配列を元の配列に入れ直す
	for(int i = 0; i < 15; i++)
	{
		for(int j = 0; j < 15; j++)
		{
			val[i][j] = tmp[i][j];
		}
	}
}