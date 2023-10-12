#include "HitWall.h"
#include "BaseCollider.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"

std::unique_ptr<HitWall> HitWall::Create(const std::string& fileName)
{
	//オブジェクトのインスタンスを生成
	std::unique_ptr<HitWall> instance = nullptr;
	instance = std::make_unique<HitWall>();
	if(instance == nullptr)
	{
		return nullptr;
	}

	instance->Initialize();
	instance->SetModel(fileName);
	instance->SetIsBreak(false);
	instance->AddCollider(instance->GetModel());
	//属性を設定
	instance->collider_->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	return instance;
}

void HitWall::OnCollision(const CollisionInfo& info)
{
	BaseCollider* baseCollider = info.collider;

	//球と当たった時
	if(baseCollider->GetShapeType() == COLLISIONSHAPE_SPHERE)
	{
		isBreak_ = true;
	}
}

void HitWall::Draw()
{	
	if(isBreak_ == false)
	{
		Object3d::Draw();
	}
}
