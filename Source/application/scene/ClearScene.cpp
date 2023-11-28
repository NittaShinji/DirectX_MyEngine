#include "ClearScene.h"
#include "SceneManager.h"
#include "Easing.h"
#include "MathUtillity.h"
#include "TextureManager.h"

using namespace MathUtillty;

DirectXBasic* ClearScene::directXBasic_ = nullptr;
ImGuiManager* ClearScene::imGuiManager_ = nullptr;

void ClearScene::Initialize()
{
	end_ = std::make_unique<Sprite>();
	check_ = std::make_unique<Sprite>();
	aButton_ = std::make_unique<Sprite>();

	//SpriteCommon::GetInstance()->LoadTexture("end.png");
	TextureManager::GetInstance()->LoadTexture("check.png");
	TextureManager::GetInstance()->LoadTexture("A.png");

	Vector2 titlePosition = { 0.0f,0.0f };;
	end_->Initialize("WhiteTex",titlePosition);

	checkPosition_.x = (WindowsAPI::kWindow_width_ / 2);
	checkPosition_.y = 0.0f;

	check_->Initialize("check.png",checkPosition_);

	Vector2 aButtonSize = { 128.0f,128.0f };
	Vector2 aButtonPosition = { 0.0f,0.0f };
	aButtonPosition.x = (WindowsAPI::kWindow_width_ / 2) - (aButtonSize.x / 2);
	aButtonPosition.y = (WindowsAPI::kWindow_height_ / 2) + (aButtonSize.y);
	aButton_->Initialize("A.png",aButtonPosition);

	//シェーダー読み込み
	SpriteCommon::GetInstance()->ShaderLoad();
	SpriteCommon::GetInstance()->SemiTransparent();

	//コントローラ
	gamePad_ = std::make_unique<GamePad>();
	gamePad_->Initialzie(Player1);

	//サウンド
	SoundManager::GetInstance()->Initialize();
	SoundManager::GetInstance()->LoadSoundWave("clear.wav");
	checkSound_ = std::make_unique<Sound>();
	checkSound_->Initialize("clear.wav");
	checkSound_->PlaySoundWave(false);

	//変数
	move_ = { 0.0f,0.0f };

	//アンカーポイントの設定
	Vector2 checkAnchorPoint = { 0.5f,0.5f };
	check_->SetAnchorPoint(checkAnchorPoint);
}

void ClearScene::Update()
{
	end_->matUpdate();
	aButton_->matUpdate();


	if(checkPosition_.y <= (WindowsAPI::kWindow_height_ / 2))
	{
		move_.y += 0.6f;
		checkPosition_.y += move_.y;
	}

	if(rotate_ <= ToRadian(360.0f))
	{
		float angle = ToRadian(360.0f);
		rotate_ -= PlayEaseIn(0.0f, angle, rotateTimer_, kRotateTime_);
	}

	if(rotateTimer_ >= 0)
	{
		rotateTimer_--;
	}
	else
	{
	}

	check_->SetPosition(checkPosition_);
	check_->SetRotation(rotate_);
	check_->matUpdate();

	//ゲームパッドが繋がっているかどうか
	if(gamePad_->IsConnected(Player1)) {}
	//押した瞬間の判定を取る
	gamePad_->PushedButtonMoment();
	if(gamePad_->GetButtonA() || keys_->PushedKeyMoment(DIK_RETURN))
	{
		SoundManager::GetInstance()->Finalize();
		SceneManager::GetInstance()->ChangeScene("StageSelect");
	}
}

void ClearScene::Draw()
{
	//描画開始
	directXBasic_->BeforeDraw();

	SpriteCommon::GetInstance()->BeforeDraw();
	SpriteCommon::GetInstance()->Update();
	end_->Update();
	check_->Update();
	aButton_->Update();

	end_->Draw("WhiteTex");
	check_->Draw("check.png");
	aButton_->Draw("A.png");

	//描画終了
	directXBasic_->AfterDraw();
}

void ClearScene::StaticInitialize()
{
	directXBasic_ = BaseScene::directXBasic_;
	imGuiManager_ = BaseScene::imGuiManager_;
}
