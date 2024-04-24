#include "TutorialEvent.h"
#include "GameSpeed.h"
#include "Player.h"
#include "ObjectAttribute.h"
#include "ObjectColor.h"

void TutorialEvent::Initialzie(Player* player, Camera* camera)
{
	player_ = player;
	camera_ = camera;

	const Vector2 kUiSize = { 168.0f,108.0f };
	const Vector2 kUiPosition = { 596.0f,570.0f };

	//スタートイベント設定
	const float kStartEventPos_ = 0;
	const float kEndEventPos_ = 20;
	startEvent_ = std::make_unique<Event>();
	startEvent_->Initialzie(kStartEventPos_, kEndEventPos_);
	startEvent_->AddBillboard("aButton.png", Billboard::BillboardType::Yaxis, Vector3(0.0f, 5.0f, player->GetTransform().z), 2.5f, Vector4(1.0f, 1.0f, 1.0f, 0.9f));

	//ジャンプイベント設定
	const float kStartJumpEventPos_ = 75;
	const float kEndJumpEventPos_ = 100;
	jumpEvent_ = std::make_unique<Event>();
	jumpEvent_->Initialzie(kStartJumpEventPos_, kEndJumpEventPos_);
	jumpEvent_->AddBillboard("aButton.png", Billboard::BillboardType::Yaxis, Vector3(0.0f, 5.0f, player->GetTransform().z), 2.5f, Vector4(1.0f, 1.0f, 1.0f, 0.9f));

	//二回ジャンプイベント設定
	const float kStartDoubleJumpEventPos_ = 185;
	const float kEndDoubleJumpEventPos_ = 210;
	doubleJumpEvent_ = std::make_unique<Event>();
	doubleJumpEvent_->Initialzie(kStartDoubleJumpEventPos_, kEndDoubleJumpEventPos_);
	doubleJumpEvent_->AddBillboard("aButton.png", Billboard::BillboardType::Yaxis, Vector3(0.0f, 5.0f, player->GetTransform().z), 2.5f, Vector4(1.0f, 1.0f, 1.0f, 0.925f));

	//長押しジャンプイベント設定
	const float kStartHighBeforeJumpEventPos_ = 305;
	const float kEndHighJumpBeforeEventPos_ = 350;
	heighJumpBeforeEvent_ = std::make_unique<Event>();
	heighJumpBeforeEvent_->Initialzie(kStartHighBeforeJumpEventPos_, kEndHighJumpBeforeEventPos_);
	heighJumpBeforeEvent_->AddBillboard("aButton.png", Billboard::BillboardType::Yaxis, Vector3(0.0f, 5.0f, player->GetTransform().z), 2.5f, Vector4(1.0f, 1.0f, 1.0f, 0.925f));

	//長押しジャンプイベント設定
	const float kStartHighAfterJumpEventPos_ = 350;
	const float kEndHighJumpAfterEventPos_ = 380;
	heighJumpAfterEvent_ = std::make_unique<Event>();
	heighJumpAfterEvent_->Initialzie(kStartHighAfterJumpEventPos_, kEndHighJumpAfterEventPos_);
	heighJumpAfterEvent_->AddBillboard("aButton.png", Billboard::BillboardType::Yaxis, Vector3(0.0f, 5.0f, player->GetTransform().z), 2.5f, Vector4(1.0f, 1.0f, 1.0f, 0.925f));

	//色変えイベント設定
	const float kStartChangeColorEventPos_ = 590;
	const float kEndChangeColorEventPos_ = 610;
	firstChangeColorEvent_ = std::make_unique<Event>();
	firstChangeColorEvent_->Initialzie(kStartChangeColorEventPos_, kEndChangeColorEventPos_);
	firstChangeColorEvent_->AddBillboard("xboxLt.png", Billboard::BillboardType::Yaxis, Vector3(0.0f, 5.0f, player->GetTransform().z), 2.5f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	//空中での色変えイベント設定
	//Aボタン
	const float kStartJumptoChangeEventPos_ = 690;
	const float kEndJumptoChangeEventPos_ = 710;
	jumptoChangeEvent_ = std::make_unique<Event>();
	jumptoChangeEvent_->Initialzie(kStartJumptoChangeEventPos_, kEndJumptoChangeEventPos_);
	jumptoChangeEvent_->AddBillboard("aButton.png", Billboard::BillboardType::Yaxis, Vector3(0.0f, 3.0f, 680.0f), 2.8f, Vector4(1.0f, 1.0f, 1.0f, 0.925f));
	//Bボタン
	const float kStartChangetoColorEventPos_ = 710;
	const float kEndChangetoColorEventPos_ = 790;
	changeColorEvent_ = std::make_unique<Event>();
	changeColorEvent_->Initialzie(kStartChangetoColorEventPos_, kEndChangetoColorEventPos_);
	changeColorEvent_->AddBillboard("xboxLt.png", Billboard::BillboardType::Yaxis, Vector3(0.0f, 3.0f, 700.0f), 2.8f, Vector4(1.0f, 1.0f, 1.0f, 0.925f));

	//色切り替えビルボード
	const float kStartChangeAirEventPos_ = 600;
	const float kEndChangeAirEventPos_ = 850;
	const Vector2 kChangeColorSpriteSize = { 64.0f,64.0f };
	changeInAirEvent_ = std::make_unique<Event>();
	changeInAirEvent_->Initialzie(kStartChangeAirEventPos_, kEndChangeAirEventPos_);
	changeInAirEvent_->AddBillboard("arrow.png", Billboard::BillboardType::Yaxis, Vector3(0.0f, 3.0f, 710.0f), 3.0f, Vector4(1.0f, 1.0f, 1.0f, 0.65f));
	changeInAirEvent_->AddBillboard("aButton.png", Billboard::BillboardType::Yaxis, Vector3(0.0f, 3.0f, 800.0f), 3.0f, Vector4(1.0f, 1.0f, 1.0f, 0.925f));
	changeInAirEvent_->AddBillboard("aButton.png", Billboard::BillboardType::Yaxis, Vector3(0.0f, 10.0f, 815.0f), 3.0f, Vector4(1.0f, 1.0f, 1.0f, 0.925f));
	
	//加速イベント
	const float kStartAxcellEventPos_ = 850;
	const float kEndAxcellEventPos_ = 1000;
	axcellEvent_ = std::make_unique<Event>();
	axcellEvent_->Initialzie(kStartAxcellEventPos_, kEndAxcellEventPos_);
	axcellEvent_->AddBillboard("bButton.png", Billboard::BillboardType::Yaxis, Vector3(0.0f, 5.0f, player->GetTransform().z), 2.5f, Vector4(1.0f, 1.0f, 1.0f, 0.925f));
}

void TutorialEvent::Update()
{
	Vector3 playerPos = player_->GetTransform();
	float playerPosZ = playerPos.z;
	int32_t playerJumpCount = player_->GetJumpCount();

	if (player_->GetIsDead() == false)
	{
		//各イベントの更新
		startEvent_->Update(player_, GameSpeed::SpeedMode::NORMAL, XINPUT_GAMEPAD_A, DIK_SPACE, camera_, playerJumpCount);
		startEvent_->TransmissiveBillboard();

		jumpEvent_->Update(player_, GameSpeed::SpeedMode::SLOW, XINPUT_GAMEPAD_A, DIK_SPACE, camera_, playerJumpCount);
		jumpEvent_->BillboardSetPlayerPos(playerPos, camera_);
		jumpEvent_->TransmissiveBillboard();

		doubleJumpEvent_->Update(player_, GameSpeed::SpeedMode::SLOW, XINPUT_GAMEPAD_A, DIK_SPACE,camera_, playerJumpCount);
		doubleJumpEvent_->BillboardSetPlayerPos(playerPos, camera_);
		doubleJumpEvent_->TransmissiveBillboard();

		heighJumpBeforeEvent_->Update(player_, GameSpeed::SpeedMode::SLOW, XINPUT_GAMEPAD_A, DIK_SPACE, camera_, playerJumpCount);
		heighJumpBeforeEvent_->BillboardSetPlayerPos(playerPos, camera_);
		heighJumpBeforeEvent_->TransmissiveBillboard();

		//二段ジャンプの二回目のイベントの際に、
		// すでに前のイベントゾーンで二回飛ばれていたらスキップする
		if (player_->GetJumpCount() == 0 && heighJumpAfterEvent_->GetStartPosZ() < playerPos.z)
		{
			heighJumpAfterEvent_->SetIsFinish(true);
		}
		heighJumpAfterEvent_->Update(player_, GameSpeed::SpeedMode::SLOW, XINPUT_GAMEPAD_A, DIK_SPACE, camera_, playerJumpCount);
		heighJumpAfterEvent_->BillboardSetPlayerPos(playerPos, camera_);
		heighJumpAfterEvent_->TransmissiveBillboard();

		//色変えイベント
		firstChangeColorEvent_->Update(player_, GameSpeed::SpeedMode::SLOW, XINPUT_GAMEPAD_B, DIK_RETURN, camera_, playerJumpCount);
		firstChangeColorEvent_->BillboardSetPlayerPos(playerPos, camera_);
		firstChangeColorEvent_->TransmissiveBillboard();

		//色変えジャンプイベント
		//Aボタン
		jumptoChangeEvent_->Update(player_, GameSpeed::SpeedMode::SLOW, XINPUT_GAMEPAD_A, DIK_SPACE, camera_, playerJumpCount);
		jumptoChangeEvent_->BillboardSetPlayerPos(playerPos, camera_);
		jumptoChangeEvent_->TransmissiveBillboard();
		
		//Bボタン
		changeColorEvent_->Update(player_, GameSpeed::SpeedMode::SLOW, XINPUT_GAMEPAD_B, DIK_RETURN, camera_, playerJumpCount);
		changeColorEvent_->BillboardSetPlayerPos(playerPos, camera_);
		changeColorEvent_->TransmissiveBillboard();

		//加速イベント
		axcellEvent_->Update(player_, GameSpeed::SpeedMode::SLOW, XINPUT_GAMEPAD_B, DIK_RETURN, camera_, playerJumpCount);
		axcellEvent_->BillboardSetPlayerPos(playerPos, camera_);
		axcellEvent_->TransmissiveBillboard();
		
		//ボタンを押さないようにする処理
		//ジャンプイベントなので色変えができないように
		const float jumptoChangeEventStartPosZ = jumptoChangeEvent_->GetStartPosZ() - 30.0f;
		if (playerPosZ >= jumptoChangeEventStartPosZ && playerPosZ <= jumptoChangeEvent_->GetFinishPosZ())
		{
			if(jumptoChangeEvent_->GetIsFinish() == false)
			{
				player_->SetCanInputJump(true);
				player_->SetCanInputColor(false);
			}
		}
		//色変えのイベントなのでジャンプができないように
		else if (playerPosZ >= changeColorEvent_->GetStartPosZ() && playerPosZ <= changeColorEvent_->GetFinishPosZ())
		{
			if (changeColorEvent_->GetIsFinish() == false)
			{
				player_->SetCanInputJump(false);
 				player_->SetCanInputColor(true);
			}
			else
			{
				if (player_->GetIsLanded() == true)
				{
					player_->SetCanInputJump(true);
					player_->SetCanInputColor(false);
				}
			}
		}
		//イベントとイベントの間で色変えをできないようにしてバグを防ぐ
		else if(playerPosZ <= axcellEvent_->GetStartPosZ() && playerPosZ >= changeColorEvent_->GetFinishPosZ())
		{
			if (player_->GetIsLanded() == true)
			{
				player_->SetCanInputColor(false);
			}
		}
		else if (playerPosZ >= axcellEvent_->GetStartPosZ() && playerPosZ <= axcellEvent_->GetFinishPosZ())
		{
			if (axcellEvent_->GetIsFinish() == false)
			{
				//加速ができる状態の時に時しかボタンを押せないように
				if (player_->GetCanAccel() == false) 
				{
					player_->SetCanInputJump(false);
					player_->SetCanInputColor(false);

					//もし加速しないで地面についた場合やり直すように
					if (player_->GetOnGround() == true)
					{
						player_->SetIsDead(true);
					}
				}
				else
				{
					player_->SetCanInputJump(false);
					player_->SetCanInputColor(true);
				}
			}
			else
			{
				player_->SetCanInputJump(true);
				player_->SetCanInputColor(true);
			}
		}
		//イベント外では押せるように
		else
		{
			player_->SetCanInputJump(true);
			player_->SetCanInputColor(true);
		}

		//残りの色変えゾーンにビルボードを表示
		changeInAirEvent_->UpdateToDisplayBillboard(playerPosZ, camera_);

		//セーブポイントを更新
		const float longJumpResetStartPosZ = firstChangeColorEvent_->GetStartPosZ() - 80.0f;
		//ジャンプイベント
		if (playerPos.z >= startEvent_->GetStartPosZ() && playerPos.z < heighJumpBeforeEvent_->GetStartPosZ())
		{
			eventSavePoint_ = EventSavePoint::First;
		}
		//ロングジャンプイベント
		else if (playerPos.z >= heighJumpBeforeEvent_->GetStartPosZ() && playerPos.z < longJumpResetStartPosZ)
		{
			eventSavePoint_ = EventSavePoint::LongJump;
		}
		//色変えイベント
		else if (playerPos.z >= firstChangeColorEvent_->GetStartPosZ() && playerPos.z < jumptoChangeEvent_->GetStartPosZ())
		{
			eventSavePoint_ = EventSavePoint::ColorChange;
		}
		//色変えジャンプイベント
		else if (playerPos.z >= jumptoChangeEvent_->GetStartPosZ())
		{
			eventSavePoint_ = EventSavePoint::JumpColorChange;
		}
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
		axcellEvent_->Draw();
	}
}

void TutorialEvent::AllEventReset()
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
	axcellEvent_->Reset();
}

