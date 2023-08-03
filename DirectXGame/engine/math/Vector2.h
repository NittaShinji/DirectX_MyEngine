#pragma once

class Vector2
{
public:
	float x; // x����
	float y; // x����

public:
	//�R���X�g���N�^
	Vector2();						// ��x�N�g���Ƃ��Đ���
	Vector2(float x, float y);		//�@x����,y�������w�肵�Ă̐���

	//�����o�֐�
	float Length() const;			//	�m����(����)�����߂�
	Vector2& Normalize();			//�@���K������
	float Dot(const Vector2& v) const;		//�@���ς����߂�
	float Cross(const Vector2& v) const;	//�@�O�ς����߂�

	//�P�����Z�q�I�[�o�[���[�h
	Vector2 operator+() const;
	Vector2 operator-() const;

	//������Z�q�I�[�o�[���[�h
	Vector2& operator+=(const Vector2& v);
	Vector2& operator-=(const Vector2& v);
	Vector2& operator*=(float s);
	Vector2& operator/=(float s);
	
};

//�@2�����Z�q�I�[�o�[���[�h
//�@�������Ȉ���(�����̌^�Ə���)�̃p�^�[���ɑΉ����邽�߁A�ȉ��̂悤�ɏ���
const Vector2 operator+(const Vector2& v1, const Vector2& v2);
const Vector2 operator-(const Vector2& v1, const Vector2& v2);
const Vector2 operator*(const Vector2& v, float s);
const Vector2 operator*(float s, const Vector2& v);
const Vector2 operator/(const Vector2& v, float s);