#pragma once

//***************************************************************************************
// 障害物オブジェクトから自機への操作を制限するためのインターフェースクラス
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