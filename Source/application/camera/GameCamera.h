#pragma once
#include "Camera.h"
#include "Vector2.h"

class GameCamera : public Camera
{
public:

	void Update(bool isPlayerMoving, bool isPlayerDead,Vector3 playerAxcell_,Vector3 playerPos);

	void Reset();

	void ImGuiUpdate();

	void GoalAnimation();

	bool GetIsFinishAnimation() { return isFinishAnimetion_; }

private:

	//アニメーションが終わったかどうか
	bool isFinishAnimetion_;

	//シーンアニメーション時間
	const float kSceneAnimeTime_ = 60.0f;
	//アニメーション時間の変化量
	float sceneAnimeTimer_ = kSceneAnimeTime_;
	//シーンアニメーション用変化量
	Vector3 sceneAnimationVec_;

};

