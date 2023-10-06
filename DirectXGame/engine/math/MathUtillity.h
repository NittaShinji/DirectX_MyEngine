#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4.h"

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

	Vector3 Vector3Subtract(Vector3 v1, Vector3 v2);
	bool Vector3Equal(Vector3 v1, Vector3 v2);
	Vector3 Vector3Normalize(Vector3 v1);
	Vector3 Vector3Zero();
	Vector3 Vector3Dot(Vector3 v1, Vector3 v2);
	Vector3 Vector3Cross(Vector3 v1, Vector3 v2);
	// 座標変換（w除算なし）
	Vector3 Vector3Transform(const Vector3& v, const Matrix4& m);
	// 座標変換（w除算あり）
	Vector3 Vector3TransformCoord(const Vector3& v, const Matrix4& m);
	// ベクトル変換
	Vector3 Vector3TransformNormal(const Vector3& v, const Matrix4& m);
	Matrix4 Matrix4Orthographic(float viewLeft, float viewRight, float viewBottom, float viewTop, float nearZ, float farZ);

}