#include "CollisionPrimitive.h"

using namespace DirectX;

void Triangle::ComputeNormal()
{
	XMVECTOR p0_p1 = p1_ - p0_;
	XMVECTOR p0_p2 = p2_ - p0_;

	//�O�ςɂ��A2�ӂɐ����ȃx�N�g�����Z�o����
	normal_ = XMVector3Cross(p0_p1, p0_p2);
	normal_ = XMVector3Normalize(normal_);
}
