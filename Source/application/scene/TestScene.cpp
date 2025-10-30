#include "TestScene.h"
#include "SceneManager.h"
#include "WindowsAPI.h"
#include "Vector2.h"
#include "ObjectColor.h"

using namespace NsEngine;

DirectXBasic* TestScene::directXBasic_ = nullptr;
ImGuiManager* TestScene::imGuiManager_ = nullptr;

void TestScene::StaticInitialize()
{
	directXBasic_ = BaseScene::directXBasic_;
	imGuiManager_ = BaseScene::imGuiManager_;

	TextureManager::StaticInitialize(directXBasic_->GetDevice().Get());
	Sprite::StaticInitialize();
	Model::StaticInitialize(directXBasic_);
	Object3d::StaticInitialize(directXBasic_);
	Mesh::StaticInitialize(directXBasic_);
	LightGroup::StaticInitialize(directXBasic_->GetDevice().Get());
	Camera::StaticInitialize(directXBasic_);
	TextureManager::GetInstance()->Initialize();
}

void TestScene::Initialize()
{
	
}

void TestScene::Update()
{
	
}

void TestScene::Draw()
{
	//描画開始
	directXBasic_->BeforeDraw();

	SpriteCommon::GetInstance()->BeforeDraw();
	
	Object3d::BeforeDraw();

	//描画終了
	directXBasic_->AfterDraw();
}

