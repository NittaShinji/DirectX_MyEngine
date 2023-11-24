#include "GameSpeed.h"

void GameSpeed::Initialize()
{
	speedMode_ = SpeedMode::NORMAL;
	SetSpeed();
}

void GameSpeed::SetSpeed()
{
	if(speedMode_ == SpeedMode::SLOW) 
	{
		speedNum_ = kSlowSpeed_;
	}
	else if(speedMode_ == SpeedMode::NORMAL)
	{
		speedNum_ = kNormalSpeed_;
	}
	else if(speedMode_ == SpeedMode::HARD)
	{
		speedNum_ = kHardSpeed_;
	}
	else if(speedMode_ == SpeedMode::STOP)
	{
		speedNum_ = kStopSpeed_;
	}
}

void GameSpeed::SetSpeedMode(SpeedMode speedMode)
{
	speedMode_ = speedMode;
	SetSpeed();
}

