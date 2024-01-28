#include "MirrorOBJ.h"
#include "ObjectColor.h"
#include "ObjectAttribute.h"

std::unique_ptr<MirrorOBJ> MirrorOBJ::Create(Object3d* object)
{
	//3Dオブジェクトのインスタンスを生成
	std::unique_ptr<MirrorOBJ> instance = std::make_unique<MirrorOBJ>();

	if(instance == nullptr)
	{
		return nullptr;
	}
	
	instance->object3d_ = object;
	instance->Initialize();
	instance->SetModel(object->GetModel()->GetName());

	return instance;
}

MirrorOBJ::MirrorOBJ()
{
}

MirrorOBJ::~MirrorOBJ()
{
}

void MirrorOBJ::Initialize()
{
	Object3d::Initialize();
	SetColorFlag(true);
	SetMirrorInfo();
}

void MirrorOBJ::Update(Camera* camera)
{
	Object3d::Update(camera);
	SetMirrorInfo();
}

void MirrorOBJ::Draw()
{
	Object3d::Draw();
}

void MirrorOBJ::SetMirrorInfo()
{
	Vector3 objTransForm = object3d_->GetTransform();

	//オブジェクトの位置を代入
	scale_ = object3d_->GetScale();
	rotation_ = object3d_->GetRotation();
	transform_ = object3d_->GetTransform();
	//鏡面反射用の座標に設定
	transform_.y = -transform_.y - kMirrorDistance_;

	//オブジェクトの色に応じて、色を変更
	if(object3d_->GetAttributeColor() == Attribute::pink)
	{
		SetColor(kTitlePinkOBJColor);
	}
	else if(object3d_->GetAttributeColor() == Attribute::yellow)
	{
		SetColor(kYellowOBJColor);
	}
	else
	{
		SetColor(kBlackOBJColor);
	}

	//行列の更新
	Object3d::SetTransform(transform_);
}
