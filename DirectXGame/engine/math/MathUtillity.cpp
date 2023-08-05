#include "MathUtillity.h"
#include <cmath>
#include <cassert>

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

Vector3 MathUtillty::Vector3Normalize(Vector3 v1)
{
	float len = v1.Length();
	if(len != 0)
	{
		return v1 /= len;
	}

	return v1;
}

Vector3 MathUtillty::Vector3Zero()
{
	return Vector3(0.0f,0.0f,0.0f);
}

Vector3 MathUtillty::Vector3Dot(Vector3 v1, Vector3 v2)
{
	float dot = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
	return  Vector3{ dot,dot,dot };
}

Vector3 MathUtillty::Vector3Cross(Vector3 v1, Vector3 v2)
{
	return Vector3((v1.y * v2.z) - (v1.z * v2.y), (v1.z * v2.x) - (v1.x * v2.z), (v1.x * v2.y) - (v1.y * v2.x));
}

Vector3 MathUtillty::Vector3Transform(const Vector3& v, const Matrix4& m)
{
	Vector3 result{
	  v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0],
	  v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1],
	  v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2] };

	return result;
}

Vector3 MathUtillty::Vector3TransformCoord(const Vector3& v, const Matrix4& m)
{
	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

	Vector3 result{
	  (v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0]) / w,
	  (v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1]) / w,
	  (v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2]) / w };

	return result;
}

Vector3 MathUtillty::Vector3TransformNormal(const Vector3& v, const Matrix4& m)
{

	Vector3 result{
	  v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
	  v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
	  v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] };

	return result;
}

Matrix4 MathUtillty::Matrix4Orthographic(float viewLeft, float viewRight, float viewBottom, float viewTop, float nearZ, float farZ)
{
	assert(fabsf(viewRight - viewLeft) > 0.00001f);
	assert(fabsf(viewTop - viewBottom) > 0.00001f);
	assert(fabsf(farZ - nearZ) > 0.00001f);

	float width = 1.0f / (viewRight - viewLeft);
	float height = 1.0f / (viewTop - viewBottom);
	float fRange = 1.0f / (farZ - nearZ);
	float sx = width * 2.0f;
	float sy = height * 2.0f;
	float sz = fRange;
	float tx = -(viewLeft + viewRight) * width;
	float ty = -(viewTop + viewBottom) * height;
	float tz = -fRange * nearZ;

	Matrix4 m{ sx, 0.0f, 0.0f, 0.0f, 0.0f, sy, 0.0f, 0.0f, 0.0f, 0.0f, sz, 0.0f, tx, ty, tz, 1.0f };

	return m;
}
