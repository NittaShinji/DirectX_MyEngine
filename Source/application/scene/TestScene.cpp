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
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();

	lightGroup_ = LightGroup::Create();
	//3Dオブジェクトにライトをセット
	Object3d::SetLightGroup(lightGroup_);

	//テスト3Dオブジェクト
	const std::string sphere = "Sphere";
	Model::Load(sphere);
	testObj = Object3d::Create("Sphere");
	
	objPos_ = Vector3{ 0.0f,5.0f,5.0f };
	testObj->SetTransform(objPos_);
	testObj->SetScale(Vector3{ 3.0f, 3.0f, 3.0f });

	//画像
	crownSprite = std::make_unique<Sprite>();
	crownSprite->Initialize("crown.png", Vector2(0, 0));

	//シェーダー読み込み
	SpriteCommon::GetInstance()->ShaderLoad();
	//ブレンドモード(半透明)ここでグラフィックスパイプラインを生成している
	SpriteCommon::GetInstance()->SemiTransparent();
}

void TestScene::Update()
{
	camera_->Update();

	crownSprite->matUpdate();

	testObj->Update(camera_.get());
}

void TestScene::Draw()
{
	//描画開始
	directXBasic_->BeforeDraw();
	

	// spriteCommmonでパイプラインを生成していないので通らない
	SpriteCommon::GetInstance()->BeforeDraw();

	crownSprite->Draw("crown.png");
	
	// 上のObject3dの静的初期化でパイプラインを生成しているので通る
	Object3d::BeforeDraw();
	testObj->Draw();

	//描画終了
	directXBasic_->AfterDraw();
}

