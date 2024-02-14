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
void Event::AddSprite(const std::string& fileName,const Vector2& position, const Vector2& size)
{
	std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>();
	sprite->Initialize(fileName, position);
	sprite->SetSize(size);
	eventButtonSprites_.emplace_back(std::move(sprite));
}

void Event::AddBillboard(const std::string& fileName, const Billboard::BillboardType& billBoardtype, const Vector3& position, float scale, const Vector4& color)
{
	std::unique_ptr<Billboard> billboard = Billboard::Create(fileName,billBoardtype);
	billboard->SetPos(position);
	billboard->SetScale(scale);
	billboard->SetColor(color);
	eventBillboard_.emplace_back(std::move(billboard));
}

void Event::Reset()
{
	isStart_ = false;
	isFinish_ = false;
}

void Event::Update(float playerPosZ,GameSpeed::SpeedMode speedMode,int16_t buttonInfo, BYTE keyboardInfo,Camera* camera)
{
	//画像更新
	if(eventButtonSprites_.empty() == false)
	{
		for(auto& sprite : eventButtonSprites_)
		{
			sprite->matUpdate();
		}
	}
	//ビルボード更新
	if(eventBillboard_.empty() == false)
	{
		for(auto& billBoard : eventBillboard_)
		{
			billBoard->Update(camera);
		}
	}

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
	if(isStart_ == true)
	{
		if(eventButtonSprites_.empty() == false)
		{
			//画像描画
			for(auto& sprite : eventButtonSprites_)
			{
				sprite->Draw();
			}
		}
		if(eventBillboard_.empty() == false)
		{
			//画像描画
			for(auto& billBoard : eventBillboard_)
			{
				billBoard->PreDraw();
				billBoard->Draw();
			}
		}
	}
}
