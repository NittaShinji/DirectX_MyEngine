#include "GameCamera.h"
#include "ImGuiManager.h"
#include <cmath>
using namespace MathUtillty;

void GameCamera::Initialize()
{
	//メンバ変数の初期化
	eye_ = initCameraEye;
	target_ = initCameraTarget;
	up_ = initCameraUp;

	speedEasing_.time = static_cast<float>(kInitTime_);
	axcellWaitTimer = kInitTime_;
	slowTimer_ = kSlowTime_;
	axcellRate_ = kAxcellNormalRate_;

	sceneAnimeTimer_ = kInitSceneAnimeTime_;
	cameraSpeed_ = kInitCameraSpeed_;
	cameraSpeedY_ = kInitCameraSpeed_;
	isNotBackAnimation_ = false;
	isSlowDown_ = false;
	isStartGoalEasing_ = false;

	//プレイヤーが衝突時に停止する座標を設定
	goalEyePos_.x = { initCameraEye.x + initEyeDistance_.x };
	goalEyePos_.y = { initCameraEye.y + initEyeDistance_.y };
	goalEyePos_.z = { initCameraEye.z + initEyeDistance_.z };

	goalEyeTarget_.x = { initCameraTarget.x + initTargetDistance_.x };
	goalEyeTarget_.y = { initCameraTarget.y + initTargetDistance_.y };
	goalEyeTarget_.z = { initCameraTarget.z + initTargetDistance_.z };

}

