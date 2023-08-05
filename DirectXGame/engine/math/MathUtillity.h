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

	Vector3 Vector3Subtract(Vector3 v1, Vector3 v2);
	bool Vector3Equal(Vector3 v1, Vector3 v2);
	Vector3 Vector3Normalize(Vector3 v1);
	Vector3 Vector3Zero();
	Vector3 Vector3Dot(Vector3 v1, Vector3 v2);
	Vector3 Vector3Cross(Vector3 v1, Vector3 v2);
	// ���W�ϊ��iw���Z�Ȃ��j
	Vector3 Vector3Transform(const Vector3& v, const Matrix4& m);
	// ���W�ϊ��iw���Z����j
	Vector3 Vector3TransformCoord(const Vector3& v, const Matrix4& m);
	// �x�N�g���ϊ�
	Vector3 Vector3TransformNormal(const Vector3& v, const Matrix4& m);
	Matrix4 Matrix4Orthographic(float viewLeft, float viewRight, float viewBottom, float viewTop, float nearZ, float farZ);

}