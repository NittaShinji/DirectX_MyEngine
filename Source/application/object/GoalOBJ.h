#pragma once
#include "TouchableObject.h"
#include "LevelManager.h"
#include "ObjectAttribute.h"

class GoalOBJ : public TouchableObject
{
public:

	static std::unique_ptr<GoalOBJ> Create(const std::string& fileName, const unsigned short coliderAttribute);

	void OnCollision(const CollisionInfo& info) override;

	void Initialize();

	void Update(Camera* camera, Vector3 goalPos);

	void Draw() override;

	bool GetIsBreak_() { return isBreak_; }
	void SetIsBreak(bool isBreak) { isBreak_ = isBreak; }

	Vector3 GetWallPos() { return transform_; }
	bool GetIsCheckChangeColor() { return isCheckChangeColor_; }
	bool GetIsStartGoalStagin() { return isStartGoalStagin_; }
	void SetIsStartGoalStagin(bool isStartGoalStagin) { isStartGoalStagin_ = isStartGoalStagin; }

private:

	//プレイヤーが近づいた際に演出を入れる処理
	const float kStagingStartDistance_ = 7.0f;

	//演出開始フラグ
	bool isStartGoalStagin_ = false;

	//壁が壊れているか
	bool isBreak_;

	//プレイヤーが加速しているか
	bool isPlayerAccelerating_;

	bool isCheckChangeColor_;
};


