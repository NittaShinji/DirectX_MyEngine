#include "TouchableObject.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"

std::unique_ptr<TouchableObject> TouchableObject::Create(const std::string& path)
{
	//オブジェクトのインスタンスを生成
	std::unique_ptr<TouchableObject> instance = nullptr;
	instance = std::make_unique<TouchableObject>();
	if(instance == nullptr)
	{
		return nullptr;
	}

	instance->Initialize();
	instance->SetModel(path);
	instance->AddCollider(instance->GetModel());
	//属性を設定
	instance->collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	return instance;
}

void TouchableObject::Initialize()
{
	Object3d::Initialize();
}

void TouchableObject::AddCollider(Model* model)
{
	//コライダーの追加
	objMeshCollider_ = std::make_unique<MeshCollider>();
	SetCollider(objMeshCollider_.get());
	objMeshCollider_->ConstructTriangles(model);
}

void TouchableObject::ColliderUpdate()
{
	objMeshCollider_->Update();
}

void TouchableObject::Draw()
{
	Object3d::Draw();
}
