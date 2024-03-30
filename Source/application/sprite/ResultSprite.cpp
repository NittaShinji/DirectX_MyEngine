#include "ResultSprite.h"
#include "GameTimer.h"

using namespace NsEngine;

void ResultSprite::StaticInitialize()
{
	const int32_t backGorundWidth = 1200;
	const int32_t backGorundHeight = 400;
	const Vector4 grayColor = { 0.8f, 0.8f, 0.8f, 0.6f };
	TextureManager::GetInstance()->TexMapping(backGorundWidth, backGorundHeight, grayColor, "gray");
}

void ResultSprite::Initialize()
{
	//画像情報を初期化
	const int32_t backGorundHeight = 400;
	backGroundSprite_ = std::make_unique<Sprite>();
	backGroundSprite_->Initialize("gray", Vector2(40.0f, -backGorundHeight));
	aButtonSprite_ = std::make_unique<Sprite>();
	aButtonSprite_->Initialize("aButton.png", Vector2(576.0f, -backGorundHeight));
	aButtonSprite_->SetColor(Vector4(1.0f,1.0f,1.0f,0.8f));
	stageSprite_ = std::make_unique<Sprite>();
	stageSprite_->Initialize("stage.png", Vector2(700.0f, -backGorundHeight));
	nextSprite_ = std::make_unique<Sprite>();
	nextSprite_->Initialize("next.png", Vector2(270.0f, -backGorundHeight));

	//メンバ変数を初期化
	isFinishOutEasing_ = true;
	isFinishInEasing_ = false;
	resultInEasing_.startPos = -backGorundHeight;
	resultInEasing_.endDistance = backGorundHeight + (WindowsAPI::kWindow_height_ / 2 - backGorundHeight / 2);
	resultOutEasing_.endDistance = -(backGorundHeight + (WindowsAPI::kWindow_height_ / 2 - backGorundHeight / 2));

	//ゲームタイマー
	GameTimer::GetInstance()->InGameInitialize();
}

void ResultSprite::Update()
{
	backGroundSprite_->matUpdate();
	aButtonSprite_->matUpdate();
	stageSprite_->matUpdate();
	nextSprite_->matUpdate();
}

void ResultSprite::Draw()
{
	SpriteCommon::GetInstance()->BeforeDraw();
	backGroundSprite_->Draw("gray");
	aButtonSprite_->Draw("aButton.png");
	stageSprite_->Draw("stage.png");
	nextSprite_->Draw("next.png");
}

void ResultSprite::ComeOutOffScreen()
{
	if(isFinishOutEasing_ == false)
	{
		resultOutEasing_.time++;
		if(resultOutEasing_.time > 0 && resultOutEasing_.time <= resultOutEasing_.totalTime)
		{
			Vector2 backGroundPosition_ = backGroundSprite_->GetPosition();
			backGroundPosition_.y = PlayEaseOutQuint(resultOutEasing_);
			backGroundSprite_->SetPosition(backGroundPosition_);
			backGroundSprite_->matUpdate();

			//Aボタンを背景イージングに追従するよう動かす
			float aButtonPositionY;
			aButtonPositionY = backGroundPosition_.y;
			Vector4 aButtonChangeColor = aButtonSprite_->GetColor();
			const float changeColorSpeed = 0.025f;
			aButtonChangeColor.x -= changeColorSpeed;
			aButtonChangeColor.y -= changeColorSpeed;
			aButtonChangeColor.z -= changeColorSpeed;
			aButtonSprite_->SetColor(aButtonChangeColor);
			aButtonSprite_->SetPosition(Vector2(aButtonSprite_->GetPosition().x, aButtonPositionY + 256));
			aButtonSprite_->matUpdate();

			//表示文字画像を背景イージングに追従するよう動かす
			float stringSpritePositionY;
			stringSpritePositionY = backGroundPosition_.y;
			nextSprite_->SetPosition(Vector2(nextSprite_->GetPosition().x, stringSpritePositionY + 131));
			nextSprite_->matUpdate();
			stageSprite_->SetPosition(Vector2(stageSprite_->GetPosition().x, stringSpritePositionY + 131));
			stageSprite_->matUpdate();
		}
		else
		{
			resultOutEasing_.time = 0;
			isFinishOutEasing_ = true;
		}
	}
	else
	{
		Reset();
	}
}

