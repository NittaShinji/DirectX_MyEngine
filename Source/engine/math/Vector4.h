#pragma once

/// <summary>
/// 4次元ベクトル
/// </summary>
class Vector4
{
public:
	float x; // x成分
	float y; // y成分
	float z; // z成分
	float w; // w成分

public:
	// コンストラクタ
	Vector4();                                   // 零ベクトルとする
	Vector4(float x, float y, float z, float w); // x成分, y成分, z成分 を指定しての生成

	//単項演算子オーバーロード
	Vector4 operator+() const;
	Vector4 operator-() const;

	//代入演算子オーバーロード
	Vector4& operator+=(const Vector4& v);
	Vector4& operator-=(const Vector4& v);
	Vector4& operator*=(float s);
	Vector4& operator/=(float s);

};
//　2項演算子オーバーロード
//　※いろんな引数(引数の型と順序)のパターンに対応するため
const Vector4 operator+(const Vector4& v1, const Vector4& v2);
const Vector4 operator-(const Vector4& v1, const Vector4& v2);
const Vector4 operator*(const Vector4& v, float s);
const Vector4 operator*(float s, const Vector4& v);
const Vector4 operator/(const Vector4& v, float s);


