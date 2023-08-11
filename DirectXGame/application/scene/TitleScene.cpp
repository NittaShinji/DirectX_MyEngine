#include "TitleScene.h"
#include "SceneManager.h"
#include "Sound.h"
#include "WindowsAPI.h"
#include "Vector2.h"
#include "Easing.h"

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
	titleSphere_->SetScale(Vector3{ 3.0f, 3.0f, 3.0f });

	//画像
	titleSprite_ = std::make_unique<Sprite>();
	clickSprite_= std::make_unique<Sprite>();
	aButtonSprite_ = std::make_unique<Sprite>();
	bButtonSprite_ = std::make_unique<Sprite>();
	backGroundSprite_ = std::make_unique<Sprite>();

	SpriteCommon::GetInstance()->LoadTexture("TitleFont.png");
	SpriteCommon::GetInstance()->LoadTexture("A.png");
	SpriteCommon::GetInstance()->LoadTexture("B.png");
	SpriteCommon::GetInstance()->LoadTexture("click.png");

	Vector2 backGroundPosition = { 0.0f,0.0f };
	const int32_t backGroundWidth = 1280;
	const int32_t backGroundHeight = 720;
	const Vector2 backGroundSize = { backGroundWidth,backGroundHeight};

	SpriteCommon::GetInstance()->TexMapping(backGroundWidth, backGroundHeight, Vector4(1.0f, 1.0f, 1.0f, 1.0f), "WhiteTex");

	Vector2 titlePosition = { 400.0f,33.0f };
	const Vector2 titleSize = { 480.0f,103.0f };
	titleSprite_->Initialize(titlePosition, titleSize);

	const Vector2 clickButtonSize = { 128.0f,128.0f };
	Vector2 clickButtonPosition;
	clickButtonPosition.x = (WindowsAPI::kWindow_width_  / 2) - (clickButtonSize.x / 2);
	clickButtonPosition.y = (WindowsAPI::kWindow_height_ / 2) + (clickButtonSize.y) + (clickButtonSize.y / 3);

	const Vector2 aButtonSize = { 128.0f,128.0f };
	Vector2 aButtonPosition;
	aButtonPosition.x = (WindowsAPI::kWindow_width_) - (aButtonSize.x * 2);
	aButtonPosition.y = (WindowsAPI::kWindow_height_ / 2) + (aButtonSize.y) + (aButtonSize.y / 3);

	const Vector2 bButtonSize = { 128.0f,128.0f };
	Vector2 bButtonPosition;
	bButtonPosition.x = (WindowsAPI::kWindow_width_) - (bButtonSize.x);
	bButtonPosition.y = (WindowsAPI::kWindow_height_ / 2) + (bButtonSize.y) + (bButtonSize.y / 3);

	clickSprite_->Initialize(clickButtonPosition, clickButtonSize);
	aButtonSprite_->Initialize(aButtonPosition, aButtonSize);
	bButtonSprite_->Initialize(bButtonPosition, bButtonSize);
	backGroundSprite_->Initialize(backGroundPosition, backGroundSize);

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

	Vector3 cameraEye = { 30,15.5,-20 };

	Vector3 cameraTarget = { 0,5,5 };
	
	Vector3 cameraUp = { 0,1,0 };

	camera_->Initialize(cameraEye, cameraTarget, cameraUp);

	isChangeScene_ = false;
	isChangeColor_ = false;
	rotateAcc_ = 0.0f;
	moveRotate_ = 0.0f;

	bool isJump = false;
	Vector3 jumpVec = {0.0f,0.0f,0.0f};
	float jumpAcc = 0.0f;
}

void TitleScene::Update()
{
	camera_->Update();

	titleSprite_->matUpdate();
	clickSprite_->matUpdate();
	aButtonSprite_->matUpdate();
	bButtonSprite_->matUpdate();
	backGroundSprite_->matUpdate();

	titleSphere_->Update(camera_.get());

	//回転処理
	if(isChangeScene_ == false)
	{
		float angle = ToRadian(360.0f);
		sphereRotate.y -= PlayEaseIn(rotateTimer_, 0.0f, angle, kRotateTime_);
		titleSphere_->SetRotation(sphereRotate);
	}

	if(rotateTimer_ >= 0)
	{
		rotateTimer_--;
	}
	else
	{
		rotateTimer_ = kRotateTime_;

		isJump = true;
		moveTimer_ = kActionTime_;

		if(isChangeColor_ == false)
		{
			isChangeColor_ = true;
		}
		else
		{
			isChangeColor_ = false;
		}
	}

	if(isJump == true)
	{
		spherPos_.y += PlayEaseIn(moveTimer_, 0.0, 1.0, kActionTime_);
		titleSphere_->SetTransform(spherPos_);
	}
	
	//移動処理
	if(moveTimer_ > 0)
	{
		moveTimer_--;
	}
	else
	{
		isJump = false;
	}

	if(isJump == false)
	{
		if(spherPos_.y >= 5.0f)
		{
			spherPos_.y -= easeInCubic(0.75f);
		}
		titleSphere_->SetTransform(spherPos_);
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
	if(changeColorTimer_ >= 0)
	{
		changeColorTimer_--;

	}
	else
	{
		changeColorTimer_ = kChangeColorTime_;
	}

	if(isChangeColor_ == false)
	{
		titleSphere_->SetColorFlag(true);
		titleSphere_->SetColor(Vector3(1.0f, 0.4f, 0.7f));
	}
	else
	{
		titleSphere_->SetColorFlag(true);
		titleSphere_->SetColor(Vector3(1.0f, 0.469f, 0.0f));
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
		changeWhiteTimer_--;
		titleSphere_->SetColorFlag(true);
		titleSphere_->SetColor(Vector3(1.0f, 1.0f, 1.0f));
		if(changeWhiteTimer_ <= 0)
		{
			Sound::GetInstance()->Finalize();
			//SceneManager::GetInstance()->ChangeScene("GAME");
			SceneManager::GetInstance()->ChangeScene("StageSelect");
		}
	}
}

void TitleScene::Draw()
{
	SpriteCommon::GetInstance()->BeforeDraw();
	SpriteCommon::GetInstance()->Update();
	//titleSprite_->Draw("WhiteTex");
	backGroundSprite_->Draw("WhiteTex");
	titleSprite_->Draw("TitleFont.png");

	clickSprite_->Draw("click.png");
	aButtonSprite_->Draw("A.png");
	bButtonSprite_->Draw("B.png");

	Object3d::BeforeDraw();
	titleSphere_->BeforeDraw();
	titleSphere_->Draw();
}