void ResultSprite::ComeInScreen()
{
	Vector2 backGroundPosition = backGroundSprite_->GetPosition();

	if(isFinishInEasing_ == false)
	{
		//画面中央に半透明なグレーの背景画像をイージングで動かす
		resultInEasing_.time++;
		if(resultInEasing_.time > 0 && resultInEasing_.time <= resultInEasing_.totalTime)
		{
			backGroundPosition.y = PlayEaseOutQuint(resultInEasing_);
		}
		else
		{
			resultInEasing_.time = 0;
			isFinishInEasing_ = true;
		}
	}
	else
	{
		//画面外に出るイージングの開始位置を設定
		resultOutEasing_.startPos = backGroundPosition.y;
	}

	//背景画像を更新
	backGroundSprite_->SetPosition(backGroundPosition);
	backGroundSprite_->matUpdate();

	//Aボタンを背景イージングに追従するよう動かす
	float aButtonPositionY;
	aButtonPositionY = backGroundPosition.y;
	aButtonSprite_->SetPosition(Vector2(aButtonSprite_->GetPosition().x, aButtonPositionY + 256));
	aButtonSprite_->matUpdate();

	//表示文字画像を背景イージングに追従するよう動かす
	float stringSpritePositionY;
	stringSpritePositionY = backGroundPosition.y;
	nextSprite_->SetPosition(Vector2(nextSprite_->GetPosition().x, stringSpritePositionY + 131));
	nextSprite_->matUpdate();
	stageSprite_->SetPosition(Vector2(stageSprite_->GetPosition().x, stringSpritePositionY + 131));
	stageSprite_->matUpdate();
}

void ResultSprite::Reset()
{
	//画像情報を初期時にリセット
	//透明背景画像
	const float kBackGorundX = 40.0f;
	const float kBackGorundY = 400.0f;
	backGroundSprite_->SetPosition(Vector2(kBackGorundX, -kBackGorundY));
	backGroundSprite_->matUpdate();
	//Aボタン画像
	const float kaBuutonX = 576.0f;
	const float kaButtonY = 400.0f;
	aButtonSprite_->SetPosition(Vector2(kaBuutonX, -kaButtonY));
	aButtonSprite_->matUpdate();
	const Vector4 aButtonDefaultColor = { 1.0f,1.0f,1.0f,0.8f };
	aButtonSprite_->SetColor(aButtonDefaultColor);
	//NEXT文字画像
	const float kNextBuutonX = 270.0f;
	const float kNextButtonY = 400.0f;
	nextSprite_->SetPosition(Vector2(kNextBuutonX, -kNextButtonY));
	nextSprite_->matUpdate();
	////STAGE文字画像
	const float kStageBuutonX = 700.0f;
	const float kStageButtonY = 400.0f;
	stageSprite_->SetPosition(Vector2(kStageBuutonX, -kStageButtonY));
	stageSprite_->matUpdate();
	
	//イージングなどのメンバー変数を初期化
	isFinishOutEasing_ = true;
	isFinishInEasing_ = false;
	resultOutEasing_.time = 0.0f;
	resultInEasing_.time = 0.0f;
	resultInEasing_.startPos = -kBackGorundY;
	resultInEasing_.endDistance = kBackGorundY + (WindowsAPI::kWindow_height_ / 2 - kBackGorundY / 2);
	resultOutEasing_.endDistance = -(kBackGorundY + (WindowsAPI::kWindow_height_ / 2 - kBackGorundY / 2));
}

