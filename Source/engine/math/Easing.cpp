#include "Easing.h"
#include "MathUtillity.h"
#include <cmath>

using namespace MathUtillty;

float PlayEaseInCubic(EasingInfo easingInfo)
{
	float x = easingInfo.time / easingInfo.totalTime;
	float v = EaseInCubic(x);
	float ret = easingInfo.endDistance * v + easingInfo.startPos;

	return ret;
}

float PlayEaseInCubic(float startPos, float endDistance, float time, float totalTime)
{
	float x = time / totalTime;
	float v = EaseInCubic(x);
	float ret = endDistance * v + startPos;

	return ret;
}

float EaseInCubic(float x)
{
	return x * x * x;
}

float PlayEaseOutSine(EasingInfo easingInfo)
{
	float x = easingInfo.time / easingInfo.totalTime;
	float v = EaseOutSine(x);
	float ret = easingInfo.endDistance * v + easingInfo.startPos;

	return ret;
}

float EaseOutSine(float x)
{
	return  sin((x * MathUtillty::PI) / 2);
}

float PlayEaseOutQuint(EasingInfo easingInfo)
{
	float x = easingInfo.time / easingInfo.totalTime;
	float v = EaseOutQuint(x);
	float ret = easingInfo.endDistance * v + easingInfo.startPos;

	return ret;
}

float PlayEaseOutQuint( float startPos, float endDistance, float time, float totalTime)
{
	float x = time / totalTime;
	float v = EaseOutQuint(x);
	float ret = endDistance * v + startPos;

	return ret;
}

float EaseOutQuint(float x)
{
	return static_cast <float> (1 - pow(1 - x, 5));
}

float PlayEaseOutCubic(EasingInfo easingInfo)
{
	float x = easingInfo.time / easingInfo.totalTime;
	float v = EaseOutCubic(x);
	float ret = easingInfo.endDistance * v + easingInfo.startPos;

	return ret;
}

float EaseOutCubic(float x)
{
	return static_cast<float>(1 - pow(1 - x, 3));
}

float PlayEaseOutBack(EasingInfo easingInfo)
{
	float x = easingInfo.time / easingInfo.totalTime;
	float v = EaseOutBack(x);
	float ret = easingInfo.endDistance * v + easingInfo.startPos;

	return ret;
}

float EaseOutBack(float x)
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1;

	return static_cast <float>(1 + c3 * pow(x - 1, 3) + c1 * pow(x - 1, 2));
}

float PlayEaseOutBouce(float startPos, float endDistance, float time, float totalTime)
{
	float x = time / totalTime;
	float v = static_cast<float>(EaseOutBouce(x));
	float ret = endDistance * v + startPos;

	return ret;
}

float EaseInOutSine(float x)
{
	return -(cos(PI * x) - 1) / 2;
}

float PlayEaseInOutSine(EasingInfo easingInfo)
{
	float x = easingInfo.time / easingInfo.totalTime;
	float v = EaseInOutSine(x);
	float ret = easingInfo.endDistance * v + easingInfo.startPos;

	return ret;
}

float EaseOutBouce(float x)
{
	const float n1 = 7.5625;
	const float d1 = 2.75;

	if(x < 1 / d1)
	{
		return (n1 * x * x);
	}
	else if(x < 2 / d1)
	{
		return (n1 * (x -= float(1.5) / d1) * x + float(0.75));
	}
	else if(x < 2.5 / d1)
	{
		return (n1 * (x -= float(2.25) / d1) * x + float(0.9375));
	}
	else
	{
		return (n1 * (x -= float(2.625) / d1) * x + float(0.984375));
	}
}
