#pragma once

class Vector2
{
public:
	float x; // x成分
	float y; // x成分

public:
	//コンストラクタ
	Vector2();						// 零ベクトルとして生成
	Vector2(float x, float y);		//　x成分,y成分を指定しての生成

	//メンバ関数
	float Length() const;			//	ノルム(長さ)を求める
	Vector2& Normalize();			//　正規化する
	float Dot(const Vector2& v) const;		//　内積を求める
	float Cross(const Vector2& v) const;	//　外積を求める

	//単項演算子オーバーロード
	Vector2 operator+() const;
	Vector2 operator-() const;

	//代入演算子オーバーロード
	Vector2& operator+=(const Vector2& v);
	Vector2& operator-=(const Vector2& v);
	Vector2& operator*=(float s);
	Vector2& operator/=(float s);

};

//　2項演算子オーバーロード
//　※いろんな引数(引数の型と順序)のパターンに対応するため、以下のように準備
const Vector2 operator+(const Vector2& v1, const Vector2& v2);
const Vector2 operator-(const Vector2& v1, const Vector2& v2);
const Vector2 operator*(const Vector2& v, float s);
const Vector2 operator*(float s, const Vector2& v);
const Vector2 operator/(const Vector2& v, float s);