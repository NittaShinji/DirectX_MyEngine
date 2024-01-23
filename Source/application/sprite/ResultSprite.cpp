#include "ResultSprite.h"
#include "GameTimer.h"

void ResultSprite::Load()
{
	const int32_t selectWidth = 1000;
	const int32_t selectHeight = 400;
	const Vector4 grayColor = { 0.8f, 0.8f, 0.8f, 0.6f };

	TextureManager::GetInstance()->TexMapping(selectWidth, selectHeight, grayColor, "gray");
}

void ResultSprite::Initialize()
{
	const int32_t backGorundWidth = 1200;
	const int32_t backGorundHeight = 400;
	const Vector4 grayColor = { 0.8f, 0.8f, 0.8f, 0.6f };

	TextureManager::GetInstance()->TexMapping(backGorundWidth, backGorundHeight, grayColor, "gray");

	backGroundSprite_ = std::make_unique<Sprite>();
	backGroundSprite_->Initialize("gray", Vector2(40.0f, -backGorundHeight));
	resultEasing_.startPos = -backGorundHeight;
	resultEasing_.endDistance = backGorundHeight + (WindowsAPI::kWindow_height_ / 2 - backGorundHeight / 2);

	isFinishBackGroundEasing_ = false;

	//ゲームタイマー
	GameTimer::GetInstance()->InGameInitialize();
}

void ResultSprite::Update()
{
	Vector2 backGroundPosition_ = backGroundSprite_->GetPosition();

	if(isFinishBackGroundEasing_ == false)
	{
		resultEasing_.time++;
		if(resultEasing_.time > 0 && resultEasing_.time <= resultEasing_.totalTime)
		{
			backGroundPosition_.y = PlayEaseOutQuint(resultEasing_);
		}
		else
		{
			resultEasing_.time = 0;
			isFinishBackGroundEasing_ = true;
		}
	}

	backGroundSprite_->SetPosition(backGroundPosition_);
	backGroundSprite_->matUpdate();
}

void ResultSprite::Draw()
{
	backGroundSprite_->Draw("gray");
}

