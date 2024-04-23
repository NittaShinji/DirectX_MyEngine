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
	//クリア画面の初期化
	void ClearInitialize();
	//ハイスコアの初期化
	static void HighScoreInitialize();

	//ハイスコアの準備
	void ReadyHighScore(int32_t stageNum, int32_t endStageNum);

	//ゲーム中の数字を更新
	void InGameNumberUpdate(bool isFinish);
	//リザルト画面の数字を更新
	void ResultNumberUpdate();
	//リザルト画面での更新
	void ResultUpdate(bool isFinishedAnimation, float easingMoveY,int32_t stageNum,int32_t endStageNum);

	//トータルタイム更新
	void CalculateTotalTime();
	//ハイスコア更新
	void CalculateHighTime(int32_t stageNum, int32_t endStageNum);


	//ハイスコア画像更新
	void HighScoreSpriteUpdate();

	//ImGuiの更新
	void InGameUpdate(bool isStart, bool isFinish, bool isReachedStageEdge);

	//リセット
	void Reset();

	//チュートリアルリセット
	void TutorialReset();

	//トータルタイムリセット
	void TotalTimeReset();

	//ゲーム中の描画
	void InGameDraw();
	//結果画面での描画
	void ResultDraw();
	//クリア画面での描画
	void ClearDraw();
	//ハイスコアを描画
	void HighScoreDraw();

	//数字をセット
	static void SetNumber(int number,Sprite* sprite);

	//次のステージ用に準備をする
	void ReadytoNextStageTime(int32_t stageNum, int32_t endStageNum);

	//トータルステージタイムを更新
	void TotalNumberUpdate();

	//クリア画面更新
	void ClearUpdate(bool isFinishedAnimation);

	static void AddClearNum() { clearNum_ += 1; }

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

	static const int kTimerDigits_ = 4;

	static int32_t clearNum_;

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

	//トータル秒
	int32_t totalSeconds_;
	//トータル分
	int32_t totalMinutes_;
	//トータルカウント用秒
	int32_t totalCountSeconds_;
	//トータルカウント用分
	int32_t totalCountMinutes_;
	//ハイスコアタイム用秒
	int32_t highSeconds_[2];
	//ハイスコアタイム用分
	int32_t highMinutes_[2];
	//トータルハイスコアタイム用秒
	int32_t totalHeighSeconds_;
	//トータルハイスコアタイム用分
	int32_t totalHeighMinutes_;

	//リザルト初期化フラグ
	bool isStartedResultAnimation_;

	//チュートリアルで死んだとき用の秒
	int32_t saveTutorialSeconds_;
	//チュートリアルで死んだとき用の分
	int32_t saveTutorialMinutes_;

	//時間を計るかどうか
	bool isTimed_;

	//数える速度
	const int32_t kInitTimerIncreaseNum = 1;
	const float  resultTimerIncreaseRate_ = 2.5f;
	int32_t resultTimerIncreaseNum_;

	//数え終わったかどうか
	bool isFinishedToTime_;

	//色を変えるかどうか
	bool canChangeColor_;
	const int32_t kChangeTime_ = 60;
	int32_t changeTime_ = 0;

	//時間計算用の数字画像
	static std::unique_ptr<Sprite> inGameNum[inGameDigits];
	static std::unique_ptr<Sprite> resultNum[resultDigits];
	static std::unique_ptr<Sprite> totalNum[resultDigits];
	static std::unique_ptr<Sprite> highScoreNum_[resultDigits];
	//小数点画像
	static std::unique_ptr<Sprite> inGameBlackDot_;
	static std::unique_ptr<Sprite> inGameGrayDot_;
	static std::unique_ptr<Sprite> resultBlackDot_;
	static std::unique_ptr<Sprite> resultGrayDot_;
	static std::unique_ptr<Sprite> highScoreBlackDot_;
	static std::unique_ptr<Sprite> highScoreGrayDot_;

	//王冠画像
	static std::unique_ptr<Sprite> crownSprite_;
	//時計画像
	static std::unique_ptr<Sprite> stopWatch_;

	//ゲーム中に表示する時間(4桁)
	static int inGameDisPlayTime_[inGameDigits];
	//結果用に表示する時間(6桁)
	static int resultDisPlaytime[resultDigits];
	//クリア画面で表示する時間(6桁)
	static int totalDisPlaytime[resultDigits];
	//ハイスコア
	static int highScorePlaytime[resultDigits];

	//最大ステージ数
	int32_t endStageNum_;

public: //アクセッサ

	void SetIsTImed(bool isTimed) { isTimed_ = isTimed; }
	bool GetIsFinishedToTime() { return isFinishedToTime_; }

};

