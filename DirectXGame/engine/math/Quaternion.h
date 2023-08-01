#pragma once
#include "Matrix4.h"

class Quaternion
{

public:

	float x;
	float y;
	float z;
	float w;

	//�P�����Z�q�I�[�o�[���[�h
	Quaternion operator+() const;
	Quaternion operator-() const;

	//������Z�q�I�[�o�[���[�h
	Quaternion& operator+=(const Quaternion& v);
	Quaternion& operator-=(const Quaternion& v);
	Quaternion& operator*=(float s);
	Quaternion& operator/=(float s);

	//Quaternion�̐�
	Quaternion Multiply(const Quaternion& rhs) const;
	//�P��Quaternion��Ԃ�
	Quaternion IdentityQuaternion();
	//����Quaternion��Ԃ�
	Quaternion Conjugate() const;
	//Quaternion��norm��Ԃ�
	float Norm();
	//���K������Quaternion��Ԃ�
	Quaternion Normalize();
	//�tQuaternion��Ԃ�
	Quaternion Inverse();
	//Quaternion�����]�s������߂�
	Matrix4 MakeRotateMatrix();
	//���ʐ��`���
	Quaternion Slerp(const Quaternion& q1, float t);
	
};

//�@2�����Z�q�I�[�o�[���[�h
//�@�������Ȉ���(�����̌^�Ə���)�̃p�^�[���ɑΉ����邽�߁A�ȉ��̂悤�ɏ���
const Quaternion operator+(const Quaternion& v1, const Quaternion& v2);
const Quaternion operator-(const Quaternion& v1, const Quaternion& v2);
const Quaternion operator*(const Quaternion& v, float s);
const Quaternion operator*(float s, const Quaternion& v);
const Quaternion operator/(const Quaternion& v, float s);

//�C�ӎ���]
Quaternion MakeAxisAngle(const Vector3& axis, float angle);
//�x�N�g����Quaternion�ŉ�]���������ʂ̃x�N�g�������߂�
Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion);
//u����v�ւ̉�]�𐶐�
Quaternion DirectionToDirection(const Vector3 &u, const Vector3 &v);
