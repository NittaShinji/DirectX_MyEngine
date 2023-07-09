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
//
////AABB�̓����蔻��
//bool Collision::CheckAABB(const AABB& box1, const AABB& box2)
//{
//	//�Փ˂��Ȃ������ꍇ
//	if (box1.min.x > box2.max.x) return false;
//	if (box1.max.x < box2.min.x) return false;
//	if (box1.min.y > box2.max.y) return false;
//	if (box1.max.y < box2.min.y) return false;
//	if (box1.min.z > box2.max.z) return false;
//	if (box1.max.z < box2.min.z) return false;
//
//	//�Փ˂����ꍇ
//	return true;
//}

void Collision::ClosetPtPoint2Triangle(const DirectX::XMVECTOR& point, const Triangle& triangle, DirectX::XMVECTOR* closest)
{
	// point��p0�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR p0_p1 = triangle.p1_ - triangle.p0_;
	XMVECTOR p0_p2 = triangle.p2_ - triangle.p0_;
	XMVECTOR p0_pt = point - triangle.p0_;

	XMVECTOR d1 = XMVector3Dot(p0_p1, p0_pt);
	XMVECTOR d2 = XMVector3Dot(p0_p2, p0_pt);

	if(d1.m128_f32[0] <= 0.0f && d2.m128_f32[0] <= 0.0f)
	{
		// p0���ŋߖT
		*closest = triangle.p0_;
		return;
	}

	// point��p1�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR p1_pt = point - triangle.p1_;

	XMVECTOR d3 = XMVector3Dot(p0_p1, p1_pt);
	XMVECTOR d4 = XMVector3Dot(p0_p2, p1_pt);

	if(d3.m128_f32[0] >= 0.0f && d4.m128_f32[0] <= d3.m128_f32[0])
	{
		// p1���ŋߖT
		*closest = triangle.p1_;
		return;
	}

	// point��p0_p1�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p1��ɑ΂���ˉe��Ԃ�
	float vc = d1.m128_f32[0] * d4.m128_f32[0] - d3.m128_f32[0] * d2.m128_f32[0];
	if(vc <= 0.0f && d1.m128_f32[0] >= 0.0f && d3.m128_f32[0] <= 0.0f)
	{
		float v = d1.m128_f32[0] / (d1.m128_f32[0] - d3.m128_f32[0]);
		*closest = triangle.p0_ + v * p0_p1;
		return;
	}

	// point��p2�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR p2_pt = point - triangle.p2_;

	XMVECTOR d5 = XMVector3Dot(p0_p1, p2_pt);
	XMVECTOR d6 = XMVector3Dot(p0_p2, p2_pt);
	if(d6.m128_f32[0] >= 0.0f && d5.m128_f32[0] <= d6.m128_f32[0])
	{
		*closest = triangle.p2_;
		return;
	}

	// point��p0_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p2��ɑ΂���ˉe��Ԃ�
	float vb = d5.m128_f32[0] * d2.m128_f32[0] - d1.m128_f32[0] * d6.m128_f32[0];
	if(vb <= 0.0f && d2.m128_f32[0] >= 0.0f && d6.m128_f32[0] <= 0.0f)
	{
		float w = d2.m128_f32[0] / (d2.m128_f32[0] - d6.m128_f32[0]);
		*closest = triangle.p0_ + w * p0_p2;
		return;
	}

	// point��p1_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p1_p2��ɑ΂���ˉe��Ԃ�
	float va = d3.m128_f32[0] * d6.m128_f32[0] - d5.m128_f32[0] * d4.m128_f32[0];
	if(va <= 0.0f && (d4.m128_f32[0] - d3.m128_f32[0]) >= 0.0f && (d5.m128_f32[0] - d6.m128_f32[0]) >= 0.0f)
	{
		float w = (d4.m128_f32[0] - d3.m128_f32[0]) / ((d4.m128_f32[0] - d3.m128_f32[0]) + (d5.m128_f32[0] - d6.m128_f32[0]));
		*closest = triangle.p1_ + w * (triangle.p2_ - triangle.p1_);
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p0_ + p0_p1 * v + p0_p2 * w;
}

bool Collision::CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, DirectX::XMVECTOR* inter)
{
	XMVECTOR p;
	//���̒��S�ɑ΂���ŋߐړ_�ł���O�p�`��ɂ���_p��������
	ClosetPtPoint2Triangle(sphere.center, triangle, &p);
	//�_p�Ƌ��̒��S�̍����x�N�g��
	XMVECTOR v = p - sphere.center;

	//�����̓������߂�
	//(�����x�N�g�����m�̓��ς͎O�����̒藝�̃��[�g�����̎��ƈ�v����)
	v = XMVector3Dot(v, v);

	//���ƎO�p�`�̋��������a�ȉ��Ȃ瓖�����Ă��Ȃ�
	if(v.m128_f32[0] > sphere.radius * sphere.radius)
	{
		return false;
	}

	//�^����_���v�Z
	if(inter)
	{
		//�O�p�`��̍ŋߐړ_p���^����_�Ƃ���
		*inter = p;
	}

	return true;
}

