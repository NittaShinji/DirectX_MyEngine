#pragma once
#include <stdint.h>

//ゲームスピード
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

	void Initialize();
	void Update();

private: //メンバ変数

	SpeedMode speedMode_;

	const float kSlowSpeed_ = 0.1f;
	const float kNormalSpeed_ = 1.0f;
	const float kHardSpeed_ = 1.2f;
	const float kStopSpeed_ = 0.0f;

	float speedNum_;

	const float slowRate_ = 1.0f / kSlowSpeed_;
	int32_t slowCount_ = 0;
	bool canMoveInSlow_ = false;

public:	//アクセッサ

	float GetSpeedNum() { return speedNum_; };
	SpeedMode GetSpeedMode() { return speedMode_; }
	bool GetCanMoveInSlow() { return canMoveInSlow_; }

	void SetSpeedMode(SpeedMode speedMode);
	void SetSpeed();
};

