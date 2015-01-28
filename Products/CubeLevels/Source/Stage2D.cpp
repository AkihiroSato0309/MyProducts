#include "Stage2D.h"
#include <list>
#include "Utility.h"

using namespace std;

//****************************************************************************
//
//		　　―					　　―
//		　｜０｜				　｜上｜
//		―＋―＋―　―　		―＋―＋―　―　
//	　｜３｜４｜２｜５｜　　  ｜左｜前｜右｜後｜
//		―＋―＋―　―　		―＋―＋―　―　
//		　｜１｜				　｜下｜
//		　　―				 	　　―
//						
//                       面  y   x
//		ステージの配列…[6][15][15]
//		左上が原点
//   
//****************************************************************************


//***************************************************************
// コンストラクタ
//***************************************************************
Stage2D::Stage2D()
{
	for(int i = 0; i < 6; i++)
	{
		for(int j = 0; j < 15; j++)
		{
			for(int k = 0; k < 15; k++)
			{
				_stageInfo[i][j][k] = false;
			}
		}
	}
}

//***************************************************************
// デストラクタ
//***************************************************************
Stage2D::~Stage2D()
{

}

//***************************************************************
// ステージを構成する
//***************************************************************
void Stage2D::CreateStage(int* map)
{
	for(int i = 0; i < 6; i++)
	{
		for(int j = 0; j < 15; j++)
		{
			for(int k = 0; k < 15; k++)
			{
				_stageInfo[i][j][k] = map[i * 225 + j * 15 + k];
			}
		}
	}
}

//***************************************************************
// 指定面のマップデータを返す
//***************************************************************
int* Stage2D::GetMapData(int face)
{
	return &_stageInfo[face][0][0];
}

//***************************************************************
// 回転によるマップの変動処理
//***************************************************************
void Stage2D::RotationCubeX(int num, const int& turnDirection)
{
	list<int> tmp;
	list<int>::iterator itr;

	if(turnDirection == TURN_RIGHT)
	{
		for(int i = 0; i < 5; i++)
		{
			for(int j = 0; j < 15; j++)
			{
				tmp.push_back(_stageInfo[eFront][j][num * 5 + i]);
				_stageInfo[eFront][j][num * 5 + i] = _stageInfo[eBottom][j][num * 5 + i];
				tmp.push_back(_stageInfo[eTop][j][num * 5 + i]);
				itr = tmp.begin();
				_stageInfo[eTop][j][num * 5 + i] = *itr;
				tmp.pop_front();
				tmp.push_back(_stageInfo[eBack][14 - j][14 - (num * 5 + i)]);
				itr = tmp.begin();
				_stageInfo[eBack][14 - j][14 - (num * 5 + i)] = *itr;
				tmp.pop_front();
				itr = tmp.begin();
				_stageInfo[eBottom][j][num * 5 + i] = *itr;
				tmp.pop_front();

				if(num == 2)
				{
					// 右面の配列全体を回転させる
					ArrayRotaion(_stageInfo[eRight], TURN_RIGHT);
				}else if(num == 0)
				{
					// 左面の配列全体を回転させる
					ArrayRotaion(_stageInfo[eLeft], TURN_LEFT);
				}
 			}
		}
	}else{
		for(int i = 0; i < 5; i++)
		{
			for(int j = 0; j < 15; j++)
			{
				tmp.push_back(_stageInfo[eFront][j][num * 5 + i]);
				_stageInfo[eFront][j][num * 5 + i] = _stageInfo[eTop][j][num * 5 + i];
				tmp.push_back(_stageInfo[eBottom][j][num * 5 + i]);
				itr = tmp.begin();
				_stageInfo[eBottom][j][num * 5 + i] = *itr;
				tmp.pop_front();
				tmp.push_back(_stageInfo[eBack][14 - j][14 - (num * 5 + i)]);
				itr = tmp.begin();
				_stageInfo[eBack][14 - j][14 - (num * 5 + i)] = *itr;
				tmp.pop_front();
				itr = tmp.begin();
				_stageInfo[eTop][j][num * 5 + i] = *itr;
				tmp.pop_front();

				if(num == 2)
				{
					// 右面の配列全体を回転させる
					ArrayRotaion(_stageInfo[eRight], TURN_LEFT);
				}else if(num == 0)
				{
					// 左面の配列全体を回転させる
					ArrayRotaion(_stageInfo[eLeft], TURN_RIGHT);
				}
 			}
		}
	}
}

