#pragma once
#include "Camera.h"
#include "Vector2.h"
#include "Easing.h"

class GameCamera : public Camera
{
public:

	void Initialize() override;

	void Update(bool isPlayerMoving,Vector3 playerPos, Vector3 playerInitPos,bool isDead, 
		Vector3 playerDeadPos,Vector3 playerAxel,bool onGround);

	void Reset();

	void ImGuiUpdate();

	void GoalAnimation();

	bool GetIsFinishAnimation() { return isFinishAnimetion_; }

private:

	const Vector3 initcameraEye = { 30,7.5,-20 };
	const Vector3 initcameraTarget = { 0,5,5 };
	const Vector3 initcameraUp = { 0,1,0 };

	const Vector3 initEyeDistance_ = { 0.0f,1.0f,12.0f };
	const Vector3 initTargetDistance_ = { 0.0f,1.0f,12.0f };

	//Y軸用の加速割合
	float EyeYAxelRate_ = 2.20f;

	EasingInfo speedEasing_ = { 1.0f, 0.225f, 0.0f, 15.0f };
	bool isFinishEasing_ = false;

	float cameraSpeed_;
	float cameraSpeedY_;

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

	Vector3 eyePosition_;
	Vector3 targetPosition_;

	Vector3 goalEyePos_;
	Vector3 goalEyeTarget_;

	Vector3 moveEyeVec;
	Vector3 moveEyeVecY;
	Vector3 moveTargetVec;
	Vector3 moveTargetVecY;

	int32_t kSlowTime_ = 30;
	int32_t slowTimer_;

	bool isStopTarget_ = false;
};

