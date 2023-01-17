#include "Collision.h"

using namespace DirectX;

Collision::Collision()
{
}

Collision::~Collision()
{
}

//球と平面の当たり判定
bool Collision::CheckSphere2Plane(const Sphere& sphere, const Plane& plane, DirectX::XMVECTOR* inter)
{
	//座標系の原点から球の中心座標への距離
	XMVECTOR distV = XMVector3Dot(sphere.center, plane.normal);
	//平面の原点距離を減算することで、平面と球の中心との距離が出る
	float dist = distV.m128_f32[0] - plane.distance;
	//距離の絶対値が半径より大きければ当たってない
	if (fabsf(dist) > sphere.radius)
	{
		return false;
	}

	//擬似交点を計算
	if (inter)
	{
		//平面上の最大接点を、疑似交点とする
		*inter = -dist * plane.normal + sphere.center;
	}

	return true;
};

//AABBの当たり判定
bool Collision::CheckAABB(const AABB& box1, const AABB& box2)
{
	//衝突しなかった場合
	if (box1.min.x > box2.max.x) return false;
	if (box1.max.x < box2.min.x) return false;
	if (box1.min.y > box2.max.y) return false;
	if (box1.max.y < box2.min.y) return false;
	if (box1.min.z > box2.max.z) return false;
	if (box1.max.z < box2.min.z) return false;

	//衝突した場合
	return true;
}