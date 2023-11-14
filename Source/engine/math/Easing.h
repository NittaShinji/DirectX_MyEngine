#pragma once
#include "Vector3.h"

/// <summary>
/// イージング情報
/// </summary>
typedef struct EasingInfo
{
	float startPos;
	float endDistance;
	float time;
	const float totalTime;
	
} EasingInfo;

float PlayEaseIn(EasingInfo easingInfo);
float PlayEaseIn(float startPos, float endDistance, float time, float totalTime);
float easeInCubic(float x);

float PlayEaseOutQuint(EasingInfo easingInfo);
float PlayEaseOutQuint(float startPos, float endDistance, float time, float totalTime);
float easeOutQuint(float x);

float PlayEaseOutBouce(float startPos, float endDistance, float time, float totalTime);
float easeOutBouce(float x);
