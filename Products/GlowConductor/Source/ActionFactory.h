#pragma once

#include "cocos2d.h"

// アクションをNodeに付属させる
class ActionFactory
{
public:
	static const bool START_UP = true;
	static const bool START_DOWN = false;
	static const bool START_RIGHT = true;
	static const bool START_LEFT = false;
	static const bool START_SMALL = false;
	static const bool START_BIG = true;

	// 上下移動のアクション　							振幅　　　　　周期　　　　　スタート位置
	static void CreateMoveVartical(cocos2d::Node* node, float amplitude, float period, bool startPrace);

	// 左右移動のアクション　							振幅　　　　　周期　　　　　スタート位置
	static void CreateMoveHorizontal(cocos2d::Node* node, float amplitude, float period, bool startPrace);

	// 上からきて止まる
	static void CreateMoveToDown(	cocos2d::Node* node, 
									float distance, 
									float duration);

	// 下からきて止まる
	static void CreateMoveToUp(	cocos2d::Node* node, 
								float distance, 
								float duration);

	// 収縮を繰り返す
	static void CreateScaleChange(	cocos2d::Node* node, 
									float maxScale,
									float smallScale,
									float period, 
									bool startScale);
};