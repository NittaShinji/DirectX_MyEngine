#pragma once
#include "Matrix4.h"
#include "Vector3.h"

namespace MathUtility 
{
	//�~����
	const float PI = 3.141592654f;

	//�r���[�s��̍쐬
	Matrix4 Matrix4LookAtLH(const Vector3& eye, const Vector3& target, const Vector3& up);
	//�������e�s��̍쐬
	Matrix4 Matrix4Perspective(float fovAngleY, float aspectRatio, float nearZ, float farZ);

	//�x���@���烉�W�A���ɕϊ�
	float ToRadian(float angle);

	/*Matrix4 TestMatrix4LookAtLH();

	Matrix4 MatrixLooktoLH(Vector3 eyePos, Vector3 eyeDirection, Vector3 up);

	Vector3 VectorSubtract(Vector3 eyePos, Vector3 target);*/
}