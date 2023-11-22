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

	cameraSpeed_ = kInitCameraSpeed_;
	cameraSpeedY_ = kInitCameraSpeed_;
}

void GameCamera::Update(bool isPlayerMoving, Vector3 playerPos, Vector3 playerInitPos, bool isDead,
	Vector3 playerDeadPos, Vector3 playerAxel, bool onGround)
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

		moveEyeVec = goalEyePos_ - eye_;
		moveTargetVec = goalEyeTarget_ - target_;

		moveEyeVecY = moveEyeVec;
		moveEyeVecY.Normalize();
		moveTargetVecY = moveTargetVec;
		moveTargetVecY.Normalize();
		
		eye_.x += moveEyeVec.x * cameraSpeed_;
		eye_.z += moveEyeVec.z / initEyeDistance_.z * cameraSpeed_;

		float distanceY = std::fabs(playerPos.y - playerInitPos.y);

		if(playerDeadPos.x <= 0) {}
		if(onGround == false) {}

		if(isDead == false)
		{
			eye_.y += moveEyeVecY.y / (distanceY + EyeYAxelRate_) * cameraSpeedY_;
			target_.y += moveTargetVecY.y / (distanceY + EyeYAxelRate_) * cameraSpeedY_;
		}

		if(playerAxel.x < 0) {}

		target_.x += moveTargetVec.x * cameraSpeed_;
		target_.z += moveTargetVec.z / initTargetDistance_.z * cameraSpeed_;

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

void GameCamera::GoalAnimation()
{
	if(sceneAnimeTimer_ >= 0)
	{
		sceneAnimeTimer_--;
	}
	else
	{
		isFinishAnimetion_ = true;
	}

	//変化量
	float x = sceneAnimeTimer_ / kSceneAnimeTime_;
	sceneAnimationVec_.x += easeOutQuint(x);

	eye_.x = PlayEaseIn(9.8f, 21.0f, sceneAnimeTimer_, kSceneAnimeTime_);
	eye_.y = PlayEaseIn(4.1f, 3.4f, sceneAnimeTimer_, kSceneAnimeTime_);
	eye_.z = PlayEaseIn(712.0f, -12.0f, sceneAnimeTimer_, kSceneAnimeTime_);

}
