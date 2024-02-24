#include "TitleScene.h"
#include "SceneManager.h"
#include "WindowsAPI.h"
#include "Vector2.h"
#include "Easing.h"
#include "ObjectColor.h"

using namespace NsEngine;

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
	const std::string sphere = "sphere";
	Model::Load(sphere);
	titleSphere_ = Object3d::Create("sphere");
	spherPos_ = kSpherInitPos_;
	titleSphere_->SetTransform(spherPos_);
	titleSphere_->SetScale(kSpherInitScale_);

	//画像
	titleSprite_ = std::make_unique<Sprite>();
	clickSprite_ = std::make_unique<Sprite>();
	aButtonSprite_ = std::make_unique<Sprite>();
	bButtonSprite_ = std::make_unique<Sprite>();
	backGroundSprite_ = std::make_unique<Sprite>();
	
	TextureManager::GetInstance()->LoadTexture("titleFont.png");
	TextureManager::GetInstance()->LoadTexture("A.png");
	TextureManager::GetInstance()->LoadTexture("B.png");
	TextureManager::GetInstance()->LoadTexture("click.png");
	
	Vector2 backGroundPosition = { 0.0f,0.0f };
	const int32_t backGroundWidth = 1280;
	const int32_t backGroundHeight = 720;
	
	const Vector4 whiteColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	TextureManager::GetInstance()->TexMapping(backGroundWidth, backGroundHeight, whiteColor, "WhiteTex");

	const Vector2 titlePosition = { 400.0f,33.0f };
	titleSprite_->Initialize("titleFont.png",titlePosition);

	const Vector2 clickButtonSize = { TextureManager::GetInstance()->GetTexSize("click.png")};
	const Vector2 clickButtonPosition = {576.0f,530.0f};
	
	const Vector2 aButtonSize = { TextureManager::GetInstance()->GetTexSize("A.png") };
	const Vector2 aButtonPosition = { 1024.0f,530.0f };
	
	const Vector2 bButtonSize = { TextureManager::GetInstance()->GetTexSize("B.png") };
	const Vector2 bButtonPosition = {1152.0f,530.0f};
	
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
		const float kOneCircleRotate = 360.0f;
		const float kStartEasingPos = 0.0f;
		float angle = ToRadian(kOneCircleRotate);
		sphereRotate.y -= PlayEaseInCubic(kStartEasingPos, angle, rotateTimer_, kRotateTime_);
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

			isSphereJump_ = true;
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

	if(isSphereJump_ == true && sceneAnimeTimer_ == 0)
	{
		const float kStartEasingPos = 0.0f;
		const float kEndEasingPos = 1.0f;
		spherPos_.y += PlayEaseInCubic(kStartEasingPos, kEndEasingPos, moveTimer_, kActionTime_);
		titleSphere_->SetTransform(spherPos_);
	}

	//移動処理
	if(moveTimer_ > 0)
	{
		moveTimer_--;
	}
	else
	{
		isSphereJump_ = false;
	}

	if(isSphereJump_ == false)
	{
		const float kSphereLimitPosY = 5.0f;
		if(spherPos_.y >= kSphereLimitPosY)
		{
			const float kSphereMoveEasingY = 0.75f;
			spherPos_.y -= EaseInCubic(kSphereMoveEasingY);
		}
		titleSphere_->SetTransform(spherPos_);
	}

	const int32_t half = 2;
	if(moveTimer_ > (kActionTime_ / half))
	{
		isSphereDown_ = false;
		isSphereUp_ = true;
		sphereMoveValue_.y = 0;
	}
	else
	{
		isSphereDown_ = true;
		isSphereUp_ = false;
		sphereMoveValue_.y = 0;
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
		titleSphere_->SetColor(kTitlePinkOBJColor);
	}
	else
	{
		titleSphere_->SetColorFlag(true);
		titleSphere_->SetColor(kYellowOBJColor);
	}

	//フラグの方向に球を移動させる
	if(isSphereDown_ == true && isSphereUp_ == false)
	{
		sphereMoveValue_.y -= kSphereMoveY_;
	}
	else if(isSphereUp_ == true && isSphereDown_ == false)
	{
		sphereMoveValue_.y += kSphereMoveY_;
	}

	titleSphere_->SetTransform(spherPos_);

	//ゲームパッドが繋がっているかどうか
	if(gamePad_->IsConnected(Player1)) {}

	if(gamePad_->HasPushedButton(XINPUT_GAMEPAD_A) || keys_->PushedKeyMoment(DIK_RETURN))
	{
		touchSound_->PlaySoundWave(false);
		isChangeScene_ = true;
	}

	if(isChangeScene_ == true)
	{
		SceneAnimation();

		if(isFinishAnimetion == true)
		{
			changeWhiteTimer_--;
			//色を白色に変更
			const Vector3 whiteColor = { 1.0f,1.0f,1.0f };
			titleSphere_->SetColorFlag(true);
			titleSphere_->SetColor(whiteColor);
			//背景画像を黒色に
			if(backGroundChangEasing_.time < backGroundChangEasing_.totalTime)
			{
				backGroundChangEasing_.time++;
				backChangeColor.x = PlayEaseOutQuint(backGroundChangEasing_);
				backChangeColor.y = PlayEaseOutQuint(backGroundChangEasing_);
				backChangeColor.z = PlayEaseOutQuint(backGroundChangEasing_);
				backGroundSprite_->SetColor(backChangeColor);
			}

			if(changeWhiteTimer_ <= 0 && backGroundChangEasing_.time >= backGroundChangEasing_.totalTime)
			{
				if(changeWhiteTimer_ <= 0)
				{
					SoundManager::GetInstance()->Finalize();
					SceneManager::GetInstance()->ChangeScene("GAME");
				}
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
	titleSprite_->Draw("titleFont.png");

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