void TutorialEvent::ResetEventforSavePoint()
{
	//チュートリアルのセーブポイントごとに設定をリセットする
	if (eventSavePoint_ == EventSavePoint::First)
	{
		startEvent_->Reset();
		jumpEvent_->Reset();
		CalcSavePoint(startEvent_.get());
	}
	else if (eventSavePoint_ == EventSavePoint::LongJump)
	{
		startEvent_->Reset();
		jumpEvent_->Reset();
		doubleJumpEvent_->Reset();
		heighJumpBeforeEvent_->Reset();
		heighJumpAfterEvent_->Reset();
		CalcSavePoint(heighJumpBeforeEvent_.get());
	}
	else if (eventSavePoint_ == EventSavePoint::ColorChange)
	{
		startEvent_->Reset();
		jumpEvent_->Reset();
		doubleJumpEvent_->Reset();
		heighJumpBeforeEvent_->Reset();
		heighJumpAfterEvent_->Reset();
		firstChangeColorEvent_->Reset();
		CalcSavePoint(firstChangeColorEvent_.get());
	}
	else if (eventSavePoint_ == EventSavePoint::JumpColorChange)
	{
		AllEventReset();
		CalcSavePoint(jumptoChangeEvent_.get());
	}
	else if (eventSavePoint_ == EventSavePoint::Finish){}
	else 
	{
		AllEventReset();
	}
}

