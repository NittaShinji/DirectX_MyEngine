#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4.h"

namespace MathUtillty
{
	//�~����
	const float PI = 3.141592654f;

	//�d��
	const float gravity = 0.098f;

	//�x���@�ϊ�(���W�A������x���@)
	float ToDegree(float radian);

	//�ʓx�@�ϊ�(�x���@���烉�W�A��)
	float ToRadian(float angle);
}