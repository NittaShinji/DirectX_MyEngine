#include "StageBlock.h"
#include "BaseCollider.h"
#include "MeshCollider.h"
#include "ObjectAttribute.h"
#include "CollisionAttribute.h"

using namespace NsEngine;

std::unique_ptr<StageBlock> StageBlock::Create(const std::string& fileName, const unsigned short coliderAttribute)
{
	//オブジェクトのインスタンスを生成
	std::unique_ptr<StageBlock> instance = nullptr;
	instance = std::make_unique<StageBlock>();
	if(instance == nullptr)
	{
		return nullptr;
	}

	instance->Object3d::Initialize();
	instance->SetModel(fileName);
	instance->AddCollider(instance->GetModel());
	//当たり判定用の属性を設定
	instance->collider_->SetAttribute(coliderAttribute);
	return instance;
}
