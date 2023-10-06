#pragma once
#include "Matrix4.h"

class Quaternion
{

public:

	float x;
	float y;
	float z;
	float w;

	//単項演算子オーバーロード
	Quaternion operator+() const;
	Quaternion operator-() const;

	//代入演算子オーバーロード
	Quaternion& operator+=(const Quaternion& v);
	Quaternion& operator-=(const Quaternion& v);
	Quaternion& operator*=(float s);
	Quaternion& operator/=(float s);

	//Quaternionの積
	Quaternion Multiply(const Quaternion& rhs) const;
	//単位Quaternionを返す
	Quaternion IdentityQuaternion();
	//共役Quaternionを返す
	Quaternion Conjugate() const;
	//Quaternionのnormを返す
	float Norm();
	//正規化したQuaternionを返す
	Quaternion Normalize();
	//逆Quaternionを返す
	Quaternion Inverse();
	//Quaternionから回転行列を求める
	Matrix4 MakeRotateMatrix();
	//球面線形補間
	Quaternion Slerp(const Quaternion& q1, float t);

};

//　2項演算子オーバーロード
//　※いろんな引数(引数の型と順序)のパターンに対応するため、以下のように準備
const Quaternion operator+(const Quaternion& v1, const Quaternion& v2);
const Quaternion operator-(const Quaternion& v1, const Quaternion& v2);
const Quaternion operator*(const Quaternion& v, float s);
const Quaternion operator*(float s, const Quaternion& v);
const Quaternion operator/(const Quaternion& v, float s);

//任意軸回転
Quaternion MakeAxisAngle(const Vector3& axis, float angle);
//ベクトルをQuaternionで回転させた結果のベクトルを求める
Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion);
//uからvへの回転を生成
Quaternion DirectionToDirection(const Vector3& u, const Vector3& v);
