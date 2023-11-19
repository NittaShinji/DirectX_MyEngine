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

	cameraSpeed_ = 1.0f;
	cameraSpeedY_ = 1.0f;
}

void GameCamera::Update(bool isPlayerMoving, Vector3 playerPos, Vector3 playerInitPos,bool isDead,
	Vector3 playerDeadPos,Vector3 playerAxel, bool onGround)
{
	if(isPlayerMoving == true)
	{
		goalEyePos_.x = playerPos.x - playerInitPos.x + initcameraEye.x + initEyeDistance_.x;
		//goalEyePos_.y = initcameraEye.y;
		//goalEyePos_.y = playerDeadPos.y - playerInitPos.y + initcameraEye.y;
		goalEyePos_.y = playerPos.y - playerInitPos.y + initcameraEye.y;

		goalEyePos_.z = playerPos.z - playerInitPos.z + initcameraEye.z + initEyeDistance_.z;

		goalEyeTarget_.x = playerPos.x - playerInitPos.x + initcameraTarget.x + initTargetDistance_.x;
		//goalEyeTarget_.y = initcameraTarget.y;
		goalEyeTarget_.y = playerPos.y - playerInitPos.y + initcameraTarget.y;
		//goalEyeTarget_.y = playerDeadPos.y - playerInitPos.y + initcameraTarget.y;

		goalEyeTarget_.z = playerPos.z - playerInitPos.z + initcameraTarget.z + initTargetDistance_.z;

		//Vector3 moveVec = goalEyePos_ - eye_;
		//moveVec.z = std::abs(moveVec.z);
		Vector3 moveEyeVec = goalEyePos_ - eye_;
		//Vector3 moveEyeVecY = playerDeadPos - eye_;
		//moveEyeVec.z = std::abs(moveEyeVec.z);
		Vector3 moveTargetVec = goalEyeTarget_ - target_;
		//Vector3 moveTargetVecY = goalEyeTarget_ - target_;


		eye_.x += moveEyeVec.x * cameraSpeed_;
		eye_.z += moveEyeVec.z / initEyeDistance_.z * cameraSpeed_;

		Vector3 moveVecY = moveEyeVec.Normalize();

		float distanceY = std::fabs(playerPos.y - playerInitPos.y);

		if(playerDeadPos.x <= 0){}
		if(onGround == false) {}
		
		if(isDead == false)
		{
			//if(playerAxel.y < 0)
			//{
			//	cameraSpeed_ = 1.5f;
			//	//eye_.y += moveVecY.y / initEyeDistance_.y * cameraSpeed_;
			//	eye_.y += playerDeadPos.y - playerInitPos.y * cameraSpeed_;

			//}
			//else
			//{
			//	cameraSpeed_ = 1.0f;
			//	eye_.y += playerDeadPos.y - playerInitPos.y * cameraSpeed_;
			//	//eye_.y += moveVecY.y / initEyeDistance_.y * cameraSpeed_;

			//}

			if(moveVecY.y < 0)
			{
				speedEasing_.time++;
				if(speedEasing_.time > 0 && speedEasing_.time <= speedEasing_.totalTime)
				{
 					cameraSpeedY_ = PlayEaseOutQuint(speedEasing_);
				}
				else
				{
					speedEasing_.time = 0.0f;
					cameraSpeedY_ = 1.0f;
				}
			}
			else
			{
				speedEasing_.time = 0.0f;
			}

			eye_.y += moveVecY.y / (distanceY + initEyeDistance_.y) * cameraSpeedY_;
			
			/*if(playerDeadPos.y - playerInitPos.y != 0)
			{
				eye_.y += moveVecY.y / (distanceY + initEyeDistance_.y) * cameraSpeed_;
			}*/
			/*else
			{
				eye_.y += moveVecY.y / initTargetDistance_.y * cameraSpeed_;
			}*/
			/*eye_.y += moveVecY.y / initTargetDistance_.y * cameraSpeed_;*/
			//eye_.y += moveEyeVecY.y * cameraSpeed_;
		}
		if(playerAxel.x < 0) {}

		target_.x += moveTargetVec.x * cameraSpeed_;
		target_.z += moveTargetVec.z / initTargetDistance_.z * cameraSpeed_;

		Vector3 moveTargetVecY = moveTargetVec.Normalize();

		if(isDead == false)
		{
			/*if(playerAxel.y < 0)
			{
				cameraSpeed_ = 1.0f;
				
				target_.y += moveTargetVecY.y / initTargetDistance_.y * cameraSpeed_;
			}
			else
			{
				cameraSpeed_ = 1.5f;
				target_.y += moveTargetVecY.y / initTargetDistance_.y * cameraSpeed_;
			}*/

			target_.y += moveTargetVecY.y / (distanceY + initEyeDistance_.y) * cameraSpeedY_;

			/*if(playerDeadPos.y - playerInitPos.y != 0)
			{
				target_.y += moveTargetVecY.y / (distanceY + initEyeDistance_.y) * cameraSpeed_;
			}*/
			/*else
			{
				target_.y += moveTargetVecY.y / initTargetDistance_.y * cameraSpeed_;
			}*/
			//target_.y += moveTargetVecY.y / initTargetDistance_.y * cameraSpeed_ ;
			//target_.y += playerDeadPos.y - playerInitPos.y * cameraSpeed_;

		}
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
