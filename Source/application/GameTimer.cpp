#include "GameTimer.h"
#include "TextureManager.h"

//ゲーム中に表示する時間(4桁)
int GameTimer::inGameTime_;
//クリア画面で表示する時間(6桁)
int GameTimer::timer_ = 0;
int GameTimer::resultTime_;
//ハイスコア
int GameTimer::highScoreTime_;

bool GameTimer::isLoadSprite_ = false;

std::unique_ptr<Sprite> GameTimer::inGameNum[inGameDigits];
std::unique_ptr<Sprite> GameTimer::resultNum[resultDigits];
std::unique_ptr<Sprite> GameTimer::BlackDot_;

//ゲーム中に表示する時間(4桁)
int GameTimer::inGameDisPlayTime_[inGameDigits];
//クリア画面で表示する時間(6桁)
int GameTimer::resultDisPlaytime[resultDigits];

void GameTimer::LoadSprite()
{
	//一度だけ画像を読み込む
	if(isLoadSprite_ == false)
	{
		//数字のテクスチャ読み込み
		TextureManager::GetInstance()->LoadTexture("numbers.png");
		//黒いドットの点画像を作成
		const int DotSize = 6;
		const Vector4 BlackColor = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
		TextureManager::GetInstance()->TexMapping(DotSize, DotSize, BlackColor, "BLACKDot");
		isLoadSprite_ = true;
	}
}

void GameTimer::StaticInitialize()
{
	//メンバ変数初期化
	inGameTime_ = 0;
	resultTime_ = 0;
	highScoreTime_ = 0;
	
	BlackDot_ = std::make_unique<Sprite>();
	for(int i = 0; i < inGameDigits; i++)
	{
		inGameNum[i] = std::make_unique<Sprite>();
	}
	for(int i = 0; i < resultDigits; i++)
	{
		resultNum[i] = std::make_unique<Sprite>();
	}
}

void GameTimer::InGameInitialize()
{
	LoadSprite();

	Vector2 texTotalSize = TextureManager::GetInstance()->GetTexSize("numbers.png");
	Vector2 texSize = texTotalSize;
	texSize.x = texSize.x / totalNumber;
	const int half = 2;
	for(int i = 0; i < inGameDigits; i++)
	{
		if(i < 2)
		{
			// 数字の初期化
			inGameNum[i]->Initialize("numbers.png", Vector2((i * texSize.x), texSize.y));
		}
		else
		{
			// 数字の初期化
			inGameNum[i]->Initialize("numbers.png", Vector2((i * texSize.x + texSize.x / half), texSize.y));
		}

		inGameDisPlayTime_[i] = { 0 };
		
		SetNumber(inGameDisPlayTime_[i], inGameNum[i].get());
	}

	for(int i = 0; i < inGameDigits; i++)
	{
		inGameNum[i]->SetTextureClipSize(texSize);
		inGameNum[i]->SetSize(texSize);
	}

	//float integerX = inGameNum[1]->GetPosition().x;
	float decimalX = inGameNum[2]->GetPosition().x;

	Vector2 decimalPointPos = Vector2(decimalX - texSize.x / half + 6, texSize.y + texSize.y - 15);

	BlackDot_->Initialize("BLACKDot", decimalPointPos);

	timer_ = 0;
}

