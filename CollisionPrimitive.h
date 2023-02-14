#pragma once 

#include <DirectXMath.h>

///<summary>
/// 球
///</summary>
struct Sphere
{
	//中心座標
	DirectX::XMVECTOR center = { 0,0,0,1 };
	//半径
	float radius = 1.0f;

	DirectX::XMFLOAT3 pos = { 0,0,0 };
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
	DirectX::XMFLOAT3 pos = { 0,0,0 };
};

/// <summary>
/// 法線付き三角形(時計回りが表面)
/// </summary>
class  Triangle
{
public:
	//頂点座標3つ
	DirectX::XMVECTOR p0;
	DirectX::XMVECTOR p1;
	DirectX::XMVECTOR p2;
	//法線ベクトル
	DirectX::XMVECTOR normal;

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

struct AABB
{
	using XMVECTOR = DirectX::XMVECTOR;
	using XMFLOAT3 = DirectX::XMFLOAT3;

	XMFLOAT3 max;
	XMFLOAT3 min;
	XMFLOAT3 oldPos;
	AABB() = default;
	~AABB() = default;
	AABB(XMFLOAT3 minValue, XMFLOAT3 maxValue, XMFLOAT3 pos)
	{
		min = minValue;
		max = maxValue;
		pos = oldPos;
	}
	//更新
	void Update(const XMFLOAT3& pos)
	{
		XMFLOAT3 diff;
		diff.x = pos.x - oldPos.x;
		diff.y = pos.y - oldPos.y;
		diff.z = pos.z - oldPos.z;

		min.x += diff.x;
		min.y += diff.y;
		min.z += diff.z;

		max.x += diff.x;
		max.y += diff.y;
		max.z += diff.z;

		oldPos = pos;
	}
};


