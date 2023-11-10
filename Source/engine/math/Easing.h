#pragma once
#include "Vector3.h"

typedef struct EasingInfo
{
	float totalTime;
	float time;
	float startPos;
	float endDistance;
	/*EasingInfo(float TotalTime, float Time, float StartPos, float EndDistance) 
	{ totalTime = TotalTime, time = Time, startPos = StartPos, endDistance = EndDistance; }*/

} EasingInfo;

//EasingInfo();

//EasingInfo Initialize(float totalTime,float time,float startPos,float endDistance);

float PlayEaseIn(EasingInfo easingInfo);
float PlayEaseIn(float time, float startPos, float endDistance, float totalTime);
float easeInCubic(float x);

float PlayEaseOutQuint(float time, float startPos, float endDistance, float totalTime);
float easeOutQuint(float x);

float PlayEaseOutBouce(float time, float startPos, float endDistance, float totalTime);
float easeOutBouce(float x);
