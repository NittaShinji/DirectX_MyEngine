// ======================
// Matrix4 型 Matrix4.cpp
// ======================

#include "Matrix4.h"
#include <cmath> // sin cod
#include <cassert>

// 単位行列を求める
Matrix4 MatrixIdentity()
{
	static const Matrix4 result
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	return result;
}

// 拡大縮小行列を求める
Matrix4 MatrixScale(const Vector3& s)
{
	Matrix4 result
	{
		s.x,0.0f,0.0f,0.0f,
		0.0f,s.y,0.0f,0.0f,
		0.0f,0.0f,s.z,0.0f,
		0.0f,0.0f,0.0f,1.0f,
	};

	return result;
}

#pragma region 各回転行列を求める

// X 軸周りの回転行列を求める
Matrix4 MatrixRotateX(float angle)
{
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	Matrix4 result
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,cos,sin,0.0f,
		0.0f,-sin,cos,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	return result;
}

// Y 軸周りの回転行列を求める
Matrix4 MatrixRotateY(float angle)
{
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	Matrix4 result
	{
		cos,0.0f,-sin,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		sin,0.0f,cos,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	return result;
}

// Z 軸周りの回転行列を求める
Matrix4 MatrixRotateZ(float angle)
{
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	Matrix4 result
	{
		cos,sin,0.0f,0.0f,
		-sin,cos,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	return result;
}

#pragma endregion

//平行移動行列を求める
Matrix4 MatrixTranslate(const Vector3& t)
{
	Matrix4 result
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		t.x,t.y,t.z,1.0f
	};

	return result;
}

// 座標変換(ベクトルと行列の掛け算)を行うtransform 関数を作成する。(透視変換にも対応)
Vector3 MatrixTransform(const Vector3& v, const Matrix4& m)
{
	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v
		.z * m.m[2][3] * m.m[3][3];

	Vector3 result
	{
		(v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0]) / w,
		(v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1]) / w,
		(v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2]) / w
	};

	return result;
}

Matrix4 MatrixTranspose(const Matrix4& m)
{
	Matrix4 result{ 0 };

	for(size_t i = 0; i < 4; i++)
	{
		for(size_t j = 0; j < 4; j++)
		{
			result.m[i][j] = m.m[j][i];
		}
	}

	return result;
}

