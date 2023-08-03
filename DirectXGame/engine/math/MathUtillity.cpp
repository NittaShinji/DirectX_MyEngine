#include "MathUtillity.h"

float MathUtillty::ToDegree(float radian)
{
	return radian * 180;
}

float MathUtillty::ToRadian(float angle)
{
	return angle *  (PI/ 180);
}

Vector3 MathUtillty::Vector3Subtract(Vector3 v1, Vector3 v2)
{
	return Vector3(v1 - v2);
}

bool MathUtillty::Vector3Equal(Vector3 v1, Vector3 v2)
{
	return (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z);
}

Vector3 MathUtillty::Vector3Zero()
{
	return Vector3(0.0f,0.0f,0.0f);
}

float MathUtillty::Vector3Dot(Vector3 v1, Vector3 v2)
{
	return  (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

Vector3 MathUtillty::Vector3Cross(Vector3 v1, Vector3 v2)
{
	return Vector3((v1.y * v2.z) - (v1.z * v2.y), (v1.z * v2.x) - (v1.x * v2.z), (v1.x * v2.y) - (v1.y * v2.x));
}

\
