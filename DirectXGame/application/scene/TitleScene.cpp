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

	Sprite::StaticInitialize();
	Model::StaticInitialize(directXBasic_);
	Object3d::StaticInitialize(directXBasic_);
	Mesh::StaticInitialize(directXBasic_);
	LightGroup::StaticInitialize(directXBasic_->GetDevice().Get());
	Camera::StaticInitialize(directXBasic_);
}

void TitleScene::Initialize()
{
	gamePad_ = std::make_unique<GamePad>();
	gamePad_->Initialzie(Player1);

	lightGroup_ = LightGroup::Create();
	//3Dオブジェクトにライトをセット
	Object3d::SetLightGroup(lightGroup_);

	//3Dオブジェクト
	titleSphere_ = Object3d::Create("sphere");
	spherPos_.x = 0.0f;
	spherPos_.y = 5.0f;
	spherPos_.z = 5.0f;

	titleSphere_->SetTransform(spherPos_);
	titleSphere_->SetScale(XMFLOAT3{ 3.0f, 3.0f, 3.0f });

	//画像
	titleSprite_ = std::make_unique<Sprite>();
	clickSprite_= std::make_unique<Sprite>();
	aButtonSprite_ = std::make_unique<Sprite>();
	bButtonSprite_ = std::make_unique<Sprite>();

	SpriteCommon::GetInstance()->LoadTexture("title.png");
	SpriteCommon::GetInstance()->LoadTexture("A.png");
	SpriteCommon::GetInstance()->LoadTexture("B.png");
	SpriteCommon::GetInstance()->LoadTexture("click.png");

	XMFLOAT2 titlePosition = { 0.0f,0.0f };
	const XMFLOAT2 titleSize = { 1280.0f,720.0f };
	titleSprite_->Initialize(titlePosition, titleSize);

	const XMFLOAT2 clickButtonSize = { 128.0f,128.0f };
	XMFLOAT2 clickButtonPosition;
	clickButtonPosition.x = (WindowsAPI::kWindow_width_  / 2) - (clickButtonSize.x / 2);
	clickButtonPosition.y = (WindowsAPI::kWindow_height_ / 2) + (clickButtonSize.y) + (clickButtonSize.y / 3);

	const XMFLOAT2 aButtonSize = { 128.0f,128.0f };
	XMFLOAT2 aButtonPosition;
	aButtonPosition.x = (WindowsAPI::kWindow_width_) - (aButtonSize.x * 2);
	aButtonPosition.y = (WindowsAPI::kWindow_height_ / 2) + (aButtonSize.y) + (aButtonSize.y / 3);

	const XMFLOAT2 bButtonSize = { 128.0f,128.0f };
	XMFLOAT2 bButtonPosition;
	bButtonPosition.x = (WindowsAPI::kWindow_width_) - (bButtonSize.x);
	bButtonPosition.y = (WindowsAPI::kWindow_height_ / 2) + (bButtonSize.y) + (bButtonSize.y / 3);

	clickSprite_->Initialize(clickButtonPosition, clickButtonSize);
	aButtonSprite_->Initialize(aButtonPosition, aButtonSize);
	bButtonSprite_->Initialize(bButtonPosition, bButtonSize);

	//シェーダー読み込み
	SpriteCommon::GetInstance()->ShaderLoad();
	SpriteCommon::GetInstance()->SemiTransparent();
	//サウンド
	Sound::GetInstance()->Initialize();
	Sound::GetInstance()->LoadSoundWave("title.wav");
	Sound::GetInstance()->LoadSoundWave("touch.wav");

	Sound::GetInstance()->PlaySoundWave("title.wav");

	//カメラ
	camera_ = std::make_unique<Camera>();

	XMFLOAT3 cameraEye = { 30,15.5,-20 };
	XMFLOAT3 cameraTarget = { 0,5,5 };
	XMFLOAT3 cameraUp = { 0,1,0 };

	camera_->Initialize(cameraEye, cameraTarget, cameraUp);

	isChangeScene_ = false;
}

void TitleScene::Update()
{
	camera_->Update();

	//アンカーポイントの設定
	XMFLOAT2 anchorPoint = { 0.0f,0.0f };
	titleSprite_->SetAnchorPoint(anchorPoint);
	titleSprite_->matUpdate();
	clickSprite_->SetAnchorPoint(anchorPoint);
	clickSprite_->matUpdate();
	aButtonSprite_->SetAnchorPoint(anchorPoint);
	aButtonSprite_->matUpdate();
	bButtonSprite_->SetAnchorPoint(anchorPoint);
	bButtonSprite_->matUpdate();

	titleSphere_->Update(camera_.get());

	//回転処理
	if(isChangeScene_ == false)
	{
		sphereRotate.y -= 0.01f;
		titleSphere_->SetRotation(sphereRotate);
	}
	
	//移動処理
	if(moveTimer_ >= 0)
	{
		moveTimer_--;
	}
	else
	{
		moveTimer_ = kActionTime_;
	}

	if(moveTimer_ > (kActionTime_ / 2))
	{
		isDown_ = false;
		isUp_ = true;
		move_.y = 0;
	}
	else
	{
		isDown_ = true;
		isUp_ = false;
		move_.y = 0;
	}

	//色を変える処理
	if(rotateTimer_ >= 0)
	{
		rotateTimer_--;
	}
	else
	{
		rotateTimer_ = kRoateTime_;
	}

	if(rotateTimer_ > (kRoateTime_ / 2))
	{
		titleSphere_->SetColorFlag(true);
		titleSphere_->SetColor(XMFLOAT3(1.0f, 0.4f, 0.7f));
	}
	else
	{
		titleSphere_->SetColorFlag(true);
		titleSphere_->SetColor(XMFLOAT3(1.0f, 0.469f, 0.0f));
	}

	if(isDown_ == true && isUp_ == false)
	{
		move_.y -= 0.3f;
	}
	else if(isUp_ == true && isDown_ == false)
	{
		move_.y += 0.3f;
	}

	titleSphere_->SetTransform(spherPos_);

	//ゲームパッドが繋がっているかどうか
	if(gamePad_->IsConnected(Player1)) {}
	//押した瞬間の判定を取る
	gamePad_->PushedButtonMoment();

	if(gamePad_->GetButtonA() || keys_->PushedKeyMoment(DIK_RETURN))
	{
		Sound::GetInstance()->PlaySoundWave("touch.wav");
		isChangeScene_ = true;	
	}

	if(isChangeScene_ == true)
	{
		changeTimer_--;
		titleSphere_->SetColorFlag(true);
		titleSphere_->SetColor(XMFLOAT3(1.0f, 1.0f, 1.0f));
		if(changeTimer_ <= 0)
		{
			Sound::GetInstance()->Finalize();
			SceneManager::GetInstance()->ChangeScene("GAME");
		}
	}
}

void TitleScene::Draw()
{
	SpriteCommon::GetInstance()->BeforeDraw();
	SpriteCommon::GetInstance()->Update();
	titleSprite_->Draw("title.png");
	clickSprite_->Draw("click.png");
	aButtonSprite_->Draw("A.png");
	bButtonSprite_->Draw("B.png");


	Object3d::BeforeDraw();
	titleSphere_->BeforeDraw();
	titleSphere_->Draw();
}