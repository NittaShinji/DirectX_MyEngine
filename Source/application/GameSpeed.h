#pragma once

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
	

private: //メンバ変数

	SpeedMode speedMode_;

	const float kSlowSpeed_ = 0.5f;
	const float kNormalSpeed_ = 1.0f;
	const float kHardSpeed_ = 1.2f;
	const float kStopSpeed_ = 0.0f;

	float speedNum_;

public:	//アクセッサ

	float GetSpeedNum() { return speedNum_; };
	SpeedMode GetSpeedMode() { return speedMode_; }

	void SetSpeedMode(SpeedMode speedMode);
	void SetSpeed();
};

