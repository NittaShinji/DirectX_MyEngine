#pragma once
#include "Camera.h"
#include "Vector2.h"
#include "Easing.h"
#include "GameSpeed.h"

/// <summary>
/// ゲームカメラ
/// </summary>
class GameCamera : public Camera
{
public:

	void Initialize() override;

	void Update(bool isPlayerMoving,Vector3 playerPos, Vector3 playerInitPos,bool isDead, 
		Vector3 playerDeadPos,bool rightAxcell);

	void Reset();

	void ImGuiUpdate();

	void GoalAnimation();

	bool GetIsFinishAnimation() { return isFinishAnimetion_; }

	void SetGameSpeed(GameSpeed* gameSpeed) { gameSpeed_ = gameSpeed; }

private:

	const Vector3 initCameraEye = { 30,7.5,780 };
	const Vector3 initCameraTarget = { 0,5,805 };
	const Vector3 initCameraUp = { 0,1,0 };

	Vector3 initEyeDistance_ = { 0.0f,1.0f,12.0f };
	const Vector3 initTargetDistance_ = { 0.0f,1.0f,12.0f };

	//Y軸用の加速割合
	float EyeYAxelRate_ = 2.20f;

	EasingInfo speedEasing_ = { 1.0f, 0.225f, 0.0f, 15.0f };
	bool isStartGoalEasing_ = false;
	bool isFinishEasing_ = false;

	const float kInitCameraSpeed_ = 1.0f;

	float cameraSpeed_;
	float cameraSpeedY_;

	//アニメーションが終わったかどうか
	bool isFinishAnimetion_;

	//シーンアニメーション時間
	const float kSceneAnimeTime_ = 60.0f;
	//アニメーション時間の変化量
	float sceneAnimeTimer_ = 0.0f;

	const float kInitSceneAnimeTime_ = 0.0f;

	//シーンアニメーション用変化量
	Vector3 sceneAnimationVec_;

	//ゴールした時のカメラの位置
	Vector3 goalEyePos_;
	Vector3 goalEyeTarget_;

	Vector3 moveEyeVec_;
	Vector3 moveEyeVecY_;
	Vector3 moveTargetVec_;
	Vector3 moveTargetVecY_;

	int32_t kSlowTime_ = 30;
	int32_t slowTimer_;

	bool isStopTarget_ = false;

	const int32_t kAxcellWaitTime = 60;
	int32_t axcellWaitTimer;
	bool isAxcellAnimation_;

	const float kAxcellNormalRate_ = 1.0f;
	//加速割合
	float axcellRate_ = 1.0f;
	
	bool isNotBackAnimation_;
	bool isSlowDown_;
	
	const float axcellRateMoveValue_ = 0.0015f;
	const float decelerationMoveValue_ = 0.01f;

	const int32_t kInitTime_ = 0;
	const float goalEyeXMoveValue_ = 0.0f;
	const float goalEyeYMoveValue_ = 0.0f;
	const float goalEyeZMoveValue_ = 0.0f;

	EasingInfo goalEyeXEasing_ = { 0.0f, goalEyeXMoveValue_, sceneAnimeTimer_, kSceneAnimeTime_ };
	EasingInfo goalEyeYEasing_ = { 0.0f, goalEyeYMoveValue_, sceneAnimeTimer_, kSceneAnimeTime_ };
	EasingInfo goalEyeZEasing_ = { 0.0f, goalEyeZMoveValue_, sceneAnimeTimer_, kSceneAnimeTime_ };

	EasingInfo slowDownEasing_ = { 1.0f, -0.2f, 0.0f, 15.0f };
	GameSpeed* gameSpeed_ = nullptr;
};

