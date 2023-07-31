#include "TitleScene.h"
#include "SceneManager.h"
#include "Sound.h"
#include "WindowsAPI.h"

DirectXBasic* TitleScene::directXBasic_ = nullptr;
ImGuiManager* TitleScene::imGuiManager_ = nullptr;

void TitleScene::StaticInitialize(DirectXBasic* directXBasic, ImGuiManager* imGuiManager)
{
	directXBasic_ = BaseScene::directXBasic_;
	imGuiManager_ = BaseScene::imGuiManager_;

	Model::StaticInitialize(directXBasic_);
	Object3d::StaticInitialize(directXBasic_);
	Mesh::StaticInitialize(directXBasic_);
	LightGroup::StaticInitialize(directXBasic_->GetDevice().Get());
	Camera::StaticInitialize(directXBasic_);
}

void TitleScene::Initialize()
{
	lightGroup_ = LightGroup::Create();
	//3Dオブジェクトにライトをセット
	Object3d::SetLightGroup(lightGroup_);

	//3Dオブジェクト
	titleSphere_ = Object3d::Create("sphere");
	titleSphere_->SetTransform(XMFLOAT3{ 0.0f, 8.0f, 5.0f });
	titleSphere_->SetScale(XMFLOAT3{ 3.0f, 3.0f, 3.0f });

	//画像
	titleSprite_ = std::make_unique<Sprite>();
	aButtonSprite_ = std::make_unique<Sprite>();

	SpriteCommon::GetInstance()->LoadTexture("title.png");
	SpriteCommon::GetInstance()->LoadTexture("A.png");

	XMFLOAT2 titlePosition = { 0.0f,0.0f };
	const XMFLOAT2 titleSize = { 1280.0f,720.0f };
	titleSprite_->Initialize(titlePosition, titleSize);

	const XMFLOAT2 aButtonSize = { 128.0f,128.0f };
	//XMFLOAT2 aButtonPosition = { 576.0f,360.0f };
	XMFLOAT2 aButtonPosition;
	aButtonPosition.x = (WindowsAPI::kWindow_width_ / 2) - (aButtonSize.x / 2);
	aButtonPosition.y = (WindowsAPI::kWindow_height_ / 2) + (aButtonSize.y / 2);


	aButtonSprite_->Initialize(aButtonPosition, aButtonSize);

	//シェーダー読み込み
	SpriteCommon::GetInstance()->ShaderLoad();
	SpriteCommon::GetInstance()->SemiTransparent();
	//サウンド
	Sound::GetInstance()->Initialize();
	Sound::GetInstance()->LoadSoundWave("title.wav");
	Sound::GetInstance()->PlaySoundWave("title.wav");

	//カメラ
	camera_ = std::make_unique<Camera>();

	//XMFLOAT3 cameraEye = { 0.0f,0.0f,0.0f };
	//XMFLOAT3 cameraTarget = { 0.0f,0.0f,0.0f };
	//XMFLOAT3 cameraUp = { 0.0f,1.0f,0.0f };

	XMFLOAT3 cameraEye = { 30,15.5,-20 };
	XMFLOAT3 cameraTarget = { 0,5,5 };
	XMFLOAT3 cameraUp = { 0,1,0 };

	camera_->Initialize(cameraEye, cameraTarget, cameraUp);
}

void TitleScene::Update()
{
	camera_->Update();

	//アンカーポイントの設定
	XMFLOAT2 anchorPoint = { 0.0f,0.0f };
	titleSprite_->SetAnchorPoint(anchorPoint);
	titleSprite_->matUpdate();
	aButtonSprite_->SetAnchorPoint(anchorPoint);
	aButtonSprite_->matUpdate();

	titleSphere_->Update(camera_.get());

	if(keys_->PushedKeyMoment(DIK_RETURN))
	{
		Sound::GetInstance()->Finalize();
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}

void TitleScene::Draw()
{
	SpriteCommon::GetInstance()->BeforeDraw();
	SpriteCommon::GetInstance()->Update();
	titleSprite_->Draw("title.png");
	aButtonSprite_->Draw("A.png");

	Object3d::BeforeDraw();
	titleSphere_->BeforeDraw();
	titleSphere_->Draw();
}