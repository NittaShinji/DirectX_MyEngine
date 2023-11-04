#pragma once
#include "Camera.h"
#include "Vector2.h"

class GameCamera : public Camera
{
public:

	void Initialize() override;

	void Update(bool isPlayerMoving, bool isPlayerDead,bool isPlayerStoped,Vector3 playerAxcell_,Vector3 playerPos);

	void Reset();

	void ImGuiUpdate();

	void GoalAnimation();

	bool GetIsFinishAnimation() { return isFinishAnimetion_; }

private:

	//カメラの加速値
	Vector3 totalAxcellSpeed_;

	//減速フラグ
	bool isSlowDown_;

	//減速するまでの時間
	const float kSlowDownTime_ = 5.0f;
	//減速タイマー
	float moveSlowTimer_ = kSlowDownTime_;

	//加速割合
	float axcelRate_;

	//アニメーションが終わったかどうか
	bool isFinishAnimetion_;

	//シーンアニメーション時間
	const float kSceneAnimeTime_ = 60.0f;
	//アニメーション時間の変化量
	float sceneAnimeTimer_ = kSceneAnimeTime_;
	//シーンアニメーション用変化量
	Vector3 sceneAnimationVec_;

};

