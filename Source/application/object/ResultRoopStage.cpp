#include "ResultRoopStage.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"
#include "ObjectColor.h"
#include "GameSpeed.h"
#include "ObjectAttribute.h"
#include "LevelManager.h"

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
	Object3d::SetAttributeColor(Attribute::Goal);
	Object3d::SetColor(kNormalOBJColor);
}

void ResultRoopStage::Update(Camera* camera)
{
	Object3d::Update(camera);
}

void ResultRoopStage::Draw()
{
	Object3d::Draw();
}