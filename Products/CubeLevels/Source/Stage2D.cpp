#include "Stage2D.h"
#include <list>
#include "Utility.h"

using namespace std;

//****************************************************************************
//
//		�@�@�\					�@�@�\
//		�@�b�O�b				�@�b��b
//		�\�{�\�{�\�@�\�@		�\�{�\�{�\�@�\�@
//	�@�b�R�b�S�b�Q�b�T�b�@�@  �b���b�O�b�E�b��b
//		�\�{�\�{�\�@�\�@		�\�{�\�{�\�@�\�@
//		�@�b�P�b				�@�b���b
//		�@�@�\				 	�@�@�\
//						
//                       ��  y   x
//		�X�e�[�W�̔z��c[6][15][15]
//		���オ���_
//   
//****************************************************************************


//***************************************************************
// �R���X�g���N�^
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
// �f�X�g���N�^
//***************************************************************
Stage2D::~Stage2D()
{

}

//***************************************************************
// �X�e�[�W���\������
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
// �w��ʂ̃}�b�v�f�[�^��Ԃ�
//***************************************************************
int* Stage2D::GetMapData(int face)
{
	return &_stageInfo[face][0][0];
}

//***************************************************************
// ��]�ɂ��}�b�v�̕ϓ�����
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
					// �E�ʂ̔z��S�̂���]������
					ArrayRotaion(_stageInfo[eRight], TURN_RIGHT);
				}else if(num == 0)
				{
					// ���ʂ̔z��S�̂���]������
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
					// �E�ʂ̔z��S�̂���]������
					ArrayRotaion(_stageInfo[eRight], TURN_LEFT);
				}else if(num == 0)
				{
					// ���ʂ̔z��S�̂���]������
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
					// ��ʂ̔z��S�̂���]������
					ArrayRotaion(_stageInfo[eTop], TURN_RIGHT);
				}else if(num == 0)
				{
					// ���ʂ̔z��S�̂���]������
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
					// ��ʂ̔z��S�̂���]������
					ArrayRotaion(_stageInfo[eTop], TURN_LEFT);
				}else if(num == 0)
				{
					// ���ʂ̔z��S�̂���]������
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
					// ���ʂ̔z��S�̂���]������
					ArrayRotaion(_stageInfo[eFront], TURN_RIGHT);
				}else if(num == 0)
				{
					// �w�ʂ̔z��S�̂���]������
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
					// ���ʂ̔z��S�̂���]������
					ArrayRotaion(_stageInfo[eFront], TURN_LEFT);
				}else if(num == 0)
				{
					// �w�ʂ̔z��S�̂���]������
					ArrayRotaion(_stageInfo[eBack], TURN_RIGHT);
				}
 			}
		}
	}
}
