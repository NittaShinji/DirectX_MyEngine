#pragma once
#include "Vector3.h"

/// <summary>
/// イージング情報
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

float PlayEaseIn(EasingInfo easingInfo);
float PlayEaseIn(float startPos, float endDistance, float time, float totalTime);
float easeInCubic(float x);

float PlayEaseOutSine(EasingInfo easingInfo);
float EaseOutSine(float x);

float PlayEaseOutQuint(EasingInfo easingInfo);
float PlayEaseOutQuint(float startPos, float endDistance, float time, float totalTime);
float easeOutQuint(float x);

float PlayEaseOutCubic(EasingInfo easingInfo);
float EaseOutCubic(float x);

float PlayEaseOutBack(EasingInfo easingInfo);
float EaseOutBack(float x);

float PlayEaseOutBouce(float startPos, float endDistance, float time, float totalTime);
float easeOutBouce(float x);


