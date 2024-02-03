#pragma once
#include "Sprite.h"

/// <summary>
/// ゲームタイマー
/// </summary>
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

	//静的初期化
	void StaticInitialize();
	//ゲーム中の初期化
	void InGameInitialize();
	//リザルト画面の初期化
	void ResultInitialize(float imagePosY);

	//ゲーム中の数字を更新
	void InGameNumberUpdate(bool isFinish);
	//リザルト画面の数字を更新
	void ResultNumberUpdate();

	//リザルト画面での更新
	void ResultUpdate(bool isFinishedAnimation, float easingMoveY);

	//ImGuiの更新
	void InGameUpdate(bool isStart, bool isFinish);

	//リセット
	void Reset(float imagePosY);

	//ゲーム中の描画
	void InGameDraw();
	//結果画面での描画
	void ResultDraw();

	//スプライト読み込み
	void LoadSprite();

	//数字をセット
	void SetNumber(int number,Sprite* sprite);

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
	static const int resultDigits = 4;

	//ゲーム中に表示する分
	static int gameMinutes_;
	//クリア画面で表示する分
	static int resultMinutes_;
	//ゲーム中に表示する秒
	int32_t gameSeconds_;
	//ゲーム中に表示する秒
	int32_t resultSeconds_;

	//保存用時間
	int32_t keepSeconds_;
	int32_t keepMinutes_;

	//1フレームの時間を計算するタイマー
	static int timer_;
	//ハイスコア
	static int highScoreTime_;

	//画像読み込みフラグ
	static bool isLoadSprite_;

	//リザルト初期化フラグ
	bool isStartedResultAnimation_;

	//時間を計るかどうか
	bool isTimed_;

	//数える速度
	const int32_t kInitTimerIncreaseNum = 1;
	const float  resultTimerIncreaseRate_ = 2.5f;
	int32_t resultTimerIncreaseNum_;


	static std::unique_ptr<Sprite> inGameNum[inGameDigits];
	static std::unique_ptr<Sprite> resultNum[resultDigits];
	static std::unique_ptr<Sprite> inGameBlackDot_;
	static std::unique_ptr<Sprite> resultBlackDot_;

	static std::unique_ptr<Sprite> stopWatch_;

	//ゲーム中に表示する時間(4桁)
	static int inGameDisPlayTime_[inGameDigits];
	//クリア画面で表示する時間(6桁)
	static int resultDisPlaytime[resultDigits];

public: //アクセッサ

	void SetIsTImed(bool isTimed) { isTimed_ = isTimed; }

};

