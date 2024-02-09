#include "TouchableObject.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"

using namespace NsEngine;

std::unique_ptr<TouchableObject> TouchableObject::Create(const std::string& path, const unsigned short coliderAttribute)
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
	instance->collider_->SetAttribute(coliderAttribute);
	return instance;
}

void TouchableObject::Initialize()
{
	Object3d::Initialize();
}

void TouchableObject::AddCollider(Model* model)
{
	if(isSetedCollider_ == false)
	{
		if(objMeshCollider_ == nullptr)
		{
			//コライダーの追加
			objMeshCollider_ = std::make_unique<MeshCollider>();
		}
		
		SetCollider(objMeshCollider_.get());
		objMeshCollider_->ConstructTriangles(model);
		isSetedCollider_ = true;
	}
}

void TouchableObject::RemoveCollider()
{
	if(objMeshCollider_ && isSetedCollider_ == true)
	{
		//コリジョンマネージャーから登録を解除する
		CollisionManager::GetInstance()->RemoveCollider(collider_);
		isSetedCollider_ = false;
	}
}

void TouchableObject::ColliderUpdate()
{
	objMeshCollider_->Update();
}

void TouchableObject::Draw()
{
	Object3d::Draw();
}
