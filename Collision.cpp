#include "Collision.h"

using namespace DirectX;

Collision::Collision()
{
}

Collision::~Collision()
{
}

//���ƕ��ʂ̓����蔻��
bool Collision::CheckSphere2Plane(const Sphere& sphere, const Plane& plane, DirectX::XMVECTOR* inter)
{
	//���W�n�̌��_���狅�̒��S���W�ւ̋���
	XMVECTOR distV = XMVector3Dot(sphere.center, plane.normal);
	//���ʂ̌��_���������Z���邱�ƂŁA���ʂƋ��̒��S�Ƃ̋������o��
	float dist = distV.m128_f32[0] - plane.distance;
	//�����̐�Βl�����a���傫����Γ������ĂȂ�
	if (fabsf(dist) > sphere.radius)
	{
		return false;
	}

	//�[����_���v�Z
	if (inter)
	{
		//���ʏ�̍ő�ړ_���A�^����_�Ƃ���
		*inter = -dist * plane.normal + sphere.center;
	}

	return true;
};

//AABB�̓����蔻��
bool Collision::CheckAABB(const AABB& box1, const AABB& box2)
{
	//�Փ˂��Ȃ������ꍇ
	if (box1.min.x > box2.max.x) return false;
	if (box1.max.x < box2.min.x) return false;
	if (box1.min.y > box2.max.y) return false;
	if (box1.max.y < box2.min.y) return false;
	if (box1.min.z > box2.max.z) return false;
	if (box1.max.z < box2.min.z) return false;

	//�Փ˂����ꍇ
	return true;
}