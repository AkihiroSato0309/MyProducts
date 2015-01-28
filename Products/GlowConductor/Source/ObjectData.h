#pragma once

#include "Singleton.h"
#include <list>

class BaseAI;

// オブジェクトのデータベース（シングルトン）
class SObjectDatabase
{
public:
	static SObjectDatabase* Instance()
	{
		static SObjectDatabase obj;
		return &obj;
	}

private:
	SObjectDatabase(){}
	~SObjectDatabase(){}
	SObjectDatabase(const SObjectDatabase& data){}
	SObjectDatabase& operator=(const SObjectDatabase& data){}

	
};