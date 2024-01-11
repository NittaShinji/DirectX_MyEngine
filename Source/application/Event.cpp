#include "Event.h"

KeyInput* Event::keys_ = nullptr;
GamePad* Event::gamePad_ = nullptr;
GameSpeed* Event::gameSpeed_ = nullptr;

void Event::StaticInitialize(KeyInput* keys, GamePad* gamePad, GameSpeed* gameSpeed)
{
	gamePad_ = gamePad;
	keys_ = keys;
	gameSpeed_ = gameSpeed;
}

void Event::Initialzie(float startPos,float endPos)
{
	Reset();
	startPos_ = startPos;
	finishPos_= endPos;
}
void Event::SetSprite(std::string fileName,Vector2 position)
{
	eventButtonSprite_ = std::make_unique<Sprite>();
	eventButtonSprite_->Initialize(fileName, position);
}

void Event::Reset()
{
	isStart_ = false;
	isFinish_ = false;
}

void Event::Update(float playerPosZ,GameSpeed::SpeedMode speedMode,int16_t buttonInfo, BYTE keyboardInfo)
{
	//イベントが終了している状態で
	//イベントの範囲にプレイヤーが入ったらイベントを開始
	if(playerPosZ >= startPos_ && playerPosZ <= finishPos_)
	{
		if(isFinish_ == false)
		{
			isStart_ = true;
			gameSpeed_->SetSpeedMode(speedMode);
		}
	}
	else
	{
		//イベントの範囲を離れたらそのイベントフラグをリセット
		Reset();
	}

	//イベント中に対象のボタンが押されたら
	if(isStart_ == true)
	{
		//イベント終了
		if(gamePad_->PushedButtonMoment(buttonInfo) || keys_->PushedKeyMoment(keyboardInfo))
		{
			isFinish_ = true;
			gameSpeed_->SetSpeedMode(GameSpeed::SpeedMode::NORMAL);
		}
	}
}

void Event::Draw()
{
	if(eventButtonSprite_ != nullptr)
	{
		eventButtonSprite_->Draw(eventButtonSprite_->GetFileName());
	}
}
