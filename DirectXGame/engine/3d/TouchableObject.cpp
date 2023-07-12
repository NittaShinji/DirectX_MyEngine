#include "TouchableObject.h"
#include "MeshCollider.h"

//TouchableObject* TouchableObject::Create(const std::string& path)
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

	return instance;
}

void TouchableObject::Initialize()
{
	Object3d::Initialize();
}

void TouchableObject::AddCollider(Model* model)
{
	//コライダーの追加
	MeshCollider* collider = new MeshCollider;
	SetCollider(collider);
	collider->ConstructTriangles(model);
}

void TouchableObject::Draw()
{
	Object3d::Draw();
}
