#include "GameTimer.h"
#include "TextureManager.h"

using namespace NsEngine;

//ゲーム中に表示する時間(4桁)
int GameTimer::gameMinutes_;
//クリア画面で表示する時間(6桁)
int GameTimer::timer_ = 0;
int GameTimer::resultMinutes_;
//ハイスコア
int GameTimer::highScoreTime_;

std::unique_ptr<Sprite> GameTimer::inGameNum[kTimerDigits_];
std::unique_ptr<Sprite> GameTimer::resultNum[kTimerDigits_];
std::unique_ptr<Sprite> GameTimer::inGameBlackDot_;
std::unique_ptr<Sprite> GameTimer::resultBlackDot_;
std::unique_ptr<Sprite> GameTimer::inGameGrayDot_;
std::unique_ptr<Sprite> GameTimer::resultGrayDot_;
std::unique_ptr<Sprite> GameTimer::stopWatch_;

//ゲーム中に表示する時間(4桁)
int GameTimer::inGameDisPlayTime_[kTimerDigits_];
//クリア画面で表示する時間(6桁)
int GameTimer::resultDisPlaytime[kTimerDigits_];

void GameTimer::StaticInitialize()
{
	//メンバ変数初期化
	gameMinutes_ = 0;
	resultMinutes_ = 0;

	highScoreTime_ = 0;

	inGameBlackDot_ = std::make_unique<Sprite>();
	resultBlackDot_ = std::make_unique<Sprite>();
	inGameGrayDot_ = std::make_unique<Sprite>();
	resultGrayDot_ = std::make_unique<Sprite>();

	stopWatch_ = std::make_unique<Sprite>();

	for(int i = 0; i < kTimerDigits_; i++)
	{
		inGameNum[i] = std::make_unique<Sprite>();
	}
	for(int i = 0; i < kTimerDigits_; i++)
	{
		resultNum[i] = std::make_unique<Sprite>();
	}
}

