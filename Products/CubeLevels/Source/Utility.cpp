#include "Utility.h"
#include "BlockFace.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//***************************************************************
// ��]�ɂ��ϓ������̕⏕�֐�
//***************************************************************
// �z��S�̂�90�x��]������
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

	// ��]�����z������̔z��ɓ��꒼��
	for(int i = 0; i < 15; i++)
	{
		for(int j = 0; j < 15; j++)
		{
			val[i][j] = tmp[i][j];
		}
	}
}

// �z��S�̂�180�x��]������
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

	// ��]�����z������̔z��ɓ��꒼��
	for(int i = 0; i < 15; i++)
	{
		for(int j = 0; j < 15; j++)
		{
			val[i][j] = tmp[i][j];
		}
	}
}