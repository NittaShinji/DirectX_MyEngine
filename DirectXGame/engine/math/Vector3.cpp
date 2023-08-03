#include "Vector3.h"
#include <cmath>

Vector3::Vector3():x(0),y(0),z(0){}

Vector3::Vector3(float x, float y,float z): x(x), y(y), z(z){}

//���������߂�
float Vector3::Length() const
{
	return sqrt((x * x) + (y * y) + (z * z));
}

//���K��
Vector3& Vector3::Normalize()
{
	float len = Length();
	if (len != 0)
	{
		return *this /= len;
	}

	return *this;
}

//����
float Vector3::Dot(const Vector3& v)const
{
	return  (x * v.x) + (y * v.y) + (z * v.z);
}

//�O��
Vector3 Vector3::Cross(const Vector3& v)const
{
	return Vector3((y * v.z) - (z * v.y), (z * v.x) - (x * v.z),(x * v.y) - (y * v.x));
}

//�t�x�N�g��
Vector3 Vector3::Negate() const
{
	return Vector3(-x,-y,-z);
}

//�P�����Z�q�I�[�o�[���[�h
Vector3 Vector3::operator+() const
{
	return *this;
}

Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y,-z);
}

//������Z�q�I�[�o�[���[�h
Vector3& Vector3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector3& Vector3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector3& Vector3::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

// Vector2 �N���X�ɑ����Ȃ��֐��S
//�@�񍀉��Z�q
const Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp += v2;
}

const Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp -= v2;
}

const Vector3 operator*(const Vector3& v, float s)
{
	Vector3 temp(v);
	return  temp *= s;
}

const Vector3 operator*(float s, const Vector3& v)
{
	return v * s;
}

const Vector3 operator/(const Vector3& v, float s)
{
	Vector3 temp(v);
	return temp /= s;
}

//���`���
const Vector3 lerp(const Vector3& start, const Vector3& end, const float t)
{
	return start * (1.0f - t) + end * t;
}
