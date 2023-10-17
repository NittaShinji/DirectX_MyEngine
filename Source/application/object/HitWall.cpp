#include "HitWall.h"
#include "BaseCollider.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"

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
	return instance;
}

void HitWall::OnCollision(const CollisionInfo& info)
{
	BaseCollider* baseCollider = info.collider;

	info.object->GetAttributeColor();

	//球と当たった時
	if(baseCollider->GetShapeType() == COLLISIONSHAPE_SPHERE)
	{
		//プレイヤーが加速している場合
		if(isPlayerAccelerating_ == true)
		{
			//壁の属性を対応する属性色に変更し、壊れるように
			if(info.object->GetAttributeColor() == pink)
			{
				Object3d::SetAttributeColor(Attribute::pink);
			}
			else
			{
				Object3d::SetAttributeColor(Attribute::yellow);
			}
			
			isBreak_ = true;
		}
	}
}

void HitWall::Initialize()
{
	isBreak_ = false;
	Object3d::SetAttributeColor(Attribute::black);
	Object3d::SetColor(Vector3(0.78f, 0.78f, 0.78f));
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
