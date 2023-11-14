#include "GameCamera.h"
#include "ImGuiManager.h"
#include "Easing.h"

using namespace MathUtillty;

void GameCamera::Initialize()
{
	eye_ = initcameraEye;
	target_ = initcameraTarget;
	up_ = initcameraUp;

	//衝突時に停止する座標を設定
	goalEyePos_.x = { initcameraEye.x + initEyeDistance_.x };
	goalEyePos_.y = { initcameraEye.y };
	goalEyePos_.z = { initcameraEye.y + initEyeDistance_.z };

	goalEyeTarget_.x = { initcameraTarget.x + initTargetDistance_.x };
	goalEyeTarget_.y = { initcameraTarget.y };
	goalEyeTarget_.z = { initcameraTarget.z + initTargetDistance_.z };

	cameraSpeed_ = 1.0f;
}

void GameCamera::Update(bool isPlayerMoving, Vector3 playerPos, Vector3 playerInitPos)
{
	if(isPlayerMoving == true)
	{
		goalEyePos_.x = playerPos.x - playerInitPos.x + initcameraEye.x + initEyeDistance_.x;
		goalEyePos_.y = initcameraEye.y;
		goalEyePos_.z = playerPos.z - playerInitPos.z + initcameraEye.z + initEyeDistance_.z;

		goalEyeTarget_.x = playerPos.x - playerInitPos.x + initcameraTarget.x + initTargetDistance_.x;
		goalEyeTarget_.y = initcameraTarget.y;
		goalEyeTarget_.z = playerPos.z - playerInitPos.z + initcameraTarget.z + initTargetDistance_.z;

		Vector3 moveVec = goalEyePos_ - eye_;
		//moveVec.z = std::abs(moveVec.z);
		Vector3 moveEyeVec = goalEyePos_ - eye_;
		//moveEyeVec.z = std::abs(moveEyeVec.z);
		Vector3 moveTargetVec = goalEyeTarget_ - target_;

		eye_.x +=  moveEyeVec.x  * cameraSpeed_;
		eye_.z +=  moveEyeVec.z / initEyeDistance_.z * cameraSpeed_;

		target_.x += moveTargetVec.x * cameraSpeed_;
		target_.z += moveTargetVec.z / initTargetDistance_.z * cameraSpeed_;
	}

	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void GameCamera::Reset()
{
	eye_ = { 30,7.5,-20 };
	target_ = { 0,5,5 };

	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void GameCamera::ImGuiUpdate()
{
	//スプライトの編集ウインドウの表示

	ImGui::Begin("Camera");
	ImGui::SetWindowPos(ImVec2(300, 0));
	ImGui::SetWindowSize(ImVec2(300, 150));

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
