#pragma once
#include "Vector3.h"

typedef struct EasingInfo
{
	const float totalTime;
	float time;
	float startPos;
	float endDistance;
	
} EasingInfo;

float PlayEaseIn(EasingInfo easingInfo);
float PlayEaseIn(float time, float startPos, float endDistance, float totalTime);
float easeInCubic(float x);

float PlayEaseOutQuint(float time, float startPos, float endDistance, float totalTime);
float easeOutQuint(float x);

float PlayEaseOutBouce(float time, float startPos, float endDistance, float totalTime);
float easeOutBouce(float x);
