#include "HitWall.h"
#include "BaseCollider.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"
#include "ObjectColor.h"

using namespace NsEngine;

std::unique_ptr<HitWall> HitWall::Create(const std::string& fileName)
{
	//オブジェクトのインスタンスを生成
	std::unique_ptr<HitWall> instance = nullptr;
	instance = std::make_unique<HitWall>();
	if(instance == nullptr)
	{
		return nullptr;
	}

	instance->Object3d::Initialize();
	instance->SetModel(fileName);
	instance->SetIsBreak(false);
	instance->AddCollider(instance->GetModel());
	//属性を設定
	instance->collider_->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	instance->isPlayerAccelerating_ = false;
	return instance;
}

void HitWall::OnCollision(const CollisionInfo& info)
{
	BaseCollider* baseCollider = info.collider;

	info.object->GetAttributeColor();

	//プレイヤー(球)と当たった時
	if(baseCollider->GetShapeType() == COLLISIONSHAPE_SPHERE)
	{
		//プレイヤーが加速している場合
		if(isPlayerAccelerating_ == true)
		{			
			isBreak_ = true;
		}
	}
}

void HitWall::Initialize()
{
	isBreak_ = false;
	Object3d::SetAttributeColor(Attribute::black);
	Object3d::SetColor(kNormalOBJColor);
}

void HitWall::Update(Camera* camera, bool isPlayerAccelerating)
{
	Object3d::Update(camera);
	isPlayerAccelerating_ = isPlayerAccelerating;
}

void HitWall::Draw()
{	
	if(isBreak_ == false)
	{
		Object3d::Draw();
	}
}
