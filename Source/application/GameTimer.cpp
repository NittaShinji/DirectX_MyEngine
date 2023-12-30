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
std::unique_ptr<Sprite> GameTimer::blackDot_;
std::unique_ptr<Sprite> GameTimer::stopWatch_;

//ゲーム中に表示する時間(4桁)
int GameTimer::inGameDisPlayTime_[inGameDigits];
//クリア画面で表示する時間(6桁)
int GameTimer::resultDisPlaytime[resultDigits];

void GameTimer::LoadSprite()
{
	//一度だけ画像を読み込む
	if(isLoadSprite_ == false)
	{
		//時計のテクスチャ読み込み
		TextureManager::GetInstance()->LoadTexture("StopWatch.png");
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
	
	blackDot_ = std::make_unique<Sprite>();
	stopWatch_ = std::make_unique<Sprite>();

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

	//10枚分のサイズ
	Vector2 texNumTotalSize = TextureManager::GetInstance()->GetTexSize("numbers.png");
	//数字一つ当たりのサイズ
	Vector2 texNumSize = texNumTotalSize;
	texNumSize.x = texNumSize.x / totalNumber;
	const int half = 2;
	const Vector4 color = { 1.0f,1.0f,1.0,0.75f };

	Vector2 texStopWatchSize = TextureManager::GetInstance()->GetTexSize("StopWatch.png");

	//数字の初期化
	for(int i = 0; i < inGameDigits; i++)
	{
		if(i < inGameDigits / half)
		{
			// 整数の初期化
			inGameNum[i]->Initialize("numbers.png", Vector2((i * texNumSize.x) + texNumSize.y, texNumSize.y));
		}
		else
		{
			// 小数点の初期化
			inGameNum[i]->Initialize("numbers.png", Vector2((i * texNumSize.x + texNumSize.x / half) + texNumSize.y, texNumSize.y));
		}

		//数字の桁を0で初期化
		inGameDisPlayTime_[i] = { 0 };
		SetNumber(inGameDisPlayTime_[i], inGameNum[i].get());
		//数字のサイズを指定
		inGameNum[i]->SetTextureClipSize(texNumSize);
		inGameNum[i]->SetSize(texNumSize);
		//透明度を下げる
		//inGameNum[i]->SetColor(color);
	}

	//ドット画像の初期化
	float decimalX = inGameNum[2]->GetPosition().x;
	Vector2 decimalPointPos = Vector2(decimalX - texNumSize.x / half + 6, texNumSize.y + texNumSize.y - 15);
	blackDot_->Initialize("BLACKDot", decimalPointPos);
	//blackDot_->SetColor(color);

	//時計画像の初期化
	float stopWatchX = inGameNum[0]->GetPosition().x - texNumSize.y;
	stopWatch_->Initialize("StopWatch.png", Vector2(stopWatchX, texNumSize.y));
	//64 x 64の正方形サイズに変更
	stopWatch_->SetSize(Vector2(texNumSize.y,texNumSize.y));
	stopWatch_->SetColor(color);


	timer_ = 0;
}

void GameTimer::ResultInitialize()
{
	Vector2 texNumTotalSize = TextureManager::GetInstance()->GetTexSize("numbers.png");
	Vector2 texNumSize = texNumTotalSize;
	const int half = 2;
	texNumSize.x = texNumSize.x / totalNumber;

	//float numberWidth = WindowsAPI::kWindow_height_ / 2;
	float numberHeight = WindowsAPI::kWindow_height_ / 2;

	//const float dotWidth = numberHeight + texNumSize.y - 15;
	const float dotHeight = numberHeight + texNumSize.y - 15;

	for(int i = 0; i < resultDigits; i++)
	{
		if(i < resultDigits / half)
		{
			// 数字の初期化
			resultNum[i]->Initialize("numbers.png", Vector2((i * texNumSize.x) + WindowsAPI::kWindow_width_ / 2 - (texNumSize.x * 3) , numberHeight));
		}
		else
		{
			// 数字の初期化
			resultNum[i]->Initialize("numbers.png", Vector2((i * texNumSize.x + texNumSize.x / half) + WindowsAPI::kWindow_width_ / 2 - (texNumSize.x * 3), numberHeight));
		}
	}

	for(int i = 0; i < resultDigits; i++)
	{
		resultNum[i]->SetTextureClipSize(texNumSize);
		resultNum[i]->SetSize(texNumSize);
	}

	float decimalX = resultNum[3]->GetPosition().x;

	Vector2 decimalPointPos = Vector2(decimalX - texNumSize.x / half + 6, dotHeight);

	blackDot_->SetPosition(decimalPointPos);

}

void GameTimer::InGameUpdate(bool isStart,bool isFinish)
{
	for(int i = 0; i < inGameDigits; i++)
	{
		inGameNum[i]->matUpdate();
	}

	blackDot_->matUpdate();
	stopWatch_->matUpdate();

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

	blackDot_->matUpdate();

	ResultNumberUpdate();

	for(size_t i = 0; i < resultDigits; i++)
	{
		SetNumber(resultDisPlaytime[i], resultNum[i].get());
	}
}

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
	stopWatch_->Draw("StopWatch.png");

	for(int i = 0; i < inGameDigits; i++)
	{
		inGameNum[i]->Draw("numbers.png");
	}

	blackDot_->Draw("BLACKDot");
}
void GameTimer::ResultDraw()
{
	for(int i = 0; i < resultDigits; i++)
	{
		resultNum[i]->Draw("numbers.png");
	}

	blackDot_->Draw("BLACKDot");
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