void Stage2D::RotationCubeY(int num, const int& turnDirection)
{
	list<int> tmp;
	list<int>::iterator itr;

	if(turnDirection == TURN_RIGHT)
	{
		for(int i = 0; i < 5; i++)
		{
			for(int j = 0; j < 15; j++)
			{
				tmp.push_back(_stageInfo[eFront][(2 - num) * 5 + i][j]);
				_stageInfo[eFront][(2 - num) * 5 + i][j] = _stageInfo[eRight][(2 - num) * 5 + i][j];
				tmp.push_back(_stageInfo[eLeft][(2 - num) * 5 + i][j]);
				itr = tmp.begin();
				_stageInfo[eLeft][(2 - num) * 5 + i][j] = *itr;
				tmp.pop_front();
				tmp.push_back(_stageInfo[eBack][(2 - num) * 5 + i][j]);
				itr = tmp.begin();
				_stageInfo[eBack][(2 - num) * 5 + i][j] = *itr;
				tmp.pop_front();
				itr = tmp.begin();
				_stageInfo[eRight][(2 - num) * 5 + i][j] = *itr;
				tmp.pop_front();

				if(num == 2)
				{
					// 上面の配列全体を回転させる
					ArrayRotaion(_stageInfo[eTop], TURN_RIGHT);
				}else if(num == 0)
				{
					// 下面の配列全体を回転させる
					ArrayRotaion(_stageInfo[eBottom], TURN_LEFT);
				}
 			}
		}
	}else{
		for(int i = 0; i < 5; i++)
		{
			for(int j = 0; j < 15; j++)
			{
				tmp.push_back(_stageInfo[eFront][(2 - num) * 5 + i][j]);
				_stageInfo[eFront][(2 - num) * 5 + i][j] = _stageInfo[eLeft][(2 - num) * 5 + i][j];
				tmp.push_back(_stageInfo[eRight][(2 - num) * 5 + i][j]);
				itr = tmp.begin();
				_stageInfo[eRight][(2 - num) * 5 + i][j] = *itr;
				tmp.pop_front();
				tmp.push_back(_stageInfo[eBack][(2 - num) * 5 + i][j]);
				itr = tmp.begin();
				_stageInfo[eBack][(2 - num) * 5 + i][j] = *itr;
				tmp.pop_front();
				itr = tmp.begin();
				_stageInfo[eLeft][(2 - num) * 5 + i][j] = *itr;
				tmp.pop_front();

				if(num == 2)
				{
					// 上面の配列全体を回転させる
					ArrayRotaion(_stageInfo[eTop], TURN_LEFT);
				}else if(num == 0)
				{
					// 下面の配列全体を回転させる
					ArrayRotaion(_stageInfo[eBottom], TURN_RIGHT);
				}
 			}
		}
	}
}

void Stage2D::RotationCubeZ(int num, const int& turnDirection)
{
	list<int> tmp;
	list<int>::iterator itr;

	if(turnDirection == TURN_RIGHT)
	{
		for(int i = 0; i < 5; i++)
		{
			for(int j = 0; j < 15; j++)
			{
				tmp.push_back(_stageInfo[eTop][num * 5 + i][j]);
				_stageInfo[eTop][num * 5 + i][j] = _stageInfo[eLeft][14 - j][num * 5 + i];
				tmp.push_back(_stageInfo[eRight][j][14 - (num * 5 + i)]);
				itr = tmp.begin();
				_stageInfo[eRight][j][14 - (num * 5 + i)] = *itr;
				tmp.pop_front();
				tmp.push_back(_stageInfo[eBottom][14 - (num * 5 + i)][14 - j]);
				itr = tmp.begin();
				_stageInfo[eBottom][14 - (num * 5 + i)][14 - j] = *itr;
				tmp.pop_front();
				itr = tmp.begin();
				_stageInfo[eLeft][14 - j][num * 5 + i] = *itr;
				tmp.pop_front();

				if(num == 2)
				{
					// 正面の配列全体を回転させる
					ArrayRotaion(_stageInfo[eFront], TURN_RIGHT);
				}else if(num == 0)
				{
					// 背面の配列全体を回転させる
					ArrayRotaion(_stageInfo[eBack], TURN_LEFT);
				}
 			}
		}
	}else{
		for(int i = 0; i < 5; i++)
		{
			for(int j = 0; j < 15; j++)
			{
				tmp.push_back(_stageInfo[eTop][num * 5 + i][j]);
				_stageInfo[eTop][num * 5 + i][j] = _stageInfo[eRight][j][14 - (num * 5 + i)];
				tmp.push_back(_stageInfo[eLeft][14 - j][num * 5 + i]);
				itr = tmp.begin();
				_stageInfo[eLeft][14 - j][num * 5 + i] = *itr;
				tmp.pop_front();
				tmp.push_back(_stageInfo[eBottom][14 - (num * 5 + i)][14 - j]);
				itr = tmp.begin();
				_stageInfo[eBottom][14 - (num * 5 + i)][14 - j] = *itr;
				tmp.pop_front();
				itr = tmp.begin();
				_stageInfo[eRight][j][14 - (num * 5 + i)] = *itr;
				tmp.pop_front();

				if(num == 2)
				{
					// 正面の配列全体を回転させる
					ArrayRotaion(_stageInfo[eFront], TURN_LEFT);
				}else if(num == 0)
				{
					// 背面の配列全体を回転させる
					ArrayRotaion(_stageInfo[eBack], TURN_RIGHT);
				}
 			}
		}
	}
}
