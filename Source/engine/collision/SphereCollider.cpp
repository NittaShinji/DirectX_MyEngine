#include "SphereCollider.h"

using namespace MathUtillty;

const float SphereCollider::kDefaultRadius_ = 1.0f;
const Vector3 SphereCollider::kDefaultOffset_ = {0.0f,0.0f,0.0f};

void SphereCollider::Update()
{
	//ワールド行列から座標を抽出
	const Matrix4& matWorld = objcet3d_->GetMatWorld();

	//球のメンバ変数を更新
	Sphere::center = Vector3Transform(offset_, matWorld);
	Sphere::sphereRadius = sphereRadius;
	Sphere::pos = Vector3TransformNormal(Sphere::pos, matWorld);
}