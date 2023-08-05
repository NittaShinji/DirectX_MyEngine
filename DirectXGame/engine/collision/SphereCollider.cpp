#include "SphereCollider.h"

using namespace DirectX;
using namespace MathUtillty;

void SphereCollider::Update()
{
	//���[���h�s�񂩂���W�𒊏o
	const Matrix4& matWorld = objcet3d_->GetMatWorld();

	//���̃����o�ϐ����X�V
	Sphere::center = Vector3Transform(offset_, matWorld);
	Sphere::radius = radius;
	Sphere::pos = Vector3TransformNormal(Sphere::pos, matWorld);
}