Matrix4 MatrixInverse(const Matrix4& m)
{
	//計算用の行列
	Matrix4 calculation{ 0 };
	calculation.m[0][0] = m.m[1][1] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) - m.m[1][2] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) + m.m[1][3] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1]);
	calculation.m[0][1] = -(m.m[1][0] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) - m.m[1][2] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) + m.m[1][3] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0]));
	calculation.m[0][2] = m.m[1][0] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) - m.m[1][1] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) + m.m[1][3] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0]);
	calculation.m[0][3] = -(m.m[1][0] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1]) - m.m[1][1] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0]) + m.m[1][2] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0]));

	calculation.m[1][0] = -(m.m[0][1] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) - m.m[0][2] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) + m.m[0][3] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1]));
	calculation.m[1][1] = m.m[0][0] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) - m.m[0][2] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) + m.m[0][3] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0]);
	calculation.m[1][2] = -(m.m[0][0] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) - m.m[0][1] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) + m.m[0][3] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0]));
	calculation.m[1][3] = m.m[0][0] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1]) - m.m[0][1] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0]) + m.m[0][2] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0]);

	calculation.m[2][0] = m.m[0][1] * (m.m[1][2] * m.m[3][3] - m.m[1][3] * m.m[3][2]) - m.m[0][2] * (m.m[1][1] * m.m[3][3] - m.m[1][3] * m.m[3][1]) + m.m[0][3] * (m.m[1][1] * m.m[3][2] - m.m[1][2] * m.m[3][1]);
	calculation.m[2][1] = -(m.m[0][0] * (m.m[1][2] * m.m[3][3] - m.m[1][3] * m.m[3][2]) - m.m[0][2] * (m.m[1][0] * m.m[3][3] - m.m[1][3] * m.m[3][0]) + m.m[0][3] * (m.m[1][0] * m.m[3][2] - m.m[1][2] * m.m[3][0]));
	calculation.m[2][2] = m.m[0][0] * (m.m[1][1] * m.m[3][3] - m.m[1][3] * m.m[3][1]) - m.m[0][1] * (m.m[1][0] * m.m[3][3] - m.m[1][3] * m.m[3][0]) + m.m[0][3] * (m.m[1][0] * m.m[3][1] - m.m[1][1] * m.m[3][0]);
	calculation.m[2][3] = -(m.m[0][0] * (m.m[1][1] * m.m[3][2] - m.m[1][2] * m.m[3][1]) - m.m[0][1] * (m.m[1][0] * m.m[3][2] - m.m[1][2] * m.m[3][0]) + m.m[0][2] * (m.m[1][0] * m.m[3][1] - m.m[1][1] * m.m[3][0]));

	calculation.m[3][0] = -(m.m[0][1] * (m.m[1][2] * m.m[2][3] - m.m[1][3] * m.m[2][2]) - m.m[0][2] * (m.m[1][1] * m.m[2][3] - m.m[1][3] * m.m[2][1]) + m.m[0][3] * (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1]));
	calculation.m[3][1] = m.m[0][0] * (m.m[1][2] * m.m[2][3] - m.m[1][3] * m.m[2][2]) - m.m[0][2] * (m.m[1][0] * m.m[2][3] - m.m[1][3] * m.m[2][0]) + m.m[0][3] * (m.m[1][0] * m.m[2][2] - m.m[1][2] * m.m[2][0]);
	calculation.m[3][2] = -(m.m[0][0] * (m.m[1][1] * m.m[2][3] - m.m[1][3] * m.m[2][1]) - m.m[0][1] * (m.m[1][0] * m.m[2][3] - m.m[1][3] * m.m[2][0]) + m.m[0][3] * (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0]));
	calculation.m[3][3] = m.m[0][0] * (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1]) - m.m[0][1] * (m.m[1][0] * m.m[2][2] - m.m[1][2] * m.m[2][0]) + m.m[0][2] * (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0]);

	// 引数行列の行列式を計算
	double detA = m.m[0][0] * calculation.m[0][0] + m.m[0][1] * calculation.m[0][1] + m.m[0][2] * calculation.m[0][2] + m.m[0][3] * calculation.m[0][3];

	//返り値用の行列
	Matrix4 result{ 0 };

	// 引数行列の逆行列を計算
	for(int i = 0; i < 4; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			result.m[i][j] = static_cast<float>(calculation.m[j][i] / detA);
		}
	}

	return result;
}

//代入演算子　*= のオーバーロード関数(行列と行列の積)
Matrix4& operator*=(Matrix4& m1, const Matrix4& m2)
{
	Matrix4 result{ 0 };

	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			for(int k = 0; k < 4; k++)
			{
				result.m[i][j] = result.m[i][j] + m1.m[i][k] * m2.m[k][j];
			}
		}
	}

	m1 = result;
	return m1;
}

// 2項演算子* のオーバーロード関数(行列と行列の積)
const Matrix4 operator*(const Matrix4& m1, const Matrix4& m2)
{
	Matrix4 result = m1;

	return result *= m2;
}

//2項演算子*のオーバーロード関数(ベクトルと行列の積)
const Vector3 operator*(const Vector3& v, const Matrix4& m)
{
	return MatrixTransform(v, m);
}

//アフィン変換 
Vector3 TransformAffine(const Vector3& v, const Matrix4& m)
{
	return Vector3{
		((m.m[0][0] * v.x) + (m.m[1][0] * v.y) + (m.m[2][0] * v.z) + m.m[3][0]),
		((m.m[0][1] * v.x) + (m.m[1][1] * v.y) + (m.m[2][1] * v.z) + m.m[3][1]),
		((m.m[0][2] * v.x) + (m.m[1][2] * v.y) + (m.m[2][2] * v.z) + m.m[3][2])
	};
}

Matrix4 MatrixPerspectiveFovLH(float angle, float aspectRatio, float nearZ, float farZ)
{
	assert(nearZ > 0.0f && farZ > 0.f);
	assert(fabsf(angle) > 0.00001f * 2.0f);
	assert(fabsf(aspectRatio) > 0.00001f);
	assert(fabsf(farZ - nearZ) > 0.00001f);

	Matrix4 result = { 0 };
	float tanFov = std::tan(angle / 2.0f);
	result.m[0][0] = 1.0f / (aspectRatio * tanFov);
	result.m[1][1] = 1.0f / tanFov;
	result.m[2][2] = (farZ + nearZ) / (farZ - nearZ);
	result.m[2][3] = 1.0f;
	result.m[3][2] = -(2.0f * farZ * nearZ) / (farZ - nearZ);
	return result;
}
