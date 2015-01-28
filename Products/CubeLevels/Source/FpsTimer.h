#pragma once

#include <Windows.h>

class Timer
{
	// リセットしてからの時間
	LARGE_INTEGER m_OffsetTime;

	// CPU周波数（１秒あたりのカウント数）
	LARGE_INTEGER m_CpuFrequency;

public:
	// コンストラクタ
	Timer();

	// デストラクタ
	virtual ~Timer() {}

	// タイマーのリセット
	void Reset();

	// タイマーの取得
	DWORD Get();
};

// FPSタイマーを制御するクラス
class FpsTimer : public Timer
{
	// FPS
	DWORD m_fps;

	// １フレーム辺りの待ち時間(ms)×0x1000
	DWORD m_fpsWait;

	// １フレーム辺りの待ち時間の端数部分を保持するワーク
	DWORD m_fpsWaitTT;

	// 前回描画した時間
	DWORD m_lastDraw;

	// true:フレームスキップが必要
	bool m_drawSkipFlag;

	// 描画した回数
	DWORD m_drawCnt;

	// 描画した時間の履歴保存用ワーク
	DWORD m_drawTime[32];

public:
	// コンストラクタ
	FpsTimer();

	// FPSの設定
	DWORD SetFps(DWORD fps);

	// 設定されているＦＰＳの取得
	DWORD GetFps()
	{
		return m_fps;
	}

	// ウエイト
	void Wait();

	// 描画をスキップすべきかチェックする関数
	bool CheckDrawSkip()
	{
		return m_drawSkipFlag;
	}

	// 実際のＦＰＳの算出
	DWORD GetRealFps();

};