void GameCamera::Update(bool isPlayerMoving, Vector3 playerPos, Vector3 playerInitPos, bool isDead,
	Vector3 playerDeadPos, bool rightAxcell)
{
	cameraSpeed_ = gameSpeed_->GetSpeedNum();
	cameraSpeedY_ = gameSpeed_->GetSpeedNum();

	//プレイヤーの球が動いているとき
	if(isPlayerMoving == true)
	{
		//補間移動の計算
		goalEyePos_.x = playerPos.x - playerInitPos.x + initCameraEye.x + initEyeDistance_.x;
		goalEyePos_.y = playerDeadPos.y - playerInitPos.y + initCameraEye.y;
		goalEyePos_.z = playerPos.z - playerInitPos.z + initCameraEye.z + initEyeDistance_.z;

		goalEyeTarget_.x = playerPos.x - playerInitPos.x + initCameraTarget.x + initTargetDistance_.x;
		goalEyeTarget_.y = playerDeadPos.y - playerInitPos.y + initCameraTarget.y;
		goalEyeTarget_.z = playerPos.z - playerInitPos.z + initCameraTarget.z + initTargetDistance_.z;

		moveEyeVec_ = goalEyePos_ - eye_;
		moveTargetVec_ = goalEyeTarget_ - target_;

		moveEyeVecY_ = moveEyeVec_;
		moveEyeVecY_.Normalize();
		moveTargetVecY_ = moveTargetVec_;
		moveTargetVecY_.Normalize();

		//プレイヤーはx方向には動かないので補間計算はしない。
		eye_.x += moveEyeVec_.x * cameraSpeed_;
		target_.x += moveTargetVec_.x * cameraSpeed_;

		//プレイヤーが加速していないとき
		if(rightAxcell == false)
		{
			//カメラが遅れているアニメーション中の場合
			if(isNotBackAnimation_ == true)
			{
				//カメラとプレイヤーの距離が所定の距離より離れている時
				if(moveEyeVec_.z >= initEyeDistance_.z)
				{
					axcellRate_ += axcellRateMoveValue_;
				}
				//カメラとプレイヤーの距離が所定の距離以下になった場合
				else
				{
					isNotBackAnimation_ = false;
					//カメラの加速割合を戻す
					axcellRate_ = kAxcellNormalRate_;
				}
			}
		}
		//プレイヤーが加速しているとき
		else
		{
			if(isNotBackAnimation_ == false)
			{
				isSlowDown_ = true;
			}
		}

		//スローダウンする処理
		if(isSlowDown_ == true)
		{
			slowDownEasing_.time++;
			if(slowDownEasing_.time > kInitTime_ && slowDownEasing_.time <= slowDownEasing_.totalTime)
			{
				axcellRate_ = PlayEaseOutQuint(slowDownEasing_);
			}
			else
			{
  				slowDownEasing_.time = static_cast<float>(kInitTime_);
				isNotBackAnimation_ = true;
				isSlowDown_ = false;
			}
		}

		//Y方向の処理
		float distanceY = std::fabs(playerPos.y - playerInitPos.y);
		//もしプレイヤーが死んだ際に
		if(isDead == true)
		{
			//Y軸のスピードがマイナスならば、そのまま動かす
			if(moveEyeVecY_.y < 0)
			{
				eye_.y += moveEyeVecY_.y / (distanceY + EyeYAxelRate_) * cameraSpeedY_;
				target_.y += moveTargetVecY_.y / (distanceY + EyeYAxelRate_) * cameraSpeedY_;
			}
		}
		else
		{
			eye_.y += moveEyeVecY_.y / (distanceY + EyeYAxelRate_) * cameraSpeedY_;
			target_.y += moveTargetVecY_.y / (distanceY + EyeYAxelRate_) * cameraSpeedY_;
		}

		//加速時のアニメーション中は通常のスピードを足す
		if(isNotBackAnimation_ == true || isSlowDown_ == true)
		{
			eye_.z += cameraSpeed_ * axcellRate_;
			target_.z += cameraSpeed_ * axcellRate_;
		}
		//加速時以外は補間移動をするように
		else
		{
			eye_.z += moveEyeVec_.z / initEyeDistance_.z * cameraSpeed_ * axcellRate_;
			target_.z += moveTargetVec_.z / initTargetDistance_.z * cameraSpeed_ * axcellRate_;
		}
	}

	//行列の更新
	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void GameCamera::Reset()
{
	//メンバ変数を初期化
	eye_ = initCameraEye;
	target_ = initCameraTarget;
	up_ = initCameraUp;
	axcellRate_ = kAxcellNormalRate_;
	isStopTarget_ = false;
	isStartGoalEasing_ = false;

	//行列の更新
	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void GameCamera::ImGuiUpdate()
{
	//スプライトの編集ウインドウの表示

	ImGui::Begin("Camera");
	const Vector2 kImGuiPos = { 300.0f, 0.0f };
	const Vector2 kImGuiSize = { 300.0f, 250.0f };
	ImGui::SetWindowPos(ImVec2(kImGuiPos.x, kImGuiPos.y));
	ImGui::SetWindowSize(ImVec2(kImGuiSize.x, kImGuiPos.y));

	const Vector2 kImGuiMoveRate = { -100.0f,50.0f };
	const Vector2 kImGuiMoveTargetRate = { -100.0f,50.0f };
	const Vector2 kImGuiEyeYAxllRate = { 0.0f,10.0f };
	const Vector2 kImGuiAxllRate = { 0.0f,10.0f };

	const Vector2 kImGuiEasingXRate = { 0.0f,10.0f };
	const Vector2 kImGuiEasingYRate = { 0.0f,10.0f };
	const Vector2 kImGuiEasingZRate = { 0.0f,10.0f };
	const Vector2 kImGuiEasingTimerRate = { 0.0f,100.0f };
	const Vector2 kImGuiCameraSpeedYRate = { 0.0f,100.0f };
	const Vector2 kImGuiEyeRate = { -100.0f,50.0f };
	const Vector2 kImGuiTargetRate = { -100.0f,1000.0f };

	ImGui::SliderFloat("moveEyeY", &moveEyeVecY_.y, kImGuiMoveRate.x, kImGuiMoveRate.y);
	ImGui::SliderFloat("moveTargetVecY", &moveTargetVecY_.y, kImGuiMoveTargetRate.x, kImGuiMoveTargetRate.y);
	ImGui::SliderFloat("EyeYAxelRate_", &EyeYAxelRate_, kImGuiEyeYAxllRate.x, kImGuiEyeYAxllRate.y);
	ImGui::SliderFloat("AxcelRate", &axcellRate_, kImGuiAxllRate.x, kImGuiAxllRate.y);
	ImGui::SliderFloat("EasingX", &goalEyeXEasing_.endDistance, kImGuiEasingXRate.x, kImGuiEasingXRate.y);
	ImGui::SliderFloat("EasingY", &goalEyeYEasing_.endDistance, kImGuiEasingYRate.x, kImGuiEasingYRate.y);
	ImGui::SliderFloat("EasingZ", &goalEyeZEasing_.endDistance, kImGuiEasingZRate.x, kImGuiEasingZRate.y);

	ImGui::SliderFloat("easingTimer", &speedEasing_.time, kImGuiEasingTimerRate.x, kImGuiEasingTimerRate.y);
	ImGui::SliderFloat("cameraSpeedY_", &cameraSpeedY_, kImGuiCameraSpeedYRate.x, kImGuiCameraSpeedYRate.y);

	ImGui::SliderFloat("eyeX", &eye_.x, kImGuiEyeRate.x, kImGuiEyeRate.y);
	ImGui::SliderFloat("eyeY", &eye_.y, kImGuiEyeRate.x, kImGuiEyeRate.y);
	ImGui::SliderFloat("eyeZ", &eye_.z, kImGuiEyeRate.x, kImGuiEyeRate.y);

	ImGui::SliderFloat("targetX", &target_.x, kImGuiTargetRate.x, kImGuiTargetRate.y);
	ImGui::SliderFloat("targetY", &target_.y, kImGuiTargetRate.x, kImGuiTargetRate.y);
	ImGui::SliderFloat("targetZ", &target_.z, kImGuiTargetRate.x, kImGuiTargetRate.y);

	ImGui::End();

}

void GameCamera::GoalAnimation()
{
	//ゴール演出スタート時にカメラ位置を代入
	if(isStartGoalEasing_ == false)
	{
		goalEyeXEasing_.startPos = eye_.x;
		goalEyeYEasing_.startPos = eye_.y;
		goalEyeZEasing_.startPos = eye_.z;
		isStartGoalEasing_ = true;
	}

	//アニメーション中
	if(sceneAnimeTimer_ < kSceneAnimeTime_)
	{
		sceneAnimeTimer_++;
		goalEyeXEasing_.time = sceneAnimeTimer_;
		goalEyeYEasing_.time = sceneAnimeTimer_;
		goalEyeZEasing_.time = sceneAnimeTimer_;

		eye_.x = PlayEaseOutCubic(goalEyeXEasing_);
		eye_.y = PlayEaseOutCubic(goalEyeYEasing_);
		eye_.z = PlayEaseOutCubic(goalEyeZEasing_);
	}
	else
	{
		isStartGoalEasing_ = false;
		isFinishAnimetion_ = true;
		sceneAnimeTimer_ = kInitSceneAnimeTime_;
	}
}
