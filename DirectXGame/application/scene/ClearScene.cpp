#include "ClearScene.h"
#include "SceneManager.h"
#include "Sound.h"
#include "Easing.h"
#include "MathUtillity.h"

using namespace MathUtillty;

void ClearScene::Initialize()
{
	end_ = std::make_unique<Sprite>();
	check_ = std::make_unique<Sprite>();
	aButton_ = std::make_unique<Sprite>();

	//SpriteCommon::GetInstance()->LoadTexture("end.png");
	SpriteCommon::GetInstance()->LoadTexture("check.png");
	SpriteCommon::GetInstance()->LoadTexture("A.png");

	Vector2 titlePosition = { 0.0f,0.0f };
	Vector2 titleSize = { 1280.0f,720.0f };
	end_->Initialize(titlePosition, titleSize);

	Vector2 checkSize = { 157.0f,112.0f };
	checkPosition_.x = (WindowsAPI::kWindow_width_ / 2) - (checkSize.x / 2);
	//checkPosition_.y = (WindowsAPI::kWindow_height_ / 2) - (checkSize.y);
	checkPosition_.y = 0.0f;

	check_->Initialize(checkPosition_, checkSize);

	Vector2 aButtonSize = { 128.0f,128.0f };
	Vector2 aButtonPosition = { 0.0f,0.0f };
	aButtonPosition.x = (WindowsAPI::kWindow_width_ / 2) - (aButtonSize.x / 2);
	aButtonPosition.y = (WindowsAPI::kWindow_height_ / 2) + (aButtonSize.y);
	aButton_->Initialize(aButtonPosition, aButtonSize);

	//シェーダー読み込み
	SpriteCommon::GetInstance()->ShaderLoad();
	SpriteCommon::GetInstance()->SemiTransparent();

	//コントローラ
	gamePad_ = std::make_unique<GamePad>();
	gamePad_->Initialzie(Player1);

	//サウンド
	Sound::GetInstance()->Initialize();
	Sound::GetInstance()->LoadSoundWave("clear.wav");
	Sound::GetInstance()->PlaySoundWave("clear.wav");

	Sound::GetInstance()->LoadSoundWave("touch.wav");

	//変数
	move_ = {0.0f,0.0f};
}

void ClearScene::Update()
{
	//アンカーポイントの設定
	end_->matUpdate();
	aButton_->matUpdate();

	
	if(checkPosition_.y <= (WindowsAPI::kWindow_height_ / 2) - 112.0f)
	{
		float angle = ToRadian(360.0f);
		//rotate_ -= PlayEaseIn(rotateTimer_, 0.0f, angle, kRotateTime_);
		move_.y += 0.1f;
		checkPosition_.y += move_.y;
	}

	if(rotateTimer_ >= 0)
	{
		rotateTimer_--;
	}
	else
	{
	}

	check_->SetPosition(checkPosition_);
	check_->SetRotation(ToRadian(180.0f));
	check_->matUpdate();

	//ゲームパッドが繋がっているかどうか
	if(gamePad_->IsConnected(Player1)) {}
	//押した瞬間の判定を取る
	gamePad_->PushedButtonMoment();
	if(gamePad_->GetButtonA() || keys_->PushedKeyMoment(DIK_RETURN))
	{
		Sound::GetInstance()->PlaySoundWave("touch.wav");
		//SceneManager::GetInstance()->ChangeScene("TITLE");
		SceneManager::GetInstance()->ChangeScene("StageSelect");
	}
}

void ClearScene::Draw()
{
	SpriteCommon::GetInstance()->BeforeDraw();
	SpriteCommon::GetInstance()->Update();
	end_->Update();
	check_->Update();
	aButton_->Update();

	end_->Draw("WhiteTex");
	check_->Draw("check.png");
	aButton_->Draw("A.png");

}
