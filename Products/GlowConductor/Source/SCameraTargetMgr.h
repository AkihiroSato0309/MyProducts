#pragma once
#include "Singleton.h"
#include "CameraTarget.h"
#define TargetPointer SCameraTargetMgr::Instance()->GetCameraTarget()

class SCameraTargetMgr : public Singleton<SCameraTargetMgr>
{
	friend Singleton;
public:
	SCameraTargetMgr(){ _pTarget = nullptr; }

	// ターゲットをセットする
	void SetCameraTarget(CameraTarget* pTarget){
		_pTarget = pTarget; 
	}

	// ターゲットを取得する
	CameraTarget* GetCameraTarget(){ return _pTarget; }

	// ターゲットを消去する
	void DeleteCameraTarget()
	{
		_pTarget->removeFromParentAndCleanup(true);
	}

private:
	CameraTarget* _pTarget;
};