#pragma once
#include "Singleton.h"

static const int SCORE_SAVE_MAX = 5;

class SDataMgr : public Singleton<SDataMgr>
{
	friend Singleton;
public:
	void SetScore(const int& score){ _score = score; }
	int GetScore(){ return _score; }

private:
	SDataMgr(){}
	~SDataMgr(){}

	int _score;

};