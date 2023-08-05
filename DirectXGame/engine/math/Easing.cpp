#include "Easing.h"

float PlayEaseIn(float time, float startPos, float endDistance, float totalTime)
{
	float x = time / totalTime;
	float v = easeInCubic(x);
	float ret = endDistance * v + startPos;

	return ret;
}

float easeInCubic(float x)
{
	return x * x * x;
}

float PlayEaseOutBouce(float time, float startPos, float endDistance, float totalTime)
{
	float x = time / totalTime;
	float v = static_cast<float>(easeOutBouce(x));
	float ret = endDistance * v + startPos;

	return ret;
}

float easeOutBouce(float x)
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
	return 0.0f;
}