void TutorialEvent::CalcSavePoint(Event* event)
{
	Vector3 playerPos = player_->GetTransform();

	if (eventSavePoint_ == EventSavePoint::First)
	{
		const float startPosZ = 2.0f;
		playerPos.z = startPosZ;
	}
	else if (eventSavePoint_ == EventSavePoint::LongJump)
	{
		const float startPosZ = event->GetStartPosZ() + 10.0f;
		playerPos.z = startPosZ;
	}
	else if (eventSavePoint_ == EventSavePoint::ColorChange)
	{
		const float startPosY = 7.5f;
		const float startPosZ = event->GetStartPosZ() - 80.0f;

		playerPos.y = startPosY;
		playerPos.z = startPosZ;
	}
	else if (eventSavePoint_ == EventSavePoint::JumpColorChange)
	{
		const float startPosY = 0.0f;
		const float startPosZ = event->GetStartPosZ() - 10.0f;

		playerPos.y = startPosY;
		playerPos.z = startPosZ;
		player_->SetAttributeColor(yellow);
		player_->SetColor(kYellowOBJColor);
	}
	else
	{
		playerPos.z = event->GetStartPosZ();
	}
	//初期位置からのイベント開始位置を計算
	float distance = playerPos.z - player_->GetInitPos().z;
	//プレイヤーとカメラをイベント終了位置まで動かす
	player_->SetTransform(playerPos);
	Vector3 cameraEye = camera_->GetEye();
	cameraEye.z = camera_->GetEye().z + distance;
	Vector3 cameraTarget = camera_->GetTarget();
	cameraTarget.z = camera_->GetTarget().z + distance;
	camera_->SetEye(cameraEye);
	camera_->SetTarget(cameraTarget);
}
