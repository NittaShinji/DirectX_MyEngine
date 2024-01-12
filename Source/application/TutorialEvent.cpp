#include "TutorialEvent.h"
#include "GameSpeed.h"
#include "Player.h"

void TutorialEvent::Initialzie()
{
	//ジャンプイベント設定
	const float kStartJumpEventPos_ = 70;
	const float kEndJumpEventPos_ = 120;
	jumpEvent_ = std::make_unique<Event>();
	jumpEvent_->Initialzie(kStartJumpEventPos_, kEndJumpEventPos_);
	jumpEvent_->AddSprite("A.png", Vector2(576.0f,296.0f));
	jumpEvent_->AddSprite("B.png", Vector2(704.0f,424.0f));

	//jumpEvent_->AddSprite("A.png", Vector2(576.0f,296.0f));

}

void TutorialEvent::Update(Player* player)
{
	float playerPosZ = player->GetTransform().z;

	jumpEvent_->Update(playerPosZ, GameSpeed::SpeedMode::SLOW, XINPUT_GAMEPAD_A, DIK_SPACE);
}

void TutorialEvent::Draw()
{
	jumpEvent_->Draw();
}
