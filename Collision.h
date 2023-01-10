#pragma once

#include "CollisionPrimitive.h"

class Collision
{
public:
	Collision();
	~Collision();

	//‹…‚Æ•½–Ê‚ÌÕ“Ë”»’è
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, DirectX::XMVECTOR* inter = nullptr);
	//AABB‚ÆAABB‚ÌÕ“Ë”»’è
	static bool CheckAABB(const AABB& box1, const AABB& box2);

private:

};