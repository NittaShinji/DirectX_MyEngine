#include "GoalOBJ.h"
#include "BaseCollider.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"

std::unique_ptr<GoalOBJ> GoalOBJ::Create(const std::string& fileName, const unsigned short coliderAttribute)
{
	//オブジェクトのインスタンスを生成
	std::unique_ptr<GoalOBJ> instance = nullptr;
	instance = std::make_unique<GoalOBJ>();
	if(instance == nullptr)
	{
		return nullptr;
	}

	instance->Object3d::Initialize();
	instance->SetModel(fileName);
	instance->SetIsBreak(false);
	instance->AddCollider(instance->GetModel());
	//属性を設定
	instance->collider_->SetAttribute(coliderAttribute);
	instance->isPlayerAccelerating_ = false;
	return instance;
}

void GoalOBJ::OnCollision(const CollisionInfo& info)
{
	BaseCollider* baseCollider = info.collider;

	info.object->GetAttributeColor();

	//球と当たった時
	if(baseCollider->GetShapeType() == COLLISIONSHAPE_SPHERE)
	{
		isBreak_ = true;
		////プレイヤーが加速している場合
		//if(isPlayerAccelerating_ == true)
		//{
		//	
		//}
	}
}

void GoalOBJ::Initialize()
{
	isStartGoalStagin_ = false;
	isBreak_ = false;
	Object3d::SetAttributeColor(Attribute::Goal);
	Object3d::SetColor(Vector3(0.78f, 0.78f, 0.78f));
}

void GoalOBJ::Update(Camera* camera, Vector3 playerPos)
{
	Object3d::Update(camera);

	if(playerPos.z < transform_.z)
	{
		float result = transform_.z - playerPos.z;
		if(result <= kStagingStartDistance_)
		{
			isStartGoalStagin_ = true;
		}
	}
}

void GoalOBJ::Draw()
{
	if(isBreak_ == false)
	{
		Object3d::Draw();
	}
}
