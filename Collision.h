#pragma once

#include "CollisionPrimitive.h"

class Collision
{
public:
	Collision();
	~Collision();

	//���ƕ��ʂ̏Փ˔���
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, DirectX::XMVECTOR* inter = nullptr);
	//AABB��AABB�̏Փ˔���
	static bool CheckAABB(const AABB& box1, const AABB& box2);

private:

};