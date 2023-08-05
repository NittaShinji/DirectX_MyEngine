#include "SphereCollider.h"

using namespace DirectX;
using namespace MathUtillty;

void SphereCollider::Update()
{
	//ワールド行列から座標を抽出
	const Matrix4& matWorld = objcet3d_->GetMatWorld();

	//球のメンバ変数を更新
	//Sphere::center = matWorld.r[3] + offset_;
	Sphere::center = Vector3Transform(offset_, matWorld);
	Sphere::radius = radius;
	Sphere::pos = Vector3TransformNormal(Sphere::pos, matWorld);

	//const XMMATRIX& matWorld = objcet3d_->GetMatWorld();

	////球のメンバ変数を更新
	//Sphere::center = matWorld.r[3] + offset;
	//Sphere::radius = radius;
	//Sphere::pos = { matWorld.r[3].m128_f32[0],matWorld.r[3].m128_f32[1],matWorld.r[3].m128_f32[2] };

}