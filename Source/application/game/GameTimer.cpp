#include "GameTimer.h"
#include "TextureManager.h"

//ゲーム中に表示する時間(4桁)
int GameTimer::gameMinutes_;
//クリア画面で表示する時間(6桁)
int GameTimer::timer_ = 0;
int GameTimer::resultMinutes_;
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
	gameMinutes_ = 0;
	resultMinutes_ = 0;

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
	}

	//ドット画像の初期化
	float decimalX = inGameNum[2]->GetPosition().x;
	Vector2 decimalPointPos = Vector2(decimalX - texNumSize.x / half + 6, texNumSize.y + texNumSize.y - 15);
	blackDot_->Initialize("BLACKDot", decimalPointPos);

	//時計画像の初期化
	float stopWatchX = inGameNum[0]->GetPosition().x - texNumSize.y;
	stopWatch_->Initialize("StopWatch.png", Vector2(stopWatchX, texNumSize.y));
	//64 x 64の正方形サイズに変更
	stopWatch_->SetSize(Vector2(texNumSize.y,texNumSize.y));
	stopWatch_->SetColor(color);

	gameSeconds_ = 0;
}

void GameTimer::ResultInitialize(float imagePosY)
{
	Vector2 texNumTotalSize = TextureManager::GetInstance()->GetTexSize("numbers.png");
	Vector2 texNumSize = texNumTotalSize;
	const int half = 2;
	texNumSize.x = texNumSize.x / totalNumber;
	const int decimalBeforeNumber = 3;

	//float numberHeight = WindowsAPI::kWindow_height_ / 2;
	
	for(int i = 0; i < resultDigits; i++)
	{
		//数字全体の半分の位置で小数点で分割
		if(i < resultDigits / half)
		{
			// 数字の初期化
			resultNum[i]->Initialize("numbers.png", Vector2((i * texNumSize.x) + WindowsAPI::kWindow_width_ / 2 - (texNumSize.x * 3) , imagePosY));
		}
		else
		{
			// 数字の初期化
			resultNum[i]->Initialize("numbers.png", Vector2((i * texNumSize.x + texNumSize.x / half) + WindowsAPI::kWindow_width_ / 2 - (texNumSize.x * 3), imagePosY));
		}
	}

	//1～9までの画像を切り抜いてセット
	for(int i = 0; i < resultDigits; i++)
	{
		resultNum[i]->SetTextureClipSize(texNumSize);
		resultNum[i]->SetSize(texNumSize);
	}

	float decimalX = resultNum[decimalBeforeNumber]->GetPosition().x;

	const float dotHeight = resultNum[0]->GetPosition().y + texNumSize.y - 15;

	//小数点の位置を設定
	Vector2 decimalPointPos = Vector2(decimalX - texNumSize.x - 7, dotHeight);
	blackDot_->SetPosition(decimalPointPos);
}

void GameTimer::InGameUpdate(bool isStart,bool isFinish)
{
	//画像を更新
	for(int i = 0; i < inGameDigits; i++)
	{
		inGameNum[i]->matUpdate();
	}

	blackDot_->matUpdate();
	stopWatch_->matUpdate();

	//クリア時間を保存
	if(isFinish == true)
	{
		keepSeconds_ = gameSeconds_;
		keepMinutes_ = gameMinutes_;
	}
	else
	{
		//プレイヤーが動き始めたら
		if(isStart == true)
		{
			//インゲーム中の数字を更新
			InGameNumberUpdate(isFinish);
		}
		else
		{
			gameSeconds_ = 0;
			for(size_t i = 0; i < inGameDigits; i++)
			{
				inGameDisPlayTime_[i] = { 0 };
				SetNumber(inGameDisPlayTime_[i], inGameNum[i].get());
			}
		}
	}
}

