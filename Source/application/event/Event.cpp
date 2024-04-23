#include "Event.h"
#include "Player.h"

KeyInput* Event::keys_ = nullptr;
GamePad* Event::gamePad_ = nullptr;
GameSpeed* Event::gameSpeed_ = nullptr;

void Event::StaticInitialize(KeyInput* keys, GamePad* gamePad, GameSpeed* gameSpeed)
{
	gamePad_ = gamePad;
	keys_ = keys;
	gameSpeed_ = gameSpeed;
}

void Event::Initialzie(float startPos, float endPos)
{
	startPos_ = startPos;
	finishPos_ = endPos;
	buttonAnimeTime_ = defaultAnimeTime;
	Reset();
}

void Event::AddSprite(const std::string& fileName, const Vector2& position, const Vector2& size)
{
	std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>();
	sprite->Initialize(fileName, position);
	sprite->SetSize(size);
	eventButtonSprites_.emplace_back(std::move(sprite));
}

void Event::AddBillboard(const std::string& fileName, const Billboard::BillboardType& billBoardtype, const Vector3& position, float scale, const Vector4& color)
{
	std::unique_ptr<Billboard> billboard = Billboard::Create(fileName, billBoardtype);
	billboard->SetPos(position);
	billboard->SetScale(scale);
	billboard->SetColor(color);
	eventBillboard_.emplace_back(std::move(billboard));
}

void Event::Reset()
{
	isStart_ = false;
	isFinish_ = false;
	isPushedButton_ = false;
	isClearSprite_ = false;
	buttonTimer_ = buttonAnimeTime_;
	isDisplayBillboard_ = false;
	
	for (auto& billBoard : eventBillboard_)
	{
		billBoard->SetColor(billBoard->GetkDefaultColor());
	}
}

void Event::Finish()
{
	isPushedButton_ = true;
	isFinish_ = true;
	gameSpeed_->SetSpeedMode(GameSpeed::SpeedMode::NORMAL);
}

void Event::SpriteMatUpdate()
{
	//画像更新
	if (eventButtonSprites_.empty() == false)
	{
		for (auto& sprite : eventButtonSprites_)
		{
			sprite->matUpdate();
		}
	}
}

void Event::BillboardMatUpdate(Camera* camera)
{
	//ビルボード更新
	if (eventBillboard_.empty() == false)
	{
		for (auto& billBoard : eventBillboard_)
		{
			billBoard->Update(camera);
		}
	}
}

void Event::UpdateToDisplayBillboard(float playerPosZ,Camera* camera)
{
	//表示したい範囲内にプレイヤーが入ったらビルボードを表示
	if (playerPosZ >= startPos_ && playerPosZ <= finishPos_)
	{
		isDisplayBillboard_ = true;
		BillboardMatUpdate(camera);
	}
	else
	{
		//範囲外に出たらイベント終了
		isDisplayBillboard_ = false;
	}
}

void Event::BillboardSetPlayerPos(const Vector3& playerPos, Camera* camera)
{
	//ビルボード更新
	if (eventBillboard_.empty() == false)
	{
		for (auto& billBoard : eventBillboard_)
		{
			billBoard->SetPos(Vector3(billBoard->GetPos().x, playerPos.y + 3.0f, playerPos.z));
			billBoard->Update(camera);
		}
	}
}

void Event::TransmissiveBillboard()
{ 
	if (isPushedButton_ == true)
	{
		for (auto& billBoard : eventBillboard_)
		{
			float colorAlpha_ = billBoard->GetColor().w;
			if (colorAlpha_ > 0.0f)
			{
				colorAlpha_ -= 0.1f;
			}
			Vector3 color = Vector3(billBoard->GetColor().x, billBoard->GetColor().y, billBoard->GetColor().z);
			billBoard->SetColor(Vector4(color.x, color.y, color.z, colorAlpha_));
		}
	}
}

void Event::Update(Player* player, GameSpeed::SpeedMode speedMode, int16_t buttonInfo, BYTE keyboardInfo, Camera* camera, int32_t canJumpCount)
{
	buttonInfo_ = buttonInfo;
	keyboardInfo_ = keyboardInfo;
	float playerPosZ = player->GetTransform().z;

	SpriteMatUpdate();
	BillboardMatUpdate(camera);

	//イベントが終了している状態で
	//イベントの範囲にプレイヤーが入ったらイベントを開始
	if (playerPosZ >= startPos_ && playerPosZ <= finishPos_)
	{
		if (isFinish_ == false)
		{
			isStart_ = true;

			//ジャンプボタンがトリガーの場合、
			//ジャンプできる状態であれば、モードを設定にする
			if (buttonInfo == XINPUT_GAMEPAD_A || keyboardInfo == DIK_SPACE)
			{
				if (player->GetCanInputJump() == true)
				{
					if (canJumpCount > 0)
					{
						gameSpeed_->SetSpeedMode(speedMode);
					}
				}
			}
			else
			{
				if (player->GetCanInputColor() == true)
				{
					gameSpeed_->SetSpeedMode(speedMode);
				}
			}
		}
	}
	else
	{
		//範囲外にでた場合、
		//スピードを普通の状態に戻し、イベントを終了
		if (isStart_ == true)
		{
			gameSpeed_->SetSpeedMode(GameSpeed::SpeedMode::NORMAL);
			isStart_ = false;
		}
	}

	//イベント中に
	if (isStart_ == true)
	{
		if(buttonInfo == XINPUT_GAMEPAD_B)
		{
			if (player->GetCanInputColor() == true)
			{
				if (gamePad_->PushedLeftTriggerMoment() || keys_->PushedKeyMoment(keyboardInfo))
				{
					Finish();
				}
			}
		}
		else
		{
			if (player->GetCanInputJump() == true)
			{
				//対象のボタンが押されたらイベント終了
				if (gamePad_->PushedButtonMoment(buttonInfo) || keys_->PushedKeyMoment(keyboardInfo))
				{
					Finish();
				}
			}
		}
	}
}

void Event::AnimationUpdate()
{
	if (isStart_ == true)
	{
		if (buttonTimer_ > 0)
		{
			buttonTimer_--;
			if (buttonTimer_ <= 0)
			{
				buttonTimer_ = buttonAnimeTime_;
				isPushedButton_ = false;
			}
		}
	}
}

void Event::Draw()
{
	if (isStart_ == true || isDisplayBillboard_ == true)
	{
		if (eventButtonSprites_.empty() == false)
		{
			//画像描画
			for (auto& sprite : eventButtonSprites_)
			{
				sprite->Draw();
			}
		}
		if (eventBillboard_.empty() == false)
		{
			//画像描画
			for (auto& billBoard : eventBillboard_)
			{
				billBoard->PreDraw();
				billBoard->Draw();
			}
		}
	}
}

void Event::AnimationDraw()
{
	if (isStart_ == true)
	{
		if (isPushedButton_ == true)
		{
			eventButtonSprites_[1]->Draw();
		}
		else
		{
			if (eventButtonSprites_.empty() == false)
			{
				//画像描画
				eventButtonSprites_[0]->Draw();
			}
		}
	}
}
