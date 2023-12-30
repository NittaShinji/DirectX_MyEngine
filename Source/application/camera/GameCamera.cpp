#include "GameCamera.h"
#include "ImGuiManager.h"
#include <cmath>
using namespace MathUtillty;

void GameCamera::Initialize()
{
	eye_ = initCameraEye;
	target_ = initCameraTarget;
	up_ = initCameraUp;

	//衝突時に停止する座標を設定
	goalEyePos_.x = { initCameraEye.x + initEyeDistance_.x };
	goalEyePos_.y = { initCameraEye.y + initEyeDistance_.y };
	goalEyePos_.z = { initCameraEye.z + initEyeDistance_.z };

	goalEyeTarget_.x = { initCameraTarget.x + initTargetDistance_.x };
	goalEyeTarget_.y = { initCameraTarget.y + initTargetDistance_.y };
	goalEyeTarget_.z = { initCameraTarget.z + initTargetDistance_.z };

	
	speedEasing_.time = static_cast<float>(kInitTime_);
	axcellWaitTimer = kInitTime_;
	slowTimer_ = kSlowTime_;
	axcellRate_ = kAxcellNormalRate_;

	sceneAnimeTimer_ = kInitSceneAnimeTime_;
	cameraSpeed_ = kInitCameraSpeed_;
	cameraSpeedY_ = kInitCameraSpeed_;
	isNotBackAnimation_ = false;
	isAxcellrate_ = false;
	isStartGoalEasing_ = false;
}

void GameCamera::Update(bool isPlayerMoving, Vector3 playerPos, Vector3 playerInitPos, bool isDead,
	Vector3 playerDeadPos, bool rightAxcell)
{
	cameraSpeed_ = gameSpeed_->GetSpeedNum();
	cameraSpeedY_ = gameSpeed_->GetSpeedNum();

	if(isPlayerMoving == true)
	{
		goalEyePos_.x = playerPos.x - playerInitPos.x + initCameraEye.x + initEyeDistance_.x;
		goalEyePos_.y = playerDeadPos.y - playerInitPos.y + initCameraEye.y;
		goalEyePos_.z = playerPos.z - playerInitPos.z + initCameraEye.z + initEyeDistance_.z;

		goalEyeTarget_.x = playerPos.x - playerInitPos.x + initCameraTarget.x + initTargetDistance_.x;
		goalEyeTarget_.y = playerDeadPos.y - playerInitPos.y + initCameraTarget.y;
		goalEyeTarget_.z = playerPos.z - playerInitPos.z + initCameraTarget.z + initTargetDistance_.z;

		moveEyeVec = goalEyePos_ - eye_;
		moveTargetVec = goalEyeTarget_ - target_;

		moveEyeVecY = moveEyeVec;
		moveEyeVecY.Normalize();
		moveTargetVecY = moveTargetVec;
		moveTargetVecY.Normalize();

		eye_.x += moveEyeVec.x * cameraSpeed_;

		if(rightAxcell == false)
		{
			if(isNotBackAnimation_ == true)
			{
				if(moveEyeVec.z >= initEyeDistance_.z)
				{
	/*				axcellEasing_.time++;
					if(axcellEasing_.time > 0 && axcellEasing_.time <= axcellEasing_.totalTime)
					{
						axcellRate_ = PlayEaseOutQuint(axcellEasing_);
					}*/

					axcellRate_ += axcellRateMoveValue_;
				}
				else
				{
					axcellEasing_.time = 0;
					isNotBackAnimation_ = false;
					if(axcellRate_ >= kAxcellNormalRate_)
					{
						axcellRate_ -= decelerationMoveValue_;
					}
					axcellRate_ = kAxcellNormalRate_;
				}

				/*axcellEasing_.time++;
				if(axcellEasing_.time > 0 && axcellEasing_.time <= axcellEasing_.totalTime)
				{
					axcellRate_ = PlayEaseOutQuint(axcellEasing_);

				}
				else
				{
					axcellEasing_.time = 0;
					isNotBackAnimation_ = false;
				}*/
			}
		}
		else
		{
			if(isNotBackAnimation_ == false)
			{
				isAxcellrate_ = true;
			}

			//if(axcellRate_ > 0)
			//{
			//	//waitRate_++;
			//	
			//	/*axcellEasing_.endDistance = 1.0f - axcelRate_;
			//	axcellEasing_.startPos = axcelRate_
			//	axcellRate_ = PlayEaseOutQuint(axcellEasing_);*/

			//	
			//	isNotBackAnimation_ = true;
			//	axcellRate_ -= 0.01f;
			//}
		}

		if(isAxcellrate_ == true)
		{
			slowDownEasing_.time++;
			if(slowDownEasing_.time > kInitTime_ && slowDownEasing_.time <= slowDownEasing_.totalTime)
			{
				axcellRate_ = PlayEaseOutQuint(slowDownEasing_);
				//axcellRate_ = PlayEaseOutBack(slowDownEasing_);

			}
			else
			{
  				slowDownEasing_.time = static_cast<float>(kInitTime_);
				isNotBackAnimation_ = true;
				isAxcellrate_ = false;
			}
		}

		if(isNotBackAnimation_ == true || isAxcellrate_ == true)
		{
			eye_.z += cameraSpeed_ * axcellRate_;
		}
		else
		{
			eye_.z += moveEyeVec.z / initEyeDistance_.z * cameraSpeed_ * axcellRate_;
		}

		//eye_.z += moveEyeVec.z / initEyeDistance_.z * cameraSpeed_ * axcellRate_;

		float distanceY = std::fabs(playerPos.y - playerInitPos.y);
		
		if(isDead == false)
		{
			eye_.y += moveEyeVecY.y / (distanceY + EyeYAxelRate_) * cameraSpeedY_;
			target_.y += moveTargetVecY.y / (distanceY + EyeYAxelRate_) * cameraSpeedY_;
		}

		target_.x += moveTargetVec.x * cameraSpeed_;

		if(isNotBackAnimation_ == true || isAxcellrate_ == true)
		{
			target_.z += cameraSpeed_ * axcellRate_;
		}
		else
		{
			target_.z += moveTargetVec.z / initTargetDistance_.z * cameraSpeed_ * axcellRate_;
		}

		//target_.z += moveTargetVec.z / initTargetDistance_.z * cameraSpeed_ * axcellRate_;

		if(isDead == true)
		{
			//もし死んだ際にY軸のスピードがマイナスならば、そのまま動かす
			if(moveEyeVecY.y < 0)
			{
				eye_.y += moveEyeVecY.y / (distanceY + EyeYAxelRate_) * cameraSpeedY_;
				target_.y += moveTargetVecY.y / (distanceY + EyeYAxelRate_) * cameraSpeedY_;
			}
		}

	}

	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void GameCamera::Reset()
{
	eye_ = initCameraEye;
	target_ = initCameraTarget;
	up_ = initCameraUp;
	axcellRate_ = kAxcellNormalRate_;

	isStopTarget_ = false;
	isStartGoalEasing_ = false;

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


	ImGui::SliderFloat("moveEyeY", &moveEyeVecY.y, kImGuiMoveRate.x, kImGuiMoveRate.y);
	ImGui::SliderFloat("moveTargetVecY", &moveTargetVecY.y, kImGuiMoveTargetRate.x, kImGuiMoveTargetRate.y);
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
	//スタート時にカメラ位置を代入
	if(isStartGoalEasing_ == false)
	{
		goalEyeXEasing_.startPos = eye_.x;
		goalEyeYEasing_.startPos = eye_.y;
		goalEyeZEasing_.startPos = eye_.z;
		isStartGoalEasing_ = true;
	}
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
