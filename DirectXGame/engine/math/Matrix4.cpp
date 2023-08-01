// ======================
// Matrix4 型 Matrix4.cpp
// ======================

#include "Matrix4.h"
#include <cmath> // sin cod

// 単位行列を求める
Matrix4 identity()
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
Matrix4 scale(const Vector3& s)
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
Matrix4 rotateX(float angle)
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
Matrix4 rotateZ(float angle)
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
Matrix4 rotateY(float angle)
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
Matrix4 translate(const Vector3& t)
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

// 座標変換(ベクトルと行列の掛け算)を行うtransform 関数を作成する。(透視変換にも対応している)
Vector3 transform(const Vector3& v, const Matrix4& m)
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

//代入演算子　*= のオーバーロード関数(行列と行列の積)
Matrix4& operator*=(Matrix4& m1,const Matrix4& m2)
{
	Matrix4 result{ 0 };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				result.m[i][j] = result.m[i][j] +  m1.m[i][k] * m2.m[k][j];
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
	return transform(v, m);
}

//アフィン変換 
Vector3 TransformAffine(const Vector3& v, const Matrix4& m)
{
	return Vector3{
		((m.m[0][0] * v.x) + (m.m[1][0] * v.y) + (m.m[2][0] * v.z) + m.m[3][0] ),
		((m.m[0][1] * v.x) + (m.m[1][1] * v.y) + (m.m[2][1] * v.z) + m.m[3][1] ),
		((m.m[0][2] * v.x) + (m.m[1][2] * v.y) + (m.m[2][2] * v.z) + m.m[3][2] )
	};	
}