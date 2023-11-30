#pragma once
#include "Sprite.h"
class GameTimer
{
public: 

	void Initialize();
	void InGameInitialize();
	void ResultInitialize();

	void Update(bool isStart);
	void Reset();
	void LoadNumTexture();
	void Draw();

	void NumberUpdate();
	void InGameNumberUpdate();
	void ResultNumberUpdate();

	void SetNumber(int number,Sprite* sprite);


private:

	//時間の位置(等距離感覚)
	const float equalDistanceX = 2.0f;
	const float equalDistanceY = 0.0f;

	//数字の数
	const int totalNumber = 10;
	//インゲーム中に表示する桁
	static const int inGameDigits = 4;
	//リザルト表示する桁
	static const int resultDigits = 7;

	//ゲーム中に表示する時間(4桁)
	int inGameTime_;
	//クリア画面で表示する時間(6桁)
	int resultTime_;
	//ハイスコア
	int highScoreTime_;

	//リザルトで足していくときのタイマー
	//float inGameTimer;
	//float resultTimer;

	//std::unique_ptr<Sprite> clock_;
	std::unique_ptr<Sprite> inGameNum[inGameDigits];
	std::unique_ptr<Sprite> resultNum[resultDigits];

	//ゲーム中に表示する時間(4桁)
	int inGameDisPlayTime_[inGameDigits];
	//クリア画面で表示する時間(6桁)
	int resultDisPlaytime[resultDigits];
};

