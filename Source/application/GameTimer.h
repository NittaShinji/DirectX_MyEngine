#pragma once
#include "Sprite.h"
class GameTimer
{

public:

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//コピーコンストラクタ
	GameTimer(const GameTimer&) = delete;
	//コピー代入演算子
	GameTimer& operator=(const GameTimer&) = delete;
	//ムーブコンストラクタ
	GameTimer(GameTimer&&) = delete;
	//ムーブ代入演算子
	GameTimer& operator = (GameTimer&&) = delete;

	//静的オブジェクトとしてインスタンスを生成
	static GameTimer* GetInstance()
	{
		static GameTimer instance;
		return &instance;
	}

public: 

	void StaticInitialize();
	void InGameInitialize();
	void ResultInitialize();

	//void Update(bool isStart);

	void InGameUpdate(bool isStart, bool isFinish);
	void ResultUpdate();

	void Reset();

	void InGameDraw();
	void ResultDraw();

	void LoadSprite();

	static void Draw();

	void NumberUpdate();
	void InGameNumberUpdate(bool isFinish);
	void ResultNumberUpdate();

	void SetNumber(int number,Sprite* sprite);

	void StopTimer();

private:

	//コンストラクタ
	GameTimer() = default;
	//デストラクタ
	~GameTimer() = default;

private:

	//1秒
	static const int kOneSeconds_ = 60;

	//MAX時間
	static const int kMaxTime_ = 1000;

	//数字の数
	static const int totalNumber = 10;
	//インゲーム中に表示する桁
	static const int inGameDigits = 4;
	//リザルト表示する桁
	static const int resultDigits = 6;

	//ゲーム中に表示する時間(4桁)
	static int inGameTime_;
	//クリア画面で表示する時間(6桁)
	static int resultTime_;
	//1フレームの時間を計算するタイマー
	static int timer_;
	//ハイスコア
	static int highScoreTime_;

	//画像読み込みフラグ
	static bool isLoadSprite_;

	//リザルトで足していくときのタイマー
	//float inGameTimer;
	//float resultTimer;

	//std::unique_ptr<Sprite> clock_;
	static std::unique_ptr<Sprite> inGameNum[inGameDigits];
	static std::unique_ptr<Sprite> resultNum[resultDigits];
	static std::unique_ptr<Sprite> BlackDot_;

	//ゲーム中に表示する時間(4桁)
	static int inGameDisPlayTime_[inGameDigits];
	//クリア画面で表示する時間(6桁)
	static int resultDisPlaytime[resultDigits];
};

