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
	startEvent_->AddBillboard("aButton.png", Billboard::BillboardType::Yaxis, Vector3(0.0f, 5.0f, player->GetTransform().z), 2.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	//ジャンプイベント設定
	const float kStartJumpEventPos_ = 75;
	const float kEndJumpEventPos_ = 100;
	jumpEvent_ = std::make_unique<Event>();
	jumpEvent_->Initialzie(kStartJumpEventPos_, kEndJumpEventPos_);
	jumpEvent_->AddBillboard("aButton.png", Billboard::BillboardType::Yaxis, Vector3(0.0f, 5.0f, player->GetTransform().z), 2.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	//二回ジャンプイベント設定
	const float kStartDoubleJumpEventPos_ = 185;
	const float kEndDoubleJumpEventPos_ = 210;
	doubleJumpEvent_ = std::make_unique<Event>();
	doubleJumpEvent_->Initialzie(kStartDoubleJumpEventPos_, kEndDoubleJumpEventPos_);
	doubleJumpEvent_->AddBillboard("aButton.png", Billboard::BillboardType::Yaxis, Vector3(0.0f, 5.0f, player->GetTransform().z), 2.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	//長押しジャンプイベント設定
	const float kStartHighBeforeJumpEventPos_ = 315;
	const float kEndHighJumpBeforeEventPos_ = 350;
	heighJumpBeforeEvent_ = std::make_unique<Event>();
	heighJumpBeforeEvent_->Initialzie(kStartHighBeforeJumpEventPos_, kEndHighJumpBeforeEventPos_);
	heighJumpBeforeEvent_->AddBillboard("aButton.png", Billboard::BillboardType::Yaxis, Vector3(0.0f, 5.0f, player->GetTransform().z), 2.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	//長押しジャンプイベント設定
	const float kStartHighAfterJumpEventPos_ = 350;
	const float kEndHighJumpAfterEventPos_ = 380;
	heighJumpAfterEvent_ = std::make_unique<Event>();
	heighJumpAfterEvent_->Initialzie(kStartHighAfterJumpEventPos_, kEndHighJumpAfterEventPos_);
	heighJumpAfterEvent_->AddBillboard("aButton.png", Billboard::BillboardType::Yaxis, Vector3(0.0f, 5.0f, player->GetTransform().z), 2.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	//色変えイベント設定
	const float kStartChangeColorEventPos_ = 590;
	const float kEndChangeColorEventPos_ = 610;
	firstChangeColorEvent_ = std::make_unique<Event>();
	firstChangeColorEvent_->Initialzie(kStartChangeColorEventPos_, kEndChangeColorEventPos_);
	firstChangeColorEvent_->AddBillboard("xboxRt.png", Billboard::BillboardType::Yaxis, Vector3(0.0f, 5.0f, player->GetTransform().z), 2.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	//空中での色変えイベント設定
	//Aボタン
	const float kStartJumptoChangeEventPos_ = 690;
	const float kEndJumptoChangeEventPos_ = 700;
	jumptoChangeEvent_ = std::make_unique<Event>();
	jumptoChangeEvent_->Initialzie(kStartJumptoChangeEventPos_, kEndJumptoChangeEventPos_);
	jumptoChangeEvent_->AddBillboard("aButton.png", Billboard::BillboardType::Yaxis, Vector3(0.0f, 3.0f, 680.0f), 3.0f, Vector4(1.0f, 1.0f, 1.0f, 0.65f));
	//Bボタン
	const float kStartChangetoColorEventPos_ = 700;
	const float kEndChangetoColorEventPos_ = 710;
	changeColorEvent_ = std::make_unique<Event>();
	changeColorEvent_->Initialzie(kStartChangetoColorEventPos_, kEndChangetoColorEventPos_);
	//changeColorEvent_->AddBillboard("bButton.png", Billboard::BillboardType::Yaxis, Vector3(-2.0f, 3.0f, 768.0f), 3.0f, Vector4(1.0f, 1.0f, 1.0f, 0.65f));
	changeColorEvent_->AddBillboard("xboxRt.png", Billboard::BillboardType::Yaxis, Vector3(0.0f, 3.0f, 700.0f), 3.0f, Vector4(1.0f, 1.0f, 1.0f, 0.65f));

	//色切り替えビルボード
	const float kStartChangeAirEventPos_ = 710;
	const float kEndChangeAirEventPos_ = 1000;
	const Vector2 kChangeColorSpriteSize = { 64.0f,64.0f };
	changeInAirEvent_ = std::make_unique<Event>();
	changeInAirEvent_->Initialzie(kStartChangeAirEventPos_, kEndChangeAirEventPos_);
	changeInAirEvent_->AddBillboard("arrow.png", Billboard::BillboardType::Yaxis, Vector3(0.0f, 3.0f, 710.0f), 3.0f, Vector4(1.0f, 1.0f, 1.0f, 0.65f));
	changeInAirEvent_->AddBillboard("arrow.png", Billboard::BillboardType::Yaxis, Vector3(0.0f, 3.0f, 790.0f), 3.0f, Vector4(1.0f, 1.0f, 1.0f, 0.65f));
	changeInAirEvent_->AddBillboard("arrow.png", Billboard::BillboardType::Yaxis, Vector3(0.0f, 5.0f, 850.0f), 3.0f, Vector4(1.0f, 1.0f, 1.0f, 0.65f));
	changeInAirEvent_->AddBillboard("arrow.png", Billboard::BillboardType::Yaxis, Vector3(0.0f, 5.0f, 910.0f), 3.0f, Vector4(1.0f, 1.0f, 1.0f, 0.65f));
	changeInAirEvent_->AddBillboard("arrow.png", Billboard::BillboardType::Yaxis, Vector3(0.0f, 5.0f, 970.0f), 3.0f, Vector4(1.0f, 1.0f, 1.0f, 0.65f));
}

void TutorialEvent::Update(Camera* camera)
{
	Vector3 playerPos = player_->GetTransform();
	float playerPosZ = playerPos.z;
	int32_t playerJumpCount = player_->GetJumpCount();

	if (player_->GetIsDead() == false)
	{
		//各イベントの更新
		startEvent_->Update(playerPosZ, GameSpeed::SpeedMode::NORMAL, XINPUT_GAMEPAD_A, DIK_SPACE, camera, playerJumpCount);
		startEvent_->TransmissiveBillboard();

		jumpEvent_->Update(playerPosZ, GameSpeed::SpeedMode::SLOW, XINPUT_GAMEPAD_A, DIK_SPACE, camera, playerJumpCount);
		jumpEvent_->BillboardSetPlayerPos(playerPos, camera);
		jumpEvent_->TransmissiveBillboard();

		doubleJumpEvent_->Update(playerPosZ, GameSpeed::SpeedMode::SLOW, XINPUT_GAMEPAD_A, DIK_SPACE,camera, playerJumpCount);
		doubleJumpEvent_->BillboardSetPlayerPos(playerPos, camera);
		doubleJumpEvent_->TransmissiveBillboard();

		heighJumpBeforeEvent_->Update(playerPosZ, GameSpeed::SpeedMode::SLOW, XINPUT_GAMEPAD_A, DIK_SPACE, camera, playerJumpCount);
		heighJumpBeforeEvent_->BillboardSetPlayerPos(playerPos, camera);
		heighJumpBeforeEvent_->TransmissiveBillboard();

		//二段ジャンプの二回目のイベントの際に、
		// すでに前のイベントゾーンで二回飛ばれていたらスキップする
		if (player_->GetJumpCount() == 0 && heighJumpAfterEvent_->GetStartPos() < playerPos.z)
		{
			heighJumpAfterEvent_->SetIsFinish(true);
		}
		heighJumpAfterEvent_->Update(playerPosZ, GameSpeed::SpeedMode::SLOW, XINPUT_GAMEPAD_A, DIK_SPACE, camera, playerJumpCount);
		heighJumpAfterEvent_->BillboardSetPlayerPos(playerPos, camera);
		heighJumpAfterEvent_->TransmissiveBillboard();

		//色変えイベント
		firstChangeColorEvent_->Update(playerPosZ, GameSpeed::SpeedMode::SLOW, XINPUT_GAMEPAD_B, DIK_RETURN, camera, playerJumpCount);
		firstChangeColorEvent_->BillboardSetPlayerPos(playerPos, camera);
		firstChangeColorEvent_->TransmissiveBillboard();

		//色変えジャンプイベント
		//Aボタン
		jumptoChangeEvent_->Update(playerPosZ, GameSpeed::SpeedMode::SLOW, XINPUT_GAMEPAD_A, DIK_SPACE, camera, playerJumpCount);
		jumptoChangeEvent_->BillboardSetPlayerPos(playerPos, camera);
		jumptoChangeEvent_->TransmissiveBillboard();
		//Bボタン
		changeColorEvent_->Update(playerPosZ, GameSpeed::SpeedMode::SLOW, XINPUT_GAMEPAD_B, DIK_RETURN, camera, playerJumpCount);
		changeColorEvent_->BillboardSetPlayerPos(playerPos, camera);
		changeColorEvent_->TransmissiveBillboard();
		//残りの色変えゾーンにビルボードを表示
		changeInAirEvent_->Update(playerPosZ, GameSpeed::SpeedMode::NORMAL, 0, 0, camera, playerJumpCount);
	}
}

void TutorialEvent::Draw()
{
	if (player_->GetIsDead() == false)
	{
		startEvent_->Draw();
		jumpEvent_->Draw();
		doubleJumpEvent_->Draw();
		heighJumpBeforeEvent_->Draw();
		heighJumpAfterEvent_->Draw();
		firstChangeColorEvent_->Draw();
		changeInAirEvent_->Draw();
		jumptoChangeEvent_->Draw();
		changeColorEvent_->Draw();
	}
}

void TutorialEvent::Reset()
{
	startEvent_->Reset();
	jumpEvent_->Reset();
	doubleJumpEvent_->Reset();
	heighJumpBeforeEvent_->Reset();
	heighJumpAfterEvent_->Reset();
	firstChangeColorEvent_->Reset();
	changeInAirEvent_->Reset();
	jumptoChangeEvent_->Reset();
	changeColorEvent_->Reset();
}