bool Collision::CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, DirectX::XMVECTOR* inter)
{
	//���Ƌ��̋���
	float distance = (sphereB.pos.x - sphereA.pos.x) * (sphereB.pos.x - sphereA.pos.x) +
		(sphereB.pos.y - sphereA.pos.y) * (sphereB.pos.y - sphereA.pos.y) +
		(sphereB.pos.z - sphereA.pos.z) * (sphereB.pos.z - sphereA.pos.z);

	/*distance = (sphereB.center.m128_f32[0] - sphereA.center.m128_f32[0]) * (sphereB.center.m128_f32[0] - sphereA.center.m128_f32[0]) +
		(sphereB.center.m128_f32[1] - sphereA.center.m128_f32[1]) * (sphereB.center.m128_f32[1] - sphereA.center.m128_f32[1]) +
		(sphereB.center.m128_f32[2] - sphereA.center.m128_f32[2]) * (sphereB.center.m128_f32[2] - sphereA.center.m128_f32[2]);*/

	//���a�Ɣ��a�𑫂�������
	float addRadius = (sphereA.radius + sphereB.radius) * (sphereA.radius + sphereB.radius);

	//�Փ�(���a���ɓ����Ă��邩�ǂ���)
	if(distance <= addRadius)
	{
		return true;
	}

	return false;
}

bool Collision::CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance, DirectX::XMVECTOR* inter)
{
	const float epsilon = 1.0e-5f; //�덷�z���p�̔����Ȓl
	//�ʖ@���ƃ��C�̕����x�N�g���̓���
	float d1 = XMVector3Dot(plane.normal, ray.dir).m128_f32[0];
	//���ʂɂ͓�����Ȃ�
	if(d1 > -epsilon) { return false; }
	//�n�_�ƌ��_�̋���(���ʂ̖@������)
	//�ʖ@���ƃ��C�̎��_���W(�ʒu�x�N�g��)�̓���
	float d2 = XMVector3Dot(plane.normal, ray.start).m128_f32[0];
	//�n�_�ƕ��ʂ̋���(���ʂ̖@������)
	float dist = d2 - plane.distance;
	//�n�_�ƕ��ʂ̋���(���C����)
	float t = dist / -d1;
	//��_���n�_�����ɂ���̂ŁA������Ȃ�
	if(t < 0) return false;
	//��������������
	if(distance) { *distance = t; }
	//��_���v�Z
	if(inter) { *inter = ray.start + t * ray.dir; }
	return true;
}

bool Collision::CheckRay2Triangle(const Ray& ray, const Triangle& triangle, float* distance, DirectX::XMVECTOR* inter)
{
	//�O�p�`������Ă��镽�ʂ��Z�o
	Plane plane;
	XMVECTOR interPlane;
	plane.normal = triangle.normal_;
	plane.distance = XMVector3Dot(triangle.normal_, triangle.p0_).m128_f32[0];
	//���C�ƕ��ʂ��������Ă��Ȃ���΁A�������Ă��Ȃ�
	if(!CheckRay2Plane(ray, plane, distance, &interPlane)) { return false; }
	//���C�ƕ��ʂ̌�_���O�p�`�̓����ɂ��邩����
	const float epsilon = 1.0e-5f; //�덷�z���p�̔����Ȓl

	XMVECTOR m;
	//��p0_p1�ɂ���
	XMVECTOR pt_p0 = triangle.p0_ - interPlane;
	XMVECTOR p0_p1 = triangle.p1_ - triangle.p0_;
	m = XMVector3Cross(pt_p0, p0_p1);
	//�ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
	if(XMVector3Dot(m, triangle.normal_).m128_f32[0] < -epsilon) { return false; }

	//��p1,p2�ɂ���
	XMVECTOR pt_p1 = triangle.p1_ - interPlane;
	XMVECTOR p1_p2 = triangle.p2_ - triangle.p1_;
	m = XMVector3Cross(pt_p1, p1_p2);
	//�ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
	if(XMVector3Dot(m, triangle.normal_).m128_f32[0] < -epsilon) { return false; }

	//��p2,p0�ɂ���
	XMVECTOR pt_p2 = triangle.p2_ - interPlane;
	XMVECTOR p2_p0 = triangle.p0_ - triangle.p2_;
	m = XMVector3Cross(pt_p2, p2_p0);
	//�ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
	if(XMVector3Dot(m, triangle.normal_).m128_f32[0] < -epsilon) { return false; }

	//�����Ȃ̂œ������Ă���
	if(inter)
	{
		*inter = interPlane;
	}

	return true;
}

bool Collision::CheckRay2Sphere(const Ray& ray, const Sphere& sphere, float* distance, DirectX::XMVECTOR* inter)
{
	XMVECTOR m = ray.start - sphere.center;
	float b = XMVector3Dot(m, ray.dir).m128_f32[0];
	float c = XMVector3Dot(m, m).m128_f32[0] - sphere.radius * sphere.radius;
	//lay�̎n�_��sphere�̊O���ɂ���(c > 0)�Alay��sphere���痣��Ă��������������Ă���ꍇ(b > 0)�A������Ȃ�
	if(c > 0.0f && b > 0.0f) { return false;}

	float discr = b * b - c;
	//���̔��ʎ��̓��C�������O��Ă��邱�ƂɈ�v
	if(discr < 0.0f) { return false; }

	//���C�͋��ƌ������Ă���
	//��������ŏ��̒lt���v�Z
	float t = b - sqrtf(discr);
	//t�����ł���ꍇ�A���C�͋��̓�������J�n���Ă���̂�t���[���ɃN�����v
	if(t < 0) t = 0.0f;
	if(distance) { *distance = t; }
	if(inter) { *inter = ray.start + t * ray.dir;}

	return true;
}
