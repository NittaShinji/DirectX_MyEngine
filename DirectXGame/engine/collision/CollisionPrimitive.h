#pragma once 

#include <DirectXMath.h>
#include "Vector3.h"

///<summary>
/// 球
///</summary>
struct Sphere
{
	//中心座標
	DirectX::XMVECTOR center = { 0,0,0,1 };
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
	DirectX::XMVECTOR normal = { 0,1,0,0 };
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
	DirectX::XMVECTOR start = { 0,0,0,1 };
	//方向(単位ベクトル)
	DirectX::XMVECTOR dir = { 1,0,0,0 };
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
	DirectX::XMVECTOR p0_;
	DirectX::XMVECTOR p1_;
	DirectX::XMVECTOR p2_;
	//法線ベクトル
	DirectX::XMVECTOR normal_;

};


///<summary>
/// AABB
///</summary>
struct AABBScale
{
	DirectX::XMVECTOR max;
	DirectX::XMVECTOR min;
};

struct AABBWidth
{
	DirectX::XMVECTOR max;
	float d[3];
};

struct AABBRadius
{
	DirectX::XMVECTOR center;
	DirectX::XMVECTOR r[3];
};
//
//struct AABB
//{
//	using XMVECTOR = DirectX::XMVECTOR;
//	using Vector3 = DirectX::Vector3;
//
//	Vector3 max;
//	Vector3 min;
//	Vector3 oldPos;
//	AABB() = default;
//	~AABB() = default;
//	AABB(Vector3 minValue, Vector3 maxValue, Vector3 pos)
//	{
//		min = minValue;
//		max = maxValue;
//		pos = oldPos;
//	}
//	//更新
//	void Update(const Vector3& pos)
//	{
//		Vector3 diff;
//		diff.x = pos.x - oldPos.x;
//		diff.y = pos.y - oldPos.y;
//		diff.z = pos.z - oldPos.z;
//
//		min.x += diff.x;
//		min.y += diff.y;
//		min.z += diff.z;
//
//		max.x += diff.x;
//		max.y += diff.y;
//		max.z += diff.z;
//
//		oldPos = pos;
//	}
//};


