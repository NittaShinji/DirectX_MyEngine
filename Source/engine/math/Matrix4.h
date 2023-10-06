#pragma once

#include "Vector3.h"

// Matrix4 構造体
struct Matrix4
{
	float m[4][4];
};

//単位行列を求める
Matrix4 MatrixIdentity();

// 拡大縮小行列の設定
Matrix4 MatrixScale(const Vector3& s);

// 回転行列の設定
Matrix4 MatrixRotateX(float angle);
Matrix4 MatrixRotateY(float angle);
Matrix4 MatrixRotateZ(float angle);

// 平行移動行列の作成
Matrix4 MatrixTranslate(const Vector3& t);

// 座標変換(ベクトルと行列の掛け算をする)
//Vector3 MatrixTransform(const Vector3& v, const Matrix4& m);

//転置して行と列を入れ替えたものを求める
Matrix4 MatrixTranspose(const Matrix4& m);

//逆行列を求める
Matrix4 MatrixInverse(const Matrix4& m);

// 代入演算子オーバーロード
Matrix4& operator*=(Matrix4& m1, const Matrix4& m2);

// 2項演算子オーバーロード
const Matrix4 operator*(const Matrix4& m1, const Matrix4& m2);
const Vector3 operator*(const Vector3& v, const Matrix4& m);
//アフィン変換
Vector3 TransformAffine(const Vector3& v, const Matrix4& m);

Matrix4 MatrixPerspectiveFovLH(float angle, float AspectRatio, float nearZ, float farZ);
