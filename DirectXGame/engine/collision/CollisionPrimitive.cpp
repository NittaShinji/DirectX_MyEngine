#include "CollisionPrimitive.h"

using namespace DirectX;

void Triangle::ComputeNormal()
{
	XMVECTOR p0_p1 = p1_ - p0_;
	XMVECTOR p0_p2 = p2_ - p0_;

	//外積により、2辺に垂直なベクトルを算出する
	normal_ = XMVector3Cross(p0_p1, p0_p2);
	normal_ = XMVector3Normalize(normal_);
}
