#pragma once

#include "Vector3.h"

// Matrix4 �\����
struct Matrix4
{
	float m[4][4];
};

//�P�ʍs������߂�
Matrix4 MatrixIdentity();

// �g��k���s��̐ݒ�
Matrix4 MatrixScale(const Vector3& s);

// ��]�s��̐ݒ�
Matrix4 MatrixRotateX(float angle);
Matrix4 MatrixRotateY(float angle);
Matrix4 MatrixRotateZ(float angle);

// ���s�ړ��s��̍쐬
Matrix4 MatrixTranslate(const Vector3& t);
 
// ���W�ϊ�(�x�N�g���ƍs��̊|���Z������)
//Vector3 MatrixTransform(const Vector3& v, const Matrix4& m);

//�]�u���čs�Ɨ�����ւ������̂����߂�
Matrix4 MatrixTranspose(const Matrix4& m);

//�t�s������߂�
Matrix4 MatrixInverse(const Matrix4& m);

// ������Z�q�I�[�o�[���[�h
Matrix4& operator*=(Matrix4& m1, const Matrix4& m2);

// 2�����Z�q�I�[�o�[���[�h
const Matrix4 operator*(const Matrix4& m1, const Matrix4& m2);
const Vector3 operator*(const Vector3& v, const Matrix4& m);
//�A�t�B���ϊ�
Vector3 TransformAffine(const Vector3& v, const Matrix4& m);

Matrix4 MatrixPerspectiveFovLH(float angle, float AspectRatio, float nearZ, float farZ);
