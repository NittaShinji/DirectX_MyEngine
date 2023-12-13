#include "GameCamera.h"
#include "ImGuiManager.h"
#include <cmath>
using namespace MathUtillty;

void GameCamera::Initialize()
{
	eye_ = initcameraEye;
	target_ = initcameraTarget;
	up_ = initcameraUp;

	//衝突時に停止する座標を設定
	goalEyePos_.x = { initcameraEye.x + initEyeDistance_.x };
	goalEyePos_.y = { initcameraEye.y + initEyeDistance_.y };
	goalEyePos_.z = { initcameraEye.z + initEyeDistance_.z };

	goalEyeTarget_.x = { initcameraTarget.x + initTargetDistance_.x };
	goalEyeTarget_.y = { initcameraTarget.y + initTargetDistance_.y };
	goalEyeTarget_.z = { initcameraTarget.z + initTargetDistance_.z };

	speedEasing_.time = 0.0f;
	slowTimer_ = kSlowTime_;
	axcellWaitTimer = 0;
	waitRate_ = 0.0f;
	axcellRate_ = 1.0f;

	sceneAnimeTimer_ = 0.0f;
	cameraSpeed_ = kInitCameraSpeed_;
	cameraSpeedY_ = kInitCameraSpeed_;
	isNotBackAnimation_ = false;
	isAxcellrate_ = false;

}

void GameCamera::Update(bool isPlayerMoving, Vector3 playerPos, Vector3 playerInitPos, bool isDead,
	Vector3 playerDeadPos, Vector3 playerTotalAxel, bool onGround, bool rightAxcell, Vector3 rightAxcellVec)
{
	cameraSpeed_ = gameSpeed_->GetSpeedNum();
	cameraSpeedY_ = gameSpeed_->GetSpeedNum();

	if(isPlayerMoving == true)
	{
		goalEyePos_.x = playerPos.x - playerInitPos.x + initcameraEye.x + initEyeDistance_.x;
		goalEyePos_.y = playerDeadPos.y - playerInitPos.y + initcameraEye.y;
		goalEyePos_.z = playerPos.z - playerInitPos.z + initcameraEye.z + initEyeDistance_.z;

		goalEyeTarget_.x = playerPos.x - playerInitPos.x + initcameraTarget.x + initTargetDistance_.x;
		goalEyeTarget_.y = playerDeadPos.y - playerInitPos.y + initcameraTarget.y;
		goalEyeTarget_.z = playerPos.z - playerInitPos.z + initcameraTarget.z + initTargetDistance_.z;

		/*if(rightAxcell == true)
		{
			axcellWaitTimer++;
			if(axcellWaitTimer >= kAxcellWaitTime)
			{
				isAxcellAnimation_ = true;
			}
		}*/

		moveEyeVec = goalEyePos_ - eye_;
		moveTargetVec = goalEyeTarget_ - target_;

		moveEyeVecY = moveEyeVec;
		moveEyeVecY.Normalize();
		moveTargetVecY = moveTargetVec;
		moveTargetVecY.Normalize();

		eye_.x += moveEyeVec.x * cameraSpeed_;

		if(rightAxcellVec.z == 0) {};

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

					axcellRate_ += 0.0015f;
				}
				else
				{
					axcellEasing_.time = 0;
					isNotBackAnimation_ = false;
					if(axcellRate_ >= 1.0f)
					{
						axcellRate_ -= 0.01f;
					}
					axcellRate_ = 1.0f;
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
			if(slowDownEasing_.time > 0 && slowDownEasing_.time <= slowDownEasing_.totalTime)
			{
				axcellRate_ = PlayEaseOutQuint(slowDownEasing_);
				//axcellRate_ = PlayEaseOutBack(slowDownEasing_);

			}
			else
			{
  				slowDownEasing_.time = 0;
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

		if(playerDeadPos.x <= 0) {}
		if(onGround == false) {}

		if(isDead == false)
		{
			eye_.y += moveEyeVecY.y / (distanceY + EyeYAxelRate_) * cameraSpeedY_;
			target_.y += moveTargetVecY.y / (distanceY + EyeYAxelRate_) * cameraSpeedY_;
		}

		if(playerTotalAxel.x < 0) {}

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
	eye_ = { 30,7.5,-20 };
	target_ = { 0,5,5 };
	axcellRate_ = 1.0f;

	isStopTarget_ = false;

	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void GameCamera::ImGuiUpdate()
{
	//スプライトの編集ウインドウの表示

	ImGui::Begin("Camera");
	ImGui::SetWindowPos(ImVec2(300, 0));
	ImGui::SetWindowSize(ImVec2(300, 250));

	ImGui::SliderFloat("moveEyeY", &moveEyeVecY.y, -100.0f, 50.0f);
	ImGui::SliderFloat("moveTargetVecY", &moveTargetVecY.y, -100.0f, 50.0f);
	ImGui::SliderFloat("EyeYAxelRate_", &EyeYAxelRate_, 0.0f, 10.0f);
	ImGui::SliderFloat("AxcelRate", &axcellRate_, 0.0f, 10.0f);
	ImGui::SliderFloat("EasingX", &goalEyeXEasing_.endDistance, 0.0f, 10.0f);
	ImGui::SliderFloat("EasingY", &goalEyeYEasing_.endDistance, 0.0f, 10.0f);
	ImGui::SliderFloat("EasingZ", &goalEyeZEasing_.endDistance, 0.0f, 10.0f);



	ImGui::SliderFloat("easingTimer", &speedEasing_.time, 0.0f, 100.0f);
	ImGui::SliderFloat("cameraSpeedY_", &cameraSpeedY_, 0.0f, 100.0f);

	ImGui::SliderFloat("eyeX", &eye_.x, -100.0f, 50.0f);
	ImGui::SliderFloat("eyeY", &eye_.y, -100.0f, 50.0f);
	ImGui::SliderFloat("eyeZ", &eye_.z, -100.0f, 1000.0f);

	ImGui::SliderFloat("targetX", &target_.x, -100.0f, 50.0f);
	ImGui::SliderFloat("targetY", &target_.y, -100.0f, 50.0f);
	ImGui::SliderFloat("targetZ", &target_.z, -100.0f, 1000.0f);

	ImGui::End();

}

void GameCamera::AccelerationAnimation()
{


}

void GameCamera::GoalAnimation()
{
	if(sceneAnimeTimer_ < kSceneAnimeTime_)
	{
		//スタート時に
		if(sceneAnimeTimer_ == 0.0f)
		{
			goalEyeXEasing_.startPos = eye_.x;
			goalEyeYEasing_.startPos = eye_.y;
			goalEyeZEasing_.startPos = eye_.z;
		}

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
		isFinishAnimetion_ = true;
		sceneAnimeTimer_ = kSceneAnimeTime_;
		goalEyeXEasing_.time = sceneAnimeTimer_;
		goalEyeYEasing_.time = sceneAnimeTimer_;
		goalEyeZEasing_.time = sceneAnimeTimer_;
	}
}
