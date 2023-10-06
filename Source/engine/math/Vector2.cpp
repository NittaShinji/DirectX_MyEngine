#include "Vector2.h"
#include <cmath>	//sqrt

Vector2::Vector2()
	:x(0), y(0)
{
}

Vector2::Vector2(float x, float y)
	: x(x), y(y)
{
}

float Vector2::Length() const
{
	return sqrt((x * x) + (y * y));
}

//正規化
Vector2& Vector2::Normalize()
{
	float len = Length();
	if(len != 0)
	{
		return *this /= len;
	}

	return *this;
}

//内積
float Vector2::Dot(const Vector2& v)const
{
	return  x * v.x + y * v.y;
}

//外積
float Vector2::Cross(const Vector2& v)const
{
	return x * v.y - y * v.x;
}

//単項演算子オーバーロード
Vector2 Vector2::operator+() const
{
	return *this;
}

Vector2 Vector2::operator-() const
{
	return Vector2(-x, -y);
}

//代入演算子オーバーロード
Vector2& Vector2::operator+=(const Vector2& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

Vector2& Vector2::operator*=(float s)
{
	x *= s;
	y *= s;
	return *this;
}

Vector2& Vector2::operator/=(float s)
{
	x /= s;
	y /= s;
	return *this;
}

// Vector2 クラスに属さない関数郡
//　二項演算子
const Vector2 operator+(const Vector2& v1, const Vector2& v2)
{
	Vector2 temp(v1);
	return temp += v2;
}

const Vector2 operator-(const Vector2& v1, const Vector2& v2)
{
	Vector2 temp(v1);
	return temp -= v2;
}

const Vector2 operator*(const Vector2& v, float s)
{
	Vector2 temp(v);
	return  temp *= s;
}

const Vector2 operator*(float s, const Vector2& v)
{
	return v * s;
}

const Vector2 operator/(const Vector2& v, float s)
{
	Vector2 temp(v);
	return temp /= s;
}