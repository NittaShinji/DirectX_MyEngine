#pragma once
#include <stdint.h>

/// <summary>
/// ゲームスピード
/// </summary>
class GameSpeed
{

public:

	enum class SpeedMode
	{
		SLOW,
		NORMAL,
		HARD,
		STOP,
	};

public:	//メンバ関数

	//初期化
	void Initialize();
	//更新
	void Update();

private: //メンバ変数

	//各スピードモードの定数
	const float kSlowSpeed_ = 0.1f;
	const float kNormalSpeed_ = 1.0f;
	const float kHardSpeed_ = 1.2f;
	const float kStopSpeed_ = 0.0f;

	//スピードモード
	SpeedMode speedMode_;

	//ゲーム全体のスピード値
	float speedNum_;

	//通常時に対する割合
	const float slowRate_ = kNormalSpeed_ / kSlowSpeed_;
	//スロー時にカウントする値
	int32_t slowCount_ = 0;
	//スロー中に動けるかどうかのフラグ
	bool canMoveInSlow_ = false;

public:	//アクセッサ

	/// <summary>
	/// //スピード値を取得
	/// </summary>
	/// <returns>スピード値</returns>
	float GetSpeedNum() { return speedNum_; };
	/// <summary>
	/// スピードモードを取得
	/// </summary>
	/// <returns>スピードモード</returns>
	SpeedMode GetSpeedMode() { return speedMode_; }

	/// <summary>
	/// スロー中に動けるかどうかのフラグを取得
	/// </summary>
	/// <returns></returns>
	bool GetCanMoveInSlow() { return canMoveInSlow_; }

	/// <summary>
	/// スピードモードをセット
	/// </summary>
	/// <param name="speedMode"></param>
	void SetSpeedMode(SpeedMode speedMode);

	/// <summary>
	/// スピードをセット
	/// </summary>
	void SetSpeed();
};

