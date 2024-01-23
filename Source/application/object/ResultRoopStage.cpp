#include "ResultRoopStage.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"
#include "ObjectColor.h"
#include "GameSpeed.h"
#include "ObjectAttribute.h"
#include "LevelManager.h"

int32_t ResultRoopStage::roopObjectNum_;

std::unique_ptr<ResultRoopStage> ResultRoopStage::Create(const std::string& fileName, const unsigned short coliderAttribute)
{
	//オブジェクトのインスタンスを生成
	std::unique_ptr<ResultRoopStage> instance = nullptr;
	instance = std::make_unique<ResultRoopStage>();
	if(instance == nullptr)
	{
		return nullptr;
	}

	instance->Object3d::Initialize();
	instance->SetModel(fileName);
	instance->AddCollider(instance->GetModel());
	//属性を設定
	instance->collider_->SetAttribute(coliderAttribute);
	return instance;
}

void ResultRoopStage::Initialize()
{
	isRoop_ = false;
}

void ResultRoopStage::Update(Camera* camera, Vector3 playerPos, float roopArea)
{
	//プレイヤーがループオブジェクトを過ぎたら
	float distance = transform_.z - playerPos.z;
	float collisionDistance = std::fabs(distance);
	if(collisionDistance <= roopArea)
	{
		AddCollider(GetModel());
	}

	if(playerPos.z > transform_.z && distance < -roopArea)
	{
		isRoop_ = true;
		roopCount_++;
		RoopPosition();
	}

	Object3d::Update(camera);
}

void ResultRoopStage::Draw()
{
	Object3d::Draw();
}

void ResultRoopStage::Reset()
{
	isRoop_ = false;
	roopCount_ = 0;
	transform_.z = initTransFormZ_;
	Object3d::SetTransform(transform_);
	Object3d::UpdateWorldMatrix();
}

void ResultRoopStage::RoopPosition()
{
	if(isRoop_ == true)
	{
		transform_.z = initTransFormZ_ + (roopObjectNum_ * objectDistance_ * roopCount_);
		Object3d::SetTransform(transform_);
		isRoop_ = false;
	}
}
