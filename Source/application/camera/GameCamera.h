#pragma once
#include "Camera.h"
#include "Vector2.h"

class GameCamera : public Camera
{
public:

	void Initialize() override;

	void Update(bool isPlayerMoving, bool isPlayerDead, bool isPlayerStoped, Vector3 playerAxcell_, Vector3 playerPos, Vector3 playerInitPos, float playerNormalAxcell);

	void Reset();

	void ImGuiUpdate();

	void GoalAnimation();

	bool GetIsFinishAnimation() { return isFinishAnimetion_; }

private:

	const Vector3 initcameraEye = { 30,7.5,-20 };
	const Vector3 initcameraTarget = { 0,5,5 };
	const Vector3 initcameraUp = { 0,1,0 };

	const Vector3 initEyeDistance = { 0.0f,0.0f,12.0f };
	const Vector3 initTargetDistance = { 0,0,12.0f };

	//カメラの加速値
	//Vector3 totalAxcellSpeed_;

	//減速フラグ
	bool isSlowDown_;

	float cameraSpeed_;

	//減速するまでの時間
	const float kSlowDownTime_ = 30.0f;
	//const float kReturnTime_ = 5.0f;

	//減速タイマー
	float moveSlowTimer_ = kSlowDownTime_;
	//float moveSlowCount_ = 0.0f;

	//float returnTimer_ = kReturnTime_;

	//int32_t stopTimer_;
	//const int32_t kStopTime_ = 20;

	////減速から通常時に戻るまでのフレーム数
	//const float kBackNormalTime_ = 60.0f;
	////減速から通常時に戻るまでのフレームカウンター
	//float backNormalTimer_ = 0.0f;
	////減速中の移動量
	//float slowMoveqQuantity_;
	////スロー分の量を足す
	//float backQuantity_;

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
};

