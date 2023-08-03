#include "Quaternion.h"
#include <cmath>

#pragma region ���Z�q�I�[�o�[���[�h

//�P�����Z�q�I�[�o�[���[�h
Quaternion Quaternion::operator+() const
{
	return *this;
}

Quaternion Quaternion::operator-() const
{
	return Quaternion{ -x, -y, -z, -w };
}

//������Z�q�I�[�o�[���[�h
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

#pragma endregion ���Z�q�I�[�o�[���[�h

#pragma region �񍀉��Z�q
// �񍀉��Z�q
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

#pragma endregion �񍀉��Z�q

//Quaternion�̐�
Quaternion Quaternion::Multiply(const Quaternion &rhs) const
{
	return Quaternion
	{ (this->x * rhs.w + this->w * rhs.x - this->z * rhs.y + this->y * rhs.z),
	  (this->y * rhs.w + this->z * rhs.x + this->w * rhs.y - this->x * rhs.z),
	  (this->z * rhs.w - this->y * rhs.x + this->x * rhs.y + this->w * rhs.z),
	  (this->w * rhs.w - this->x * rhs.x - this->y * rhs.y - this->z * rhs.z) };
}

//�P��Quaternion��Ԃ�(���̒l��ς��Ȃ��֐�)
Quaternion Quaternion::IdentityQuaternion()
{
	return Quaternion{ 0,0,0,1 };
}

//����Quaternion��Ԃ�
Quaternion Quaternion::Conjugate() const
{
	return Quaternion{ -this->x ,-this->y,-this->z,this->w };
}

//Quaternion�̃m����(����)��Ԃ�
float Quaternion::Norm()
{
	return sqrt((this->w * this->w) + (this->x * this->x)
		+ (this->y * this->y) + (this->z * this->z));
}

//���K������Quaternion��Ԃ�
Quaternion Quaternion::Normalize()
{
	return Quaternion{ this->x / Norm(),
		this->y / Norm(),
		this->z / Norm(),
		this->w / Norm() };
}

//�tQuaternion��Ԃ�(����Quaternion�Ƃ̐ς�1�ƂȂ�)
Quaternion Quaternion::Inverse()
{
	return Quaternion{ Conjugate().x / (Norm() * Norm()),
		Conjugate().y / (Norm() * Norm()),
		Conjugate().z / (Norm() * Norm()),
		Conjugate().w / (Norm() * Norm()) };
}

//�C�ӎ���]
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

//�x�N�g����Quaternion�ŉ�]���������ʂ̃x�N�g�������߂�
Vector3 RotateVector(const Vector3 &vector, const Quaternion &quaternion)
{
	//�x�N�g����Quaternion�ɕύX
	Quaternion vecToQuaternion = { vector.x, vector.y, vector.z, 0 };

	//��]��̃x�N�g�������߂�(r' = qrq*)
	Quaternion resultQuaternion = quaternion.Multiply(vecToQuaternion);

	resultQuaternion = resultQuaternion.Multiply(quaternion.Conjugate());

	return Vector3(resultQuaternion.x, resultQuaternion.y, resultQuaternion.z);
}

//Quaternion�����]�s������߂�
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

//���ʐ��`���
Quaternion Quaternion::Slerp(const Quaternion &q1, float t)
{
	//Quaternion�̓���
	float dot = this->x * q1.x + this->y * q1.y + this->z * q1.z + this->w * q1.w;

	//�V�[�^��
	float theta = std::acos(dot);

	//���ςŋ߂������ǂ����𔻒f
	if(dot < 0)
	{
		*this = -(*this);   //�����Е��̉�]�𗘗p����
		dot = -dot;     //���ς����]
	}

	//��ԌW�������߂�
	float scale0;
	float scale1;

	scale0 = sin((1 - t) * theta) / sin(theta);
	scale1 = sin(t * theta) / sin(theta);

	//0���Z������邽�߁A��=0?���ς�1�ɋ߂��ꍇ�A���`��Ԃ𗘗p����
	const float EPSILON = 0.0005f;

	if(dot >= 1.0f - EPSILON)
	{
		//���`���
		return (1.0f - t) * (*this) + t * q1;
	}
	else
	{
		//���ʐ��`
		return scale0 * (*this) + scale1 * q1;
	}
}

//u����v�ւ̉�]�𐶐�
Quaternion DirectionToDirection(const Vector3& u, const Vector3& v)
{
	//u��v�𐳋K�����ē��ς����߂�,u,v��P�ʃx�N�g���O��Ƃ���Ȃ琳�K���͕s�v
	Vector3 before = u;
	before.Normalize();

	Vector3 after = v;
	after.Normalize();

	float dot = before.Dot(after);

	//u,v�̊O�ς��Ƃ�
	Vector3 cross = u.Cross(v);

	//���͒P�ʃx�N�g���ł���K�v������̂Ő��K��
	//u,v���P�ʃx�N�g���ł����Ă��A�O�ς��P�ʃx�N�g���Ƃ͌���Ȃ��̂ł����̐��K���͕K�{
	Vector3 axis = cross.Normalize();

	//�P�ʃx�N�g���œ��ς�����Ă���̂�acos�Ŋp�x�����߂�
	float theta = std::acosf(dot);

	//axis��theta�ŔC�ӎ���]������ĕԂ�
	Quaternion result = MakeAxisAngle(axis, theta);

	return result;
}