void GameTimer::ResultUpdate(bool isFinishedAnimation,float easingMoveY)
{
	if(isStartedResultAnimation_ == false)
	{
		ResultInitialize(easingMoveY);
		isStartedResultAnimation_ = true;
	}

	//画像を更新
	for(int i = 0; i < resultDigits; i++)
	{
		resultNum[i]->SetPosition(Vector2(resultNum[i]->GetPosition().x, easingMoveY));
		resultNum[i]->matUpdate();
	}

	Vector2 texNumTotalSize = TextureManager::GetInstance()->GetTexSize("numbers.png");
	Vector2 texNumSize = texNumTotalSize;
	const float dotHeight = resultNum[0]->GetPosition().y + texNumSize.y - 15;

	blackDot_->SetPosition(Vector2(blackDot_->GetPosition().x, dotHeight));
	blackDot_->matUpdate();

	//数字を更新
	if(isFinishedAnimation == true)
	{
		ResultNumberUpdate();
	}

	//描画する数字をセット
	for(size_t i = 0; i < resultDigits; i++)
	{
		SetNumber(resultDisPlaytime[i], resultNum[i].get());
	}
}

void GameTimer::Reset()
{
	gameSeconds_ = 0;
	gameMinutes_ = 0;
	resultMinutes_ = 0;
	//ゲーム中に表示する秒
	gameSeconds_ = 0;
	//ゲーム中に表示する秒
	resultSeconds_ = 0;

	isStartedResultAnimation_ = false;

	InGameInitialize();
}

void GameTimer::InGameDraw()
{
	//ゲーム中の数字・ドットを描画
	stopWatch_->Draw("StopWatch.png");

	for(int i = 0; i < inGameDigits; i++)
	{
		inGameNum[i]->Draw("numbers.png");
	}

	blackDot_->Draw("BLACKDot");
}
void GameTimer::ResultDraw()
{
	if(isStartedResultAnimation_ == true)
	{
		//リザルト画面の数字・ドットを描画
		for(int i = 0; i < resultDigits; i++)
		{
			resultNum[i]->Draw("numbers.png");
		}

		blackDot_->Draw("BLACKDot");
	}
}

void GameTimer::InGameNumberUpdate(bool isFinish)
{
	if(isFinish == false)
	{
		//t 経過時間	b最初の位置	c移動量	d移動時間
		if(gameMinutes_ <= kMaxTime_)
		{
			gameSeconds_++;
			if(gameSeconds_ >= kOneSeconds_)
			{
				gameSeconds_ = 0;
				gameMinutes_++;
			}
		}
	}

	//--4桁目(千の位を表示)
	inGameDisPlayTime_[0] = (gameMinutes_ % 100) / 10;
	//--3桁目(百の位を表示)
	inGameDisPlayTime_[1] = (gameMinutes_ % 10) / 1;

	//--2桁目(十の位を表示)
	inGameDisPlayTime_[2] = (gameSeconds_ % 100) / 10;
	//--1桁目(1の位を表示)
	inGameDisPlayTime_[3] = (gameSeconds_ % 10) / 1;

	for(size_t i = 0; i < inGameDigits; i++)
	{
		SetNumber(inGameDisPlayTime_[i], inGameNum[i].get());
	}
}

void GameTimer::ResultNumberUpdate()
{
	//リザルト画面の数字を更新
	if(resultMinutes_ < keepMinutes_)
	{
		resultSeconds_++;
		if(resultSeconds_ >= kOneSeconds_)
		{
			resultSeconds_ = 0;
			resultMinutes_++;
		}
	}

	if(resultMinutes_ == keepMinutes_)
	{
		if(resultSeconds_ < keepSeconds_)
		{
			resultSeconds_++;
		}
	}

	//--4桁目(千の位を表示)
	resultDisPlaytime[0] = (resultMinutes_ % 100) / 10;
	//--3桁目(百の位を表示)
	resultDisPlaytime[1] = (resultMinutes_ % 10) / 1;
	//
	resultDisPlaytime[2] = (resultSeconds_ % 100) / 10;
	//--2桁目(十の位を表示)
	resultDisPlaytime[3] = (resultSeconds_ % 10) / 1;
	//--1桁目(1の位を表示)

	//数字をセット
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