void GameTimer::InGameInitialize()
{
	gameSeconds_ = 0;
	gameMinutes_ = 0;

	//10枚分のサイズ
	Vector2 texNumTotalSize = TextureManager::GetInstance()->GetTexSize("numbers.png");
	//数字一つ当たりのサイズ
	Vector2 texNumSize = texNumTotalSize;
	texNumSize.x = texNumSize.x / totalNumber;
	const int half = 2;
	const Vector4 watchColor = { 1.0f,1.0f,1.0,0.95f };

	Vector2 texStopWatchSize = TextureManager::GetInstance()->GetTexSize("stopWatch.png");

	//数字の初期化
	for(int i = 0; i < kTimerDigits_; i++)
	{
		if(i < kTimerDigits_ / half)
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
	const int8_t blackDotShift = 1;
	
	//黒ドットの下に1ドット分大きい白ドットを置くので、プラス1座標をずらす確保
	inGameBlackDot_->Initialize("BlackDot", Vector2(decimalPointPos.x + blackDotShift, decimalPointPos.y + blackDotShift));
	inGameBlackDot_->SetPosition(Vector2(decimalPointPos.x + blackDotShift, decimalPointPos.y + blackDotShift));
	inGameGrayDot_->Initialize("GrayDot", decimalPointPos);
	inGameGrayDot_->SetPosition(decimalPointPos);

	//時計画像の初期化
	float stopWatchX = inGameNum[0]->GetPosition().x - texNumSize.y;
	stopWatch_->Initialize("stopWatch.png", Vector2(stopWatchX, texNumSize.y));
	//64 x 64の正方形サイズに変更
	stopWatch_->SetSize(Vector2(texNumSize.y, texNumSize.y));
	stopWatch_->SetColor(watchColor);

	gameSeconds_ = 0;
	isTimed_ = false;
}

void GameTimer::ResultInitialize(float imagePosY)
{
	keepSeconds_ = 0;
	keepMinutes_ = 0;
	resultMinutes_ = 0;
	resultSeconds_ = 0;

	Vector2 texNumTotalSize = TextureManager::GetInstance()->GetTexSize("numbers.png");
	Vector2 texNumSize = texNumTotalSize;
	const int half = 2;
	texNumSize.x = texNumSize.x / totalNumber;
	const int decimalBeforeNumber = 3;

	for(int i = 0; i < kTimerDigits_; i++)
	{
		//数字全体の半分の位置で小数点で分割
		if(i < kTimerDigits_ / half)
		{
			// 数字の初期化
			resultNum[i]->Initialize("numbers.png", Vector2((i * texNumSize.x) + WindowsAPI::kWindow_width_ / 2 - (texNumSize.x * 2) - 11, imagePosY));
		}
		else
		{
			// 数字の初期化
			resultNum[i]->Initialize("numbers.png", Vector2((i * texNumSize.x + texNumSize.x / half) + WindowsAPI::kWindow_width_ / 2 - (texNumSize.x * 2) - 11, imagePosY));
		}
	}

	//1～9までの画像を切り抜いてセット
	for(int i = 0; i < kTimerDigits_; i++)
	{
		resultNum[i]->SetTextureClipSize(texNumSize);
		resultNum[i]->SetSize(texNumSize);
	}

	float decimalX = resultNum[decimalBeforeNumber]->GetPosition().x;
	const float dotHeight = resultNum[0]->GetPosition().y + texNumSize.y - 15;
	//小数点の位置を設定
	Vector2 decimalPointPos = Vector2(decimalX - texNumSize.x - 7, dotHeight);
	const int8_t blackDotShift = 1;

	resultBlackDot_->Initialize("BlackDot", Vector2(decimalPointPos.x + blackDotShift, decimalPointPos.y + blackDotShift));
	resultBlackDot_->SetPosition(Vector2(decimalPointPos.x + blackDotShift, decimalPointPos.y + blackDotShift));
	resultGrayDot_->Initialize("GrayDot", decimalPointPos);
	resultGrayDot_->SetPosition(decimalPointPos);

	//描画する数字を0で初期化
	for(size_t i = 0; i < kTimerDigits_; i++)
	{
		resultDisPlaytime[i] = { 0 };
		SetNumber(resultDisPlaytime[i], resultNum[i].get());
	}

	//増加量を初期値にリセット
	resultTimerIncreaseNum_ = kInitTimerIncreaseNum;

	//数え終わったかどうかを初期化
	isFinishedToTime_ = false;
}

void GameTimer::InGameUpdate(bool isStart, bool isFinish,bool isReachedStageEdge)
{
	//画像を更新
	for(int i = 0; i < kTimerDigits_; i++)
	{
		inGameNum[i]->matUpdate();
	}

	inGameBlackDot_->matUpdate();
	inGameGrayDot_->matUpdate();
	stopWatch_->matUpdate();

	//プレイヤーが動き始めたら
	if(isStart == true && isReachedStageEdge == true)
	{
		if(isTimed_ == true)
		{
			//インゲーム中の数字を更新
			InGameNumberUpdate(isFinish);
		}
	}
	else
	{
		gameSeconds_ = 0;
		for(size_t i = 0; i < kTimerDigits_; i++)
		{
			inGameDisPlayTime_[i] = { 0 };
			SetNumber(inGameDisPlayTime_[i], inGameNum[i].get());
		}
	}
}

void GameTimer::ResultUpdate(bool isFinishedAnimation, float easingMoveY, int32_t stageNum, int32_t endStageNum)
{
	keepSeconds_ = gameSeconds_;
	keepMinutes_ = gameMinutes_;

	//結果タイマーを結果画面の背景に合わせて移動
	for(int i = 0; i < kTimerDigits_; i++)
	{
		resultNum[i]->SetPosition(Vector2(resultNum[i]->GetPosition().x, easingMoveY));
		resultNum[i]->matUpdate();
	}

	Vector2 texNumTotalSize = TextureManager::GetInstance()->GetTexSize("numbers.png");
	Vector2 texNumSize = texNumTotalSize;
	const float dotHeight = resultNum[0]->GetPosition().y + texNumSize.y - 15;

	resultBlackDot_->SetPosition(Vector2(resultBlackDot_->GetPosition().x, dotHeight));
	resultBlackDot_->matUpdate();
	resultGrayDot_->SetPosition(Vector2(resultGrayDot_->GetPosition().x, dotHeight));
	resultGrayDot_->matUpdate();

	//結果画面のイージングが終了したら数字を更新
	if(isFinishedAnimation == true)
	{
		if (isFinishedToTime_ == false)
		{
			ResultNumberUpdate();
		}
	}
	//最終ステージであればドラムロールしないで直接タイムを出す
	else if (stageNum == endStageNum)
	{
		//--4桁目(千の位を表示)
		resultDisPlaytime[0] = (keepMinutes_ % 100) / 10;
		//--3桁目(百の位を表示)
		resultDisPlaytime[1] = (keepMinutes_ % 10) / 1;
		//
		resultDisPlaytime[2] = (keepSeconds_ % 100) / 10;
		//--2桁目(十の位を表示)
		resultDisPlaytime[3] = (keepSeconds_ % 10) / 1;
		//--1桁目(1の位を表示)
		isFinishedToTime_ = true;
	}

	//描画する数字をセット
	for(size_t i = 0; i < kTimerDigits_; i++)
	{
		SetNumber(resultDisPlaytime[i], resultNum[i].get());
	}
}

void GameTimer::Reset()
{
	resultTimerIncreaseNum_ = kInitTimerIncreaseNum;
	//ゲーム中にリザルトに受け渡す用のタイマー
	keepSeconds_ = 0;
	keepMinutes_ = 0;
	//ゲーム中に表示するタイマー
	gameSeconds_ = 0;
	gameMinutes_ = 0;
	//結果用に表示するタイマー
	resultMinutes_ = 0;
	resultSeconds_ = 0;

	isTimed_ = true;
	isFinishedToTime_ = false;

	
	//描画する数字を0で初期化(同じ桁なので同じfor分で回す)
	for(size_t i = 0; i < kTimerDigits_; i++)
	{
		//数字の桁を0で初期化
		inGameDisPlayTime_[i] = { 0 };
		SetNumber(inGameDisPlayTime_[i], inGameNum[i].get());

		resultDisPlaytime[i] = { 0 };
		SetNumber(resultDisPlaytime[i], resultNum[i].get());
	}
}

void GameTimer::InGameDraw()
{
	SpriteCommon::GetInstance()->BeforeDraw();
	//ゲーム中の数字・ドットを描画
	stopWatch_->Draw("stopWatch.png");

	for(int i = 0; i < kTimerDigits_; i++)
	{
		inGameNum[i]->Draw("numbers.png");
	}

	inGameGrayDot_->Draw("GrayDot");
	inGameBlackDot_->Draw("BlackDot");
}
void GameTimer::ResultDraw()
{
	SpriteCommon::GetInstance()->BeforeDraw();
	//リザルト画面の数字・ドットを描画
	for(int i = 0; i < kTimerDigits_; i++)
	{
		resultNum[i]->Draw("numbers.png");
	}

	resultGrayDot_->Draw("GrayDot");
	resultBlackDot_->Draw("BlackDot");
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

	for(size_t i = 0; i < kTimerDigits_; i++)
	{
		SetNumber(inGameDisPlayTime_[i], inGameNum[i].get());
	}
}

void GameTimer::ResultNumberUpdate()
{
	//リザルト画面の数字を更新
	if(resultMinutes_ < keepMinutes_)
	{
		resultSeconds_ += resultTimerIncreaseNum_;
		if(resultSeconds_ >= kOneSeconds_)
		{
			resultSeconds_ = 0;
			resultMinutes_++;

			const int32_t maxIncreaseNum = 60;

			if(resultTimerIncreaseNum_ < maxIncreaseNum)
			{
				resultTimerIncreaseNum_ = std::abs(int32_t(resultTimerIncreaseNum_ * resultTimerIncreaseRate_));
			}
			else
			{
				resultTimerIncreaseNum_ = maxIncreaseNum;
			}
		}
	}

	if(resultMinutes_ == keepMinutes_)
	{
		if(resultSeconds_ < keepSeconds_)
		{
			resultSeconds_ += resultTimerIncreaseNum_;
		}
		else
		{
			isFinishedToTime_ = true;
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
	for(size_t i = 0; i < kTimerDigits_; i++)
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

	//引数から画像を切り抜いてセットする
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
