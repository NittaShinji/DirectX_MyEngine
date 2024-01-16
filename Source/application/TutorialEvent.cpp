#include "TutorialEvent.h"
#include "GameSpeed.h"
#include "Player.h"

void TutorialEvent::Initialzie(Player* player)
{
	player_ = player;

	const Vector2 kUiSize = { 128.0f,128.0f };

	//ジャンプイベント設定
	const float kStartJumpEventPos_ = 155;
	const float kEndJumpEventPos_ = 190;
	jumpEvent_ = std::make_unique<Event>();
	jumpEvent_->Initialzie(kStartJumpEventPos_, kEndJumpEventPos_);
	jumpEvent_->AddSprite("A.png", Vector2(586.0f, 296.0f), kUiSize);

	const float kStartHighJumpEventPos_ = 250;
	const float kEndHighJumpEventPos_ = 390;
	hightJumpEvent_ = std::make_unique<Event>();
	hightJumpEvent_->Initialzie(kStartHighJumpEventPos_, kEndHighJumpEventPos_);
	hightJumpEvent_->AddSprite("A.png", Vector2(586.0f,296.0f), kUiSize);

	const float kStartChangeColorEventPos_ = 505;
	const float kEndChangeColorEventPos_ = 525;
	changeColorEvent_ = std::make_unique<Event>();
	changeColorEvent_->Initialzie(kStartChangeColorEventPos_, kEndChangeColorEventPos_);
	changeColorEvent_->AddSprite("B.png", Vector2(586.0f, 296.0f), kUiSize);

	const float kStartChangeAirEventPos_ = 590;
	const float kEndChangeAirEventPos_ = 880;
	const Vector2 kChangeColorSpriteSize = {64.0f,64.0f};

	changeInAirEvent_ = std::make_unique<Event>();
	changeInAirEvent_->Initialzie(kStartChangeAirEventPos_, kEndChangeAirEventPos_);
	changeInAirEvent_->AddSprite("arrow.png", Vector2(640.0f, 296.0f), kChangeColorSpriteSize);
}

void TutorialEvent::Update()
{
	float playerPosZ = player_->GetTransform().z;

	if(player_->GetIsDead() == false)
	{
		jumpEvent_->Update(playerPosZ, GameSpeed::SpeedMode::SLOW, XINPUT_GAMEPAD_A, DIK_SPACE);
		hightJumpEvent_->Update(playerPosZ, GameSpeed::SpeedMode::NORMAL, XINPUT_GAMEPAD_A, DIK_SPACE);
		changeColorEvent_->Update(playerPosZ, GameSpeed::SpeedMode::SLOW, XINPUT_GAMEPAD_B, DIK_RETURN);
		changeInAirEvent_->Update(playerPosZ, GameSpeed::SpeedMode::NORMAL, XINPUT_GAMEPAD_B, DIK_RETURN);
	}
}

void TutorialEvent::Draw()
{
	if(player_->GetIsDead() == false)
	{
		jumpEvent_->Draw();
		hightJumpEvent_->Draw();
		changeColorEvent_->Draw();
		changeInAirEvent_->Draw();
	}
}
