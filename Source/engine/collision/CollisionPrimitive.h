#pragma once 

#include "MathUtillity.h"

///<summary>
/// 球
///</summary>
struct Sphere
{
	//中心座標
	const Vector3 kDefaultCenter = { 0.0f,0.0f,0.0f };
	Vector3 center = kDefaultCenter;

	const float kDefaultUpVec = 1.0f;
	float upVec = kDefaultUpVec;

	//半径
	const float kDefaultSphereRadius = 1.0f;
	float sphereRadius = kDefaultSphereRadius;

	const Vector3 kDefaultPos = { 0.0f,0.0f,0.0f};
	Vector3 pos = kDefaultPos;
};

///<summary>
/// 平面
///</summary>
struct Plane
{
	//法線ベクトル
	const Vector3 kDefaultNormal = { 0.0f,1.0f,0.0f };
	Vector3 normal = kDefaultNormal;

	const float kDefaultUpVec = 0.0f;
	float upVec = kDefaultUpVec;

	//原点(0,0,0)からの距離
	const float kDefaultDistance = 0.0f;
	float distance = kDefaultDistance;

	const Vector3 kDefaultPos = { 0.0f,0.0f,0.0f };
	Vector3 pos = kDefaultPos;
};

///<summary>
/// レイ
///</summary>
struct Ray
{
	//始点座標
	const Vector3 kDefaultStart = { 0.0f,0.0f,0.0f };
	Vector3 start = kDefaultStart;

	const float kDefaultUpVec = 0.0f;
	float upVec = kDefaultUpVec;

	//方向(単位ベクトル)
	const Vector3 kDefaultDir = { 1.0f,0.0f,0.0f };
	Vector3 dir = kDefaultDir;

	const float kDefaultDirUpVec = 0.0f;
	float dirUpVec = kDefaultDirUpVec;
};

/// <summary>
/// 法線付き三角形(時計回りが表面)
/// </summary>
class  Triangle
{
public:

	/// <summary>
	/// 法線の計算
	/// </summary>
	void ComputeNormal();

public:
	//頂点座標3つ
	Vector3 p0_;
	Vector3 p1_;
	Vector3 p2_;
	//法線ベクトル
	Vector3 normal_;
};


