#include "ClearScene.h"
#include "SceneManager.h"
#include "Easing.h"
#include "MathUtillity.h"
#include "TextureManager.h"
#include "GameTimer.h"

using namespace MathUtillty;
using namespace NsEngine;

DirectXBasic* ClearScene::directXBasic_ = nullptr;
ImGuiManager* ClearScene::imGuiManager_ = nullptr;

void ClearScene::Initialize()
{
	end_ = std::make_unique<Sprite>();
	check_ = std::make_unique<Sprite>();
	aButton_ = std::make_unique<Sprite>();

	TextureManager::GetInstance()->LoadTexture("check.png");
	TextureManager::GetInstance()->LoadTexture("A.png");

	Vector2 titlePosition = { 0.0f,0.0f };;
	end_->Initialize("WhiteTex",titlePosition);

	const Vector2 kInitCheckPos = { 656,0.0f };

	checkPosition_.x = kInitCheckPos.x;
	//checkPosition_.y = 0.0f;

	check_->Initialize("check.png", kInitCheckPos);

	const Vector2 aButtonSize = { 128.0f,128.0f };
	const Vector2 aButtonPosition = { 576.0f,488.0f };
	//Vector2 aButtonPosition = { 0.0f,0.0f };
	//aButtonPosition.x = (WindowsAPI::kWindow_width_ / 2) - (aButtonSize.x / 2);
	//aButtonPosition.y = (WindowsAPI::kWindow_height_ / 2) + (aButtonSize.y);
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
	isRotateSprite_ = false;

	//アンカーポイントの設定
	const Vector2 checkAnchorPoint = { 0.5f,0.5f };
	check_->SetAnchorPoint(checkAnchorPoint);
	GameTimer::GetInstance()->ResultInitialize(WindowsAPI::kWindow_height_ / 2);
}

void ClearScene::Update()
{
	end_->matUpdate();
	aButton_->matUpdate();

	//移動処理
	const float moveResultPosY = WindowsAPI::kWindow_height_ / 3;

	if(checkPosition_.y <= moveResultPosY)
	{
		const float kMoveSpeed = 0.6f;
		move_.y += kMoveSpeed;
		checkPosition_.y += move_.y;
	}

	//回転処理
	const float kOneRotationAngle = 360.0f;
	if(rotate_ <= ToRadian(kOneRotationAngle) && isRotateSprite_ == false)
	{
		float angle = ToRadian(kOneRotationAngle);
		rotate_ -= PlayEaseInCubic(0.0f, angle, rotateTimer_, kRotateTime_);
	}

	if(rotateTimer_ >= 0)
	{
		rotateTimer_--;
	}
	else if(rotateTimer_ < 0)
	{
		rotateTimer_ = kRotateTime_;
		isRotateSprite_ = true;
	}

	check_->SetPosition(checkPosition_);
	check_->SetRotation(rotate_);
	check_->matUpdate();

	//ゲームパッドが繋がっているかどうか
	if(gamePad_->IsConnected(Player1)) {}

	if(gamePad_->PushedButtonMoment(XINPUT_GAMEPAD_A) || keys_->PushedKeyMoment(DIK_RETURN))
	{
		SoundManager::GetInstance()->Finalize();
		SceneManager::GetInstance()->ChangeScene("StageSelect");
	}

	GameTimer::GetInstance()->ResultUpdate(true, WindowsAPI::kWindow_height_ / 2);
}

void ClearScene::Draw()
{
	//描画開始
	directXBasic_->BeforeDraw();

	SpriteCommon::GetInstance()->BeforeDraw();
	end_->Draw("WhiteTex");
	check_->Draw("check.png");
	aButton_->Draw("A.png");
	GameTimer::GetInstance()->ResultDraw();

	//描画終了
	directXBasic_->AfterDraw();
}

void ClearScene::StaticInitialize()
{
	directXBasic_ = BaseScene::directXBasic_;
	imGuiManager_ = BaseScene::imGuiManager_;
}
