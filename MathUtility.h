#pragma once
#include "Matrix4.h"
#include "Vector3.h"

namespace MathUtility 
{
	//円周率
	const float PI = 3.141592654f;

	//ビュー行列の作成
	Matrix4 Matrix4LookAtLH(const Vector3& eye, const Vector3& target, const Vector3& up);
	//透視投影行列の作成
	Matrix4 Matrix4Perspective(float fovAngleY, float aspectRatio, float nearZ, float farZ);

	//度数法からラジアンに変換
	float ToRadian(float angle);

	/*Matrix4 TestMatrix4LookAtLH();

	Matrix4 MatrixLooktoLH(Vector3 eyePos, Vector3 eyeDirection, Vector3 up);

	Vector3 VectorSubtract(Vector3 eyePos, Vector3 target);*/
}