#include "TutorialEvent.h"
#include "GameSpeed.h"
#include "Player.h"

void TutorialEvent::Initialzie()
{
	aButtonSprite_ = std::make_unique<Sprite>();
	aButtonSprite_->Initialize("A.png",Vector2(640,540));
	isStartJumpEvent_ = false;
}

void TutorialEvent::Update(Player* player, GameSpeed* gameSpeed)
{
	aButtonSprite_->matUpdate();

	int32_t playerPosZ = static_cast<int32_t>(player->GetTransform().z);
	if(playerPosZ >= StartJumpEventPos && playerPosZ <= EndJumpEventPos)
	{
		isStartJumpEvent_ = true;
		gameSpeed->SetSpeedMode(GameSpeed::SpeedMode::SLOW);
	}
	else
	{
		isStartJumpEvent_ = false;
	}
}

void TutorialEvent::Draw()
{
	if(isStartJumpEvent_ == true)
	{
		aButtonSprite_->Draw("A.png");
	}	
}
