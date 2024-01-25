#include "ResultSprite.h"
#include "GameTimer.h"

void ResultSprite::Load()
{

}

void ResultSprite::Initialize()
{
	const int32_t backGorundWidth = 1200;
	const int32_t backGorundHeight = 400;
	const Vector4 grayColor = { 0.8f, 0.8f, 0.8f, 0.6f };

	TextureManager::GetInstance()->TexMapping(backGorundWidth, backGorundHeight, grayColor, "gray");

	backGroundSprite_ = std::make_unique<Sprite>();
	backGroundSprite_->Initialize("gray", Vector2(40.0f, -backGorundHeight));
	resultInEasing_.startPos = -backGorundHeight;
	resultInEasing_.endDistance = backGorundHeight + (WindowsAPI::kWindow_height_ / 2 - backGorundHeight / 2);
	resultOutEasing_.endDistance = -(backGorundHeight + (WindowsAPI::kWindow_height_ / 2 - backGorundHeight / 2));

	isFinishOutEasing_ = true;
	isFinishInEasing_ = false;

	//ゲームタイマー
	GameTimer::GetInstance()->InGameInitialize();
}

void ResultSprite::Update()
{

}

void ResultSprite::Draw()
{
	backGroundSprite_->Draw("gray");
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
	Vector2 backGroundPosition_ = backGroundSprite_->GetPosition();

	if(isFinishInEasing_ == false)
	{
		resultInEasing_.time++;
		if(resultInEasing_.time > 0 && resultInEasing_.time <= resultInEasing_.totalTime)
		{
			backGroundPosition_.y = PlayEaseOutQuint(resultInEasing_);
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
		resultOutEasing_.startPos = backGroundPosition_.y;
	}

	backGroundSprite_->SetPosition(backGroundPosition_);
	backGroundSprite_->matUpdate();
}

void ResultSprite::Reset()
{
	const float backGorundX = 40.0f;
	const float backGorundY = 400.0f;

	backGroundSprite_->SetPosition(Vector2(backGorundX, -backGorundY));
	backGroundSprite_->matUpdate();

	isFinishOutEasing_ = true;
	isFinishInEasing_ = false;

	resultOutEasing_.time = 0.0f;
	resultInEasing_.time = 0.0f;

	resultInEasing_.startPos = -backGorundY;
	resultInEasing_.endDistance = backGorundY + (WindowsAPI::kWindow_height_ / 2 - backGorundY / 2);
	resultOutEasing_.endDistance = -(backGorundY + (WindowsAPI::kWindow_height_ / 2 - backGorundY / 2));
}

