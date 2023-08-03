#include "Quaternion.h"
#include <cmath>

#pragma region 演算子オーバーロード

//単項演算子オーバーロード
Quaternion Quaternion::operator+() const
{
	return *this;
}

Quaternion Quaternion::operator-() const
{
	return Quaternion{ -x, -y, -z, -w };
}

//代入演算子オーバーロード
Quaternion &Quaternion::operator+=(const Quaternion &q)
{
	x += q.x;
	y += q.y;
	z += q.z;
	w += q.w;
	return *this;
}

Quaternion &Quaternion::operator-=(const Quaternion &q)
{
	x -= q.x;
	y -= q.y;
	z -= q.z;
	w -= q.z;
	return *this;
}

Quaternion &Quaternion::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	w *= s;
	return *this;
}

Quaternion &Quaternion::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	w /= s;
	return *this;
}

#pragma endregion 演算子オーバーロード

#pragma region 二項演算子
// 二項演算子
const Quaternion operator+(const Quaternion &q1, const Quaternion &q2)
{
	Quaternion temp(q1);
	return temp += q2;
}

const Quaternion operator-(const Quaternion &v1, const Quaternion &q2)
{
	Quaternion temp(v1);
	return temp -= q2;
}

const Quaternion operator*(const Quaternion &q, float s)
{
	Quaternion temp(q);
	return  temp *= s;
}

const Quaternion operator*(float s, const Quaternion &q)
{
	return q * s;
}

const Quaternion operator/(const Quaternion &q, float s)
{
	Quaternion temp(q);
	return temp /= s;
}

#pragma endregion 二項演算子

//Quaternionの積
Quaternion Quaternion::Multiply(const Quaternion &rhs) const
{
	return Quaternion
	{ (this->x * rhs.w + this->w * rhs.x - this->z * rhs.y + this->y * rhs.z),
	  (this->y * rhs.w + this->z * rhs.x + this->w * rhs.y - this->x * rhs.z),
	  (this->z * rhs.w - this->y * rhs.x + this->x * rhs.y + this->w * rhs.z),
	  (this->w * rhs.w - this->x * rhs.x - this->y * rhs.y - this->z * rhs.z) };
}

//単位Quaternionを返す(元の値を変えない関数)
Quaternion Quaternion::IdentityQuaternion()
{
	return Quaternion{ 0,0,0,1 };
}

//共役Quaternionを返す
Quaternion Quaternion::Conjugate() const
{
	return Quaternion{ -this->x ,-this->y,-this->z,this->w };
}

//Quaternionのノルム(長さ)を返す
float Quaternion::Norm()
{
	return sqrt((this->w * this->w) + (this->x * this->x)
		+ (this->y * this->y) + (this->z * this->z));
}

//正規化したQuaternionを返す
Quaternion Quaternion::Normalize()
{
	return Quaternion{ this->x / Norm(),
		this->y / Norm(),
		this->z / Norm(),
		this->w / Norm() };
}

//逆Quaternionを返す(元のQuaternionとの積が1となる)
Quaternion Quaternion::Inverse()
{
	return Quaternion{ Conjugate().x / (Norm() * Norm()),
		Conjugate().y / (Norm() * Norm()),
		Conjugate().z / (Norm() * Norm()),
		Conjugate().w / (Norm() * Norm()) };
}

//任意軸回転
Quaternion MakeAxisAngle(const Vector3 &axis, float angle)
{
	Vector3 axisNorm = axis;
	axisNorm.Normalize();

	Quaternion result;

	float sin_ = sin(angle / 2);

	result.x = axisNorm.x * sin_;
	result.y = axisNorm.y * sin_;
	result.z = axisNorm.z * sin_;
	result.w = cos(angle / 2);

	return result;
}

//ベクトルをQuaternionで回転させた結果のベクトルを求める
Vector3 RotateVector(const Vector3 &vector, const Quaternion &quaternion)
{
	//ベクトルをQuaternionに変更
	Quaternion vecToQuaternion = { vector.x, vector.y, vector.z, 0 };

	//回転後のベクトルを求める(r' = qrq*)
	Quaternion resultQuaternion = quaternion.Multiply(vecToQuaternion);

	resultQuaternion = resultQuaternion.Multiply(quaternion.Conjugate());

	return Vector3(resultQuaternion.x, resultQuaternion.y, resultQuaternion.z);
}

//Quaternionから回転行列を求める
Matrix4 Quaternion::MakeRotateMatrix()
{
	Matrix4 resultMat{ };

	resultMat = identity();

	resultMat.m[0][0] = float(pow(this->w, 2) + pow(this->x, 2) - pow(this->y, 2) - pow(this->z, 2));
	resultMat.m[0][1] = 2 * ((this->x * this->y) + (this->w * this->z));
	resultMat.m[0][2] = 2 * ((this->x * this->z) - (this->w * this->y));

	resultMat.m[1][0] = 2 * ((this->x * this->y) - (this->w * this->z));
	resultMat.m[1][1] = float(pow(this->w, 2) - pow(this->x, 2) + pow(this->y, 2) - pow(this->z, 2));
	resultMat.m[1][2] = 2 * ((this->y * this->z) + (this->w * this->x));

	resultMat.m[2][0] = 2 * ((this->x * this->z) + (this->w * this->y));
	resultMat.m[2][1] = 2 * ((this->y * this->z) - (this->w * this->x));
	resultMat.m[2][2] = float(pow(this->w, 2) - pow(this->x, 2) - pow(this->y, 2) + pow(this->z, 2));

	return resultMat;
}

//球面線形補間
Quaternion Quaternion::Slerp(const Quaternion &q1, float t)
{
	//Quaternionの内積
	float dot = this->x * q1.x + this->y * q1.y + this->z * q1.z + this->w * q1.w;

	//シータθ
	float theta = std::acos(dot);

	//内積で近しいかどうかを判断
	if(dot < 0)
	{
		*this = -(*this);   //もう片方の回転を利用する
		dot = -dot;     //内積も反転
	}

	//補間係数を求める
	float scale0;
	float scale1;

	scale0 = sin((1 - t) * theta) / sin(theta);
	scale1 = sin(t * theta) / sin(theta);

	//0除算を避けるため、θ=0?内積が1に近い場合、線形補間を利用する
	const float EPSILON = 0.0005f;

	if(dot >= 1.0f - EPSILON)
	{
		//線形補間
		return (1.0f - t) * (*this) + t * q1;
	}
	else
	{
		//球面線形
		return scale0 * (*this) + scale1 * q1;
	}
}

//uからvへの回転を生成
Quaternion DirectionToDirection(const Vector3& u, const Vector3& v)
{
	//uとvを正規化して内積を求める,u,vを単位ベクトル前提とするなら正規化は不要
	Vector3 before = u;
	before.Normalize();

	Vector3 after = v;
	after.Normalize();

	float dot = before.Dot(after);

	//u,vの外積をとる
	Vector3 cross = u.Cross(v);

	//軸は単位ベクトルである必要があるので正規化
	//u,vが単位ベクトルであっても、外積が単位ベクトルとは限らないのでここの正規化は必須
	Vector3 axis = cross.Normalize();

	//単位ベクトルで内積を取っているのでacosで角度を求める
	float theta = std::acosf(dot);

	//axisとthetaで任意軸回転を作って返す
	Quaternion result = MakeAxisAngle(axis, theta);

	return result;
}
