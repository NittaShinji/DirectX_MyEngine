#pragma once

#include "CollisionPrimitive.h"

class Collision
{
public:
	Collision();
	~Collision();

	//球と平面の衝突判定
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, DirectX::XMVECTOR* inter = nullptr);
	//AABBとAABBの衝突判定
	static bool CheckAABB(const AABB& box1, const AABB& box2);

	/// <summary>
	///点と三角形の最近接点を求める 
	/// </summary>
	/// <param name = "point">点</param>
	/// <param name = "triangle">三角形</param>
	/// <param name = "closest">最近接点(出力用)</param>
	static void ClosetPtPoint2Triangle(const DirectX::XMVECTOR& point, const Triangle& triangle, DirectX::XMVECTOR* closest);

	/// <summary>
	/// 球と法線付き三角形の当たりチェック
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="triangle">三角形</param>
	/// <param name="inter">交点(三角形上の最近接点)</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, DirectX::XMVECTOR* inter = nullptr);

private:

};

