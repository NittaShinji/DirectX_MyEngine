#include "TutorialEvent.h"
#include "GameSpeed.h"
#include "Player.h"

void TutorialEvent::Initialzie(Player* player)
{
	player_ = player;

	const Vector2 kUiSize = { 168.0f,108.0f };
	const Vector2 kUiPosition = { 596.0f,570.0f };

	//スタートイベント設定
	const float kStartEventPos_ = 0;
	const float kEndEventPos_ = 20;
	startEvent_ = std::make_unique<Event>();
	startEvent_->Initialzie(kStartEventPos_, kEndEventPos_);
	startEvent_->SetIsAnimate(true);
	startEvent_->AddSprite("aButton.png", kUiPosition, kUiSize);
	startEvent_->AddSprite("aPushed.png", kUiPosition, kUiSize);

	//ジャンプイベント設定
	const float kStartJumpEventPos_ = 75;
	const float kEndJumpEventPos_ = 110;
	jumpEvent_ = std::make_unique<Event>();
	jumpEvent_->Initialzie(kStartJumpEventPos_, kEndJumpEventPos_);
	jumpEvent_->SetIsAnimate(true);
	jumpEvent_->AddSprite("aButton.png", kUiPosition, kUiSize);
	jumpEvent_->AddSprite("aPushed.png", kUiPosition, kUiSize);

	const float kStartDoubleJumpEventPos_ = 175;
	const float kEndDoubleJumpEventPos_ = 320;
	doubleJumpEvent_ = std::make_unique<Event>();
	doubleJumpEvent_->Initialzie(kStartDoubleJumpEventPos_, kEndDoubleJumpEventPos_);
	doubleJumpEvent_->SetIsAnimate(true);
	doubleJumpEvent_->AddSprite("aButton.png", kUiPosition, kUiSize);
	doubleJumpEvent_->AddSprite("aPushed.png", kUiPosition, kUiSize);

	const float kStartHighBeforeJumpEventPos_ = 350;
	const float kEndHighJumpBeforeEventPos_ = 400;
	heighJumpBeforeEvent_ = std::make_unique<Event>();
	heighJumpBeforeEvent_->Initialzie(kStartHighBeforeJumpEventPos_, kEndHighJumpBeforeEventPos_);
	heighJumpBeforeEvent_->SetIsAnimate(true);
	heighJumpBeforeEvent_->AddSprite("aButton.png", kUiPosition, kUiSize);
	heighJumpBeforeEvent_->AddSprite("aPushed.png", kUiPosition, kUiSize);
	
	const float kStartHighAfterJumpEventPos_ = 400;
	const float kEndHighJumpAfterEventPos_ = 440;
	heighJumpAfterEvent_ = std::make_unique<Event>();
	heighJumpAfterEvent_->Initialzie(kStartHighAfterJumpEventPos_, kEndHighJumpAfterEventPos_);
	heighJumpAfterEvent_->SetIsAnimate(true);
	heighJumpAfterEvent_->AddSprite("aButton.png", kUiPosition, kUiSize);
	heighJumpAfterEvent_->AddSprite("aPushed.png", kUiPosition, kUiSize);
	const int32_t heightJumpButtonTime = 30;
	heighJumpAfterEvent_->SetButtonAnimeTime(heightJumpButtonTime);

	const float kStartChangeColorEventPos_ = 650;
	const float kEndChangeColorEventPos_ = 670;
	changeColorEvent_ = std::make_unique<Event>();
	changeColorEvent_->Initialzie(kStartChangeColorEventPos_, kEndChangeColorEventPos_);
	heighJumpBeforeEvent_->SetIsAnimate(true);
	changeColorEvent_->AddSprite("bButton.png", kUiPosition, kUiSize);
	changeColorEvent_->AddSprite("bPushed.png", kUiPosition, kUiSize);

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
		startEvent_->Update(playerPosZ, GameSpeed::SpeedMode::NORMAL, XINPUT_GAMEPAD_A, DIK_SPACE, camera);
		jumpEvent_->Update(playerPosZ, GameSpeed::SpeedMode::SLOW, XINPUT_GAMEPAD_A, DIK_SPACE, camera);
		doubleJumpEvent_->Update(playerPosZ, GameSpeed::SpeedMode::NORMAL, XINPUT_GAMEPAD_A, DIK_SPACE, camera);
		heighJumpBeforeEvent_->Update(playerPosZ, GameSpeed::SpeedMode::NORMAL, XINPUT_GAMEPAD_A, DIK_SPACE, camera);
		heighJumpAfterEvent_->Update(playerPosZ, GameSpeed::SpeedMode::SLOW, XINPUT_GAMEPAD_A, DIK_SPACE, camera);

		changeColorEvent_->Update(playerPosZ, GameSpeed::SpeedMode::SLOW, XINPUT_GAMEPAD_B, DIK_RETURN, camera);
		changeInAirEvent_->Update(playerPosZ, GameSpeed::SpeedMode::NORMAL, 0, 0, camera);
	}
}

void TutorialEvent::Draw()
{
	if(player_->GetIsDead() == false)
	{
		startEvent_->AnimationDraw();
		jumpEvent_->AnimationDraw();
		doubleJumpEvent_->AnimationDraw();
		heighJumpBeforeEvent_->AnimationDraw();
		heighJumpAfterEvent_->AnimationDraw();
		changeColorEvent_->AnimationDraw();
 		changeInAirEvent_->Draw();
	}
}
