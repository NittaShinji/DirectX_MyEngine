#pragma once
#include "Vector3.h"

/// <summary>
/// イージング情報
/// 初期位置
/// 終点までの距離
/// アニメーションの現在の時間
/// アニメーションが行われる時間
/// </summary>
typedef struct EasingInfo
{
	//初期位置
	float startPos;
	//終点までの距離
	float endDistance;
	//アニメーションの現在の時間
	float time;
	//アニメーションが行われる時間
	const float totalTime;
	
} EasingInfo;


//イーズインキュービック
float EaseInCubic(float x);
//指定の範囲でイーズインキュービックを行う
float PlayEaseInCubic(EasingInfo easingInfo);
float PlayEaseInCubic(float startPos, float endDistance, float time, float totalTime);

//イーズアウトシャイン
float EaseOutSine(float x);
//指定の範囲でイーズアウトシャインを行う
float PlayEaseOutSine(EasingInfo easingInfo);

//イーズアウトクイント
float EaseOutQuint(float x);
//指定の範囲でイーズアウトクイントを行う
float PlayEaseOutQuint(EasingInfo easingInfo);
float PlayEaseOutQuint(float startPos, float endDistance, float time, float totalTime);

//イーズアウトキュービック
float EaseOutCubic(float x);
//指定の範囲でイーズアウトキュービックを行う
float PlayEaseOutCubic(EasingInfo easingInfo);

//イーズアウトバック
float EaseOutBack(float x);
//指定の範囲でイーズアウトバックを行う
float PlayEaseOutBack(EasingInfo easingInfo);

//イーズアウトバウンス
float EaseOutBouce(float x);
//指定の範囲でイーズアウトバウンスを行う
float PlayEaseOutBouce(float startPos, float endDistance, float time, float totalTime);


