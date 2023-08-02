#include "ClearScene.h"
#include "SceneManager.h"
#include "Sound.h"

void ClearScene::Initialize()
{
	end_ = std::make_unique<Sprite>();
	check_ = std::make_unique<Sprite>();
	aButton_ = std::make_unique<Sprite>();

	SpriteCommon::GetInstance()->LoadTexture("end.png");
	SpriteCommon::GetInstance()->LoadTexture("check.png");
	SpriteCommon::GetInstance()->LoadTexture("A.png");

	Vector2 titlePosition = { 0.0f,0.0f };
	Vector2 titleSize = { 1280.0f,720.0f };
	end_->Initialize(titlePosition, titleSize);

	Vector2 checkSize = { 157.0f,112.0f };
	checkPosition_.x = (WindowsAPI::kWindow_width_ / 2) - (checkSize.x / 2);
	checkPosition_.y = (WindowsAPI::kWindow_height_ / 2) - (checkSize.y);
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
	Vector2 anchorPoint = { 0.0f,0.0f };
	end_->SetAnchorPoint(anchorPoint);
	end_->matUpdate();
	aButton_->SetAnchorPoint(anchorPoint);
	aButton_->matUpdate();

	check_->SetAnchorPoint(anchorPoint);

	
	if(checkPosition_.y <= WindowsAPI::kWindow_height_ / 2)
	{
		move_.y += 0.1f;
	}

	//check_->SetMoveSpeed_(move_);
	check_->matUpdate();

	//ゲームパッドが繋がっているかどうか
	if(gamePad_->IsConnected(Player1)) {}
	//押した瞬間の判定を取る
	gamePad_->PushedButtonMoment();
	if(gamePad_->GetButtonA())
	{
		Sound::GetInstance()->PlaySoundWave("touch.wav");
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
}

void ClearScene::Draw()
{
	SpriteCommon::GetInstance()->BeforeDraw();
	SpriteCommon::GetInstance()->Update();
	end_->Draw("end.png");
	check_->Draw("check.png");
	aButton_->Draw("A.png");

}
