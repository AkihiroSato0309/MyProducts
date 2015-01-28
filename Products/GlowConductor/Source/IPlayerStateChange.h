#pragma once

//***************************************************************************************
// ��Q���I�u�W�F�N�g���玩�@�ւ̑���𐧌����邽�߂̃C���^�[�t�F�[�X�N���X
//***************************************************************************************
class IPlayerStateChange
{
public:
	virtual void ScoreUp(int score) = 0;
	virtual void ScoreDown(int score) = 0;
	virtual void AddTouch(int add) = 0;
	virtual void DownTouch(int down) = 0;
	virtual void KillPlayer() = 0;
};