void GameTimer::ResultInitialize()
{
	Vector2 texTotalSize = TextureManager::GetInstance()->GetTexSize("numbers.png");
	Vector2 texSize = texTotalSize;
	const int half = 2;
	texSize.x = texSize.x / totalNumber;
	//for(int i = 0; i < resultDigits; i++)
	//{
	//	resultNum[i] = std::make_unique<Sprite>();
	//	// 数字の初期化
	//	resultNum[i]->Initialize("numbers.png", Vector2((i * texSize.x), texSize.y));
	//}

	for(int i = 0; i < resultDigits; i++)
	{
		if(i < 3)
		{
			// 数字の初期化
			resultNum[i]->Initialize("numbers.png", Vector2((i * texSize.x), texSize.y));
		}
		else
		{
			// 数字の初期化
			resultNum[i]->Initialize("numbers.png", Vector2((i * texSize.x + texSize.x / half), texSize.y));
		}
	}

	for(int i = 0; i < resultDigits; i++)
	{
		resultNum[i]->SetTextureClipSize(texSize);
		resultNum[i]->SetSize(texSize);
	}

	//float integerX = resultNum[2]->GetPosition().x;
	float decimalX = resultNum[3]->GetPosition().x;

	Vector2 decimalPointPos = Vector2(decimalX - texSize.x / half + 6, texSize.y + texSize.y - 15);

	BlackDot_->SetPosition(decimalPointPos);

}

void GameTimer::InGameUpdate(bool isStart,bool isFinish)
{
	for(int i = 0; i < inGameDigits; i++)
	{
		inGameNum[i]->matUpdate();
	}

	BlackDot_->matUpdate();

	//プレイヤーが動き始めたら
	if(isStart == true)
	{
		InGameNumberUpdate(isFinish);
	}
	else
	{
		timer_ = 0;
		for(size_t i = 0; i < inGameDigits; i++)
		{
			inGameDisPlayTime_[i] = { 0 };
			SetNumber(inGameDisPlayTime_[i], inGameNum[i].get());
		}
	}

	//クリア時間を保存
	if(isFinish == true)
	{
		resultTime_ = inGameTime_;

		//--4桁目(千の位を表示)
		resultDisPlaytime[0] = 0;
		//--3桁目(百の位を表示)
		resultDisPlaytime[1] = inGameDisPlayTime_[0];

		//--2桁目(十の位を表示)
		resultDisPlaytime[2] = inGameDisPlayTime_[1];
		//--1桁目(1の位を表示)
		resultDisPlaytime[3] = inGameDisPlayTime_[2];
		resultDisPlaytime[4] = inGameDisPlayTime_[3];
		resultDisPlaytime[5] = 0;
	}
}

void GameTimer::ResultUpdate()
{
	for(int i = 0; i < resultDigits; i++)
	{
		resultNum[i]->matUpdate();
	}

	BlackDot_->matUpdate();

	ResultNumberUpdate();

	for(size_t i = 0; i < resultDigits; i++)
	{
		SetNumber(resultDisPlaytime[i], resultNum[i].get());
	}
}

//
//void GameTimer::Update(bool isStart)
//{
//	for(int i = 0; i < resultDigits; i++)
//	{
//		//resultNum[i]->matUpdate();
//	}
//	for(int i = 0; i < inGameDigits; i++)
//	{
//		inGameNum[i]->matUpdate();
//	}
//
//	BlackDot_->matUpdate();
//
//
//	if(isStart == true)
//	{
//		InGameNumberUpdate();
//		//ResultNumberUpdate();
//	}
//	else
//	{
//		//for(size_t i = 0; i < resultDigits; i++)
//		//{
//		//	resultDisPlaytime[i] = { 0 };
//		//	SetNumber(resultDisPlaytime[i], resultNum[i].get());
//		//}
//
//		for(size_t i = 0; i < inGameDigits; i++)
//		{
//			inGameDisPlayTime_[i] = { 0 };
//			SetNumber(inGameDisPlayTime_[i], inGameNum[i].get());
//		}
//
//	}
//}

void GameTimer::Reset()
{
	timer_ = 0;
	inGameTime_ = 0;
	resultTime_ = 0;
}

void GameTimer::NumberUpdate()
{
	for(int i = 0; i < resultDigits; i++)
	{
		//num[i]->SetTextureLeftTop(Vector2(texSize.x * i, texSize.y * i));
		//num[i]->SetTextureClipSize(texSize);
		//num[i]->SetSize(texSize);
	}
}

void GameTimer::InGameDraw()
{
	for(int i = 0; i < inGameDigits; i++)
	{
		inGameNum[i]->Draw("numbers.png");
	}

	BlackDot_->Draw("BLACKDot");
}
void GameTimer::ResultDraw()
{
	for(int i = 0; i < resultDigits; i++)
	{
		resultNum[i]->Draw("numbers.png");
	}

	BlackDot_->Draw("BLACKDot");
}

void GameTimer::InGameNumberUpdate(bool isFinish)
{
	if(isFinish == false)
	{
		//t 経過時間	b最初の位置	c移動量	d移動時間
		if(inGameTime_ <= kMaxTime_)
		{
			timer_++;
			if(timer_ >= kOneSeconds_)
			{
				timer_ = 0;
				inGameTime_++;
			}
		}
	}

	//--4桁目(千の位を表示)
	inGameDisPlayTime_[0] = (inGameTime_ % 100) / 10;
	//--3桁目(百の位を表示)
	inGameDisPlayTime_[1] = (inGameTime_ % 10) / 1;

	//--2桁目(十の位を表示)
	inGameDisPlayTime_[2] = (timer_ % 100) / 10;
	//--1桁目(1の位を表示)
	inGameDisPlayTime_[3] = (timer_ % 10) / 1;

	for(size_t i = 0; i < inGameDigits; i++)
	{
		SetNumber(inGameDisPlayTime_[i], inGameNum[i].get());
	}
}

void GameTimer::ResultNumberUpdate()
{
	//t 経過時間	b最初の位置	c移動量	d移動時間
	if(timer_ < resultTime_)
	{
		timer_++;
		if(timer_ >= resultTime_)
		{
			timer_ = resultTime_;
		}
	}	

	////--6桁目(万の位を表示)
	//resultDisPlaytime[0] = (timer_ % 1000000) / 100000;
	////--5桁目(千の位を表示)
	//resultDisPlaytime[1] = (timer_ % 100000) / 10000;
	////--4桁目(百の位を表示)
	//resultDisPlaytime[2] = (timer_ % 10000) / 1000;
	////--3桁目(十の位を表示)
	//resultDisPlaytime[3] = (timer_ % 1000) / 100;
	////--2桁目(1の位を表示)
	//resultDisPlaytime[4] = (timer_ % 100) / 10;
	////--1桁目(1の位を表示)
	//resultDisPlaytime[5] = (timer_ % 10) / 1;

	for(size_t i = 0; i < resultDigits; i++)
	{
		SetNumber(resultDisPlaytime[i], resultNum[i].get());
	}
}

void GameTimer::SetNumber(int number, Sprite* sprite)
{
	Vector2 texTotalSize = TextureManager::GetInstance()->GetTexSize("numbers.png");
	Vector2 texSize = texTotalSize;
	//一枚あたりの横サイズ
	texSize.x = texSize.x / totalNumber;

	if(number == 0)
	{
		sprite->SetTextureLeftTop(texSize * 0);
	}
	else if(number == 1)
	{
		sprite->SetTextureLeftTop(texSize * 1);
	}
	else if(number == 2)
	{
		sprite->SetTextureLeftTop(texSize * 2);
	}
	else if(number == 3)
	{
		sprite->SetTextureLeftTop(texSize * 3);
	}
	else if(number == 4)
	{
		sprite->SetTextureLeftTop(texSize * 4);
	}
	else if(number == 5)
	{
		sprite->SetTextureLeftTop(texSize * 5);
	}
	else if(number == 6)
	{
		sprite->SetTextureLeftTop(texSize * 6);
	}
	else if(number == 7)
	{
		sprite->SetTextureLeftTop(texSize * 7);
	}
	else if(number == 8)
	{
		sprite->SetTextureLeftTop(texSize * 8);
	}
	else if(number == 9)
	{
		sprite->SetTextureLeftTop(texSize * 9);
	}
}

void GameTimer::StopTimer()
{
	inGameTime_ = inGameTime_;
	resultTime_ = resultTime_;
}
