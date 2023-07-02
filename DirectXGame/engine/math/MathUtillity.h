#pragma once

namespace MathUtillty
{
	//円周率
	const float PI = 3.141592654f;

	//重力
	const float gravity = 0.098f;

	//度数法変換(ラジアンから度数法)
	float ToDegree(float radian);

	//弧度法変換(度数法からラジアン)
	float ToRadian(float angle);
}