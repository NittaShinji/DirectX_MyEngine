#include "TitleScene.h"
#include "SceneManager.h"
#include "WindowsAPI.h"
#include "Vector2.h"
#include "Easing.h"

DirectXBasic* TitleScene::directXBasic_ = nullptr;
ImGuiManager* TitleScene::imGuiManager_ = nullptr;

void TitleScene::StaticInitialize()
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
}

void TitleScene::Initialize()
{
	TextureManager::GetInstance()->Initialize();
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
	clickSprite_ = std::make_unique<Sprite>();
	aButtonSprite_ = std::make_unique<Sprite>();
	bButtonSprite_ = std::make_unique<Sprite>();
	backGroundSprite_ = std::make_unique<Sprite>();
	
	TextureManager::GetInstance()->LoadTexture("TitleFont.png");
	TextureManager::GetInstance()->LoadTexture("A.png");
	TextureManager::GetInstance()->LoadTexture("B.png");
	TextureManager::GetInstance()->LoadTexture("click.png");
	
	Vector2 backGroundPosition = { 0.0f,0.0f };
	const int32_t backGroundWidth = 1280;
	const int32_t backGroundHeight = 720;

	TextureManager::GetInstance()->TexMapping(backGroundWidth, backGroundHeight, Vector4(1.0f, 1.0f, 1.0f, 1.0f), "WhiteTex");

	Vector2 titlePosition = { 400.0f,33.0f };
	titleSprite_->Initialize("TitleFont.png",titlePosition);

	const Vector2 clickButtonSize = { TextureManager::GetInstance()->GetTexSize("click.png")};
	Vector2 clickButtonPosition;
	clickButtonPosition.x = (WindowsAPI::kWindow_width_ / 2) - (clickButtonSize.x / 2);
	clickButtonPosition.y = (WindowsAPI::kWindow_height_ / 2) + (clickButtonSize.y) + (clickButtonSize.y / 3);

	const Vector2 aButtonSize = { TextureManager::GetInstance()->GetTexSize("A.png") };
	Vector2 aButtonPosition;
	aButtonPosition.x = (WindowsAPI::kWindow_width_)-(aButtonSize.x * 2);
	aButtonPosition.y = (WindowsAPI::kWindow_height_ / 2) + (aButtonSize.y) + (aButtonSize.y / 3);

	const Vector2 bButtonSize = { TextureManager::GetInstance()->GetTexSize("B.png") };
	Vector2 bButtonPosition;
	bButtonPosition.x = (WindowsAPI::kWindow_width_)-(bButtonSize.x);
	bButtonPosition.y = (WindowsAPI::kWindow_height_ / 2) + (bButtonSize.y) + (bButtonSize.y / 3);

	aButtonSprite_->Initialize("A.png",aButtonPosition);
	bButtonSprite_->Initialize("B.png",bButtonPosition);

	clickSprite_->Initialize("click.png",clickButtonPosition);
	backGroundSprite_->Initialize("WhiteTex",backGroundPosition);
	
	//シェーダー読み込み
	SpriteCommon::GetInstance()->ShaderLoad();
	SpriteCommon::GetInstance()->SemiTransparent();
	//サウンド
	SoundManager::GetInstance()->Initialize();
	SoundManager::GetInstance()->LoadSoundWave("title.wav");
	SoundManager::GetInstance()->LoadSoundWave("touch.wav");
	titleSound_ = std::make_unique<Sound>();
	touchSound_ = std::make_unique<Sound>();
	titleSound_->Initialize("title.wav");
	touchSound_->Initialize("touch.wav");
	titleSound_->PlaySoundWave(true);

	//カメラ
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();

	isChangeScene_ = false;
	isChangeColor_ = false;
	rotateAcc_ = 0.0f;
	moveRotate_ = 0.0f;
	isFinishAnimetion = false;
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
		sphereRotate.y -= PlayEaseInCubic(0.0f, angle, rotateTimer_, kRotateTime_);
		titleSphere_->SetRotation(sphereRotate);
	}

	if(rotateTimer_ > 0)
	{
		rotateTimer_--;
	}
	else
	{
		//シーン切替アニメーションが始まっていなかったら
		if(sceneAnimeTimer_ == 0)
		{
			rotateTimer_ = kRotateTime_;

			isJump_ = true;
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
	}

	if(isJump_ == true && sceneAnimeTimer_ == 0)
	{
		spherPos_.y += PlayEaseInCubic(0.0, 1.0, moveTimer_, kActionTime_);
		titleSphere_->SetTransform(spherPos_);
	}

	//移動処理
	if(moveTimer_ > 0)
	{
		moveTimer_--;
	}
	else
	{
		isJump_ = false;
	}

	if(isJump_ == false)
	{
		if(spherPos_.y >= 5.0f)
		{
			spherPos_.y -= EaseInCubic(0.75f);
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
	if(changecolorTimer__ >= 0)
	{
		changecolorTimer__--;

	}
	else
	{
		changecolorTimer__ = kChangeColorTime_;
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
		//Sound::GetInstance()->PlaySoundWave("touch.wav",false);
		//Sound::GetInstance()->PauseSound("Resources/Sound/title.wav");
		touchSound_->PlaySoundWave(false);
		isChangeScene_ = true;
	}

	if(isChangeScene_ == true)
	{
		SceneAnimation();

		if(isFinishAnimetion == true)
		{
			changeWhiteTimer_--;
			titleSphere_->SetColorFlag(true);
			titleSphere_->SetColor(Vector3(1.0f, 1.0f, 1.0f));
			if(changeWhiteTimer_ <= 0)
			{
				//Sound::GetInstance()->PauseSound("title.wav");
				/*Sound::GetInstance()->Finalize();*/
				SoundManager::GetInstance()->Finalize();
				SceneManager::GetInstance()->ChangeScene("StageSelect");
			}
		}
	}
}

void TitleScene::Draw()
{
	//描画開始
	directXBasic_->BeforeDraw();

	SpriteCommon::GetInstance()->BeforeDraw();
	backGroundSprite_->Draw("WhiteTex");
	titleSprite_->Draw("TitleFont.png");

	clickSprite_->Draw("click.png");
	aButtonSprite_->Draw("A.png");
	bButtonSprite_->Draw("B.png");
	
	Object3d::BeforeDraw();
	titleSphere_->BeforeDraw();
	titleSphere_->Draw();

	//描画終了
	directXBasic_->AfterDraw();
}

//シーン遷移アニメーション
void TitleScene::SceneAnimation()
{
	if(sceneAnimeTimer_ < kSceneAnimeTime_)
	{
		sceneAnimeTimer_++;
	}
	else
	{
		sceneAnimeTimer_ = 0;
		isFinishAnimetion = true;
	}

	//変化量
	float x = sceneAnimeTimer_ / kSceneAnimeTime_;
	animationMoveVec2.y += EaseOutQuint(x);

	//画像を動かす処理
	titleSprite_->MovePos(animationMoveVec2);
	clickSprite_->MovePos(animationMoveVec2);
	aButtonSprite_->MovePos(animationMoveVec2);
	bButtonSprite_->MovePos(animationMoveVec2);

	//オブジェクトを動かす処理
	titleSphere_->MovePos(Vector3(animationMoveVec2.x, -animationMoveVec2.y, 0.0f));
}
