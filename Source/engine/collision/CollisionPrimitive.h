#pragma once 

#include "MathUtillity.h"

///<summary>
/// 球
///</summary>
struct Sphere
{
	//中心座標
	Vector3 center = { 0,0,0 };
	float upVec = 1.0f;

	//半径
	float radius = 1.0f;

	Vector3 pos = { 0,0,0 };
};

///<summary>
/// 平面
///</summary>
struct Plane
{
	//法線ベクトル
	Vector3 normal = { 0,1,0 };
	float upVec = 0.0f;

	//原点(0,0,0)からの距離
	float distance = 0.0f;
	Vector3 pos = { 0,0,0 };
};

///<summary>
/// レイ
///</summary>
struct Ray
{
	//始点座標
	Vector3 start = { 0,0,0 };
	float upVec = 0.0f;

	//方向(単位ベクトル)
	Vector3 dir = { 1,0,0 };
	float dirUpVec = 0.0f;
};

/// <summary>
/// 法線付き三角形(時計回りが表面)
/// </summary>
class  Triangle
{
public:

	void ComputeNormal();

public:
	//頂点座標3つ
	Vector3 p0_;
	Vector3 p1_;
	Vector3 p2_;
	//法線ベクトル
	Vector3 normal_;
};


