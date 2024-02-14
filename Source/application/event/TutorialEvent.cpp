#include "TutorialEvent.h"
#include "GameSpeed.h"
#include "Player.h"

void TutorialEvent::Initialzie(Player* player)
{
	player_ = player;

	const Vector2 kUiSize = { 128.0f,128.0f };

	//ジャンプイベント設定
	const float kStartJumpEventPos_ = 75;
	const float kEndJumpEventPos_ = 110;
	jumpEvent_ = std::make_unique<Event>();
	jumpEvent_->Initialzie(kStartJumpEventPos_, kEndJumpEventPos_);
	jumpEvent_->AddSprite("A.png", Vector2(586.0f, 296.0f), kUiSize);

	const float kStartHighJumpEventPos_ = 185;
	const float kEndHighJumpEventPos_ = 320;
	hightJumpEvent_ = std::make_unique<Event>();
	hightJumpEvent_->Initialzie(kStartHighJumpEventPos_, kEndHighJumpEventPos_);
	hightJumpEvent_->AddSprite("A.png", Vector2(586.0f,296.0f), kUiSize);

	const float kStartChangeColorEventPos_ = 650;
	const float kEndChangeColorEventPos_ = 670;
	changeColorEvent_ = std::make_unique<Event>();
	changeColorEvent_->Initialzie(kStartChangeColorEventPos_, kEndChangeColorEventPos_);
	changeColorEvent_->AddSprite("B.png", Vector2(586.0f, 296.0f), kUiSize);

	const float kStartChangeAirEventPos_ = 700;
	const float kEndChangeAirEventPos_ = 1120;
	const Vector2 kChangeColorSpriteSize = {64.0f,64.0f};

	changeInAirEvent_ = std::make_unique<Event>();
	changeInAirEvent_->Initialzie(kStartChangeAirEventPos_, kEndChangeAirEventPos_);
	changeInAirEvent_->AddBillboard("arrow.png", Billboard::BillboardType::Yaxis,Vector3(0.0f,3.0f,800.0f),3.0f,Vector4(1.0f,1.0f,1.0f,0.65f));
	changeInAirEvent_->AddBillboard("arrow.png", Billboard::BillboardType::Yaxis,Vector3(0.0f,3.0f,910.0f),3.0f,Vector4(1.0f, 1.0f, 1.0f, 0.65f));
	changeInAirEvent_->AddBillboard("arrow.png", Billboard::BillboardType::Yaxis,Vector3(0.0f,5.0f,1030.0f),3.0f,Vector4(1.0f, 1.0f, 1.0f, 0.65f));
	changeInAirEvent_->AddBillboard("arrow.png", Billboard::BillboardType::Yaxis,Vector3(0.0f,5.0f,1080.0f),3.0f,Vector4(1.0f, 1.0f, 1.0f, 0.65f));
	changeInAirEvent_->AddBillboard("arrow.png", Billboard::BillboardType::Yaxis,Vector3(0.0f,5.0f,1120.0f),3.0f,Vector4(1.0f, 1.0f, 1.0f, 0.65f));
}

void TutorialEvent::Update(Camera* camera)
{
	float playerPosZ = player_->GetTransform().z;

	if(player_->GetIsDead() == false)
	{
		jumpEvent_->Update(playerPosZ, GameSpeed::SpeedMode::SLOW, XINPUT_GAMEPAD_A, DIK_SPACE, camera);
		hightJumpEvent_->Update(playerPosZ, GameSpeed::SpeedMode::NORMAL, XINPUT_GAMEPAD_A, DIK_SPACE, camera);
		changeColorEvent_->Update(playerPosZ, GameSpeed::SpeedMode::SLOW, XINPUT_GAMEPAD_B, DIK_RETURN, camera);
		changeInAirEvent_->Update(playerPosZ, GameSpeed::SpeedMode::NORMAL, 0, 0, camera);
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
