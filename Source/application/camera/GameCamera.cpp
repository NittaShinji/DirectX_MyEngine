#include "GameCamera.h"
#include "ImGuiManager.h"
#include "Easing.h"


void GameCamera::Update(bool isPlayerMoving, bool isPlayerDead, Vector3 playerAxcell_, Vector3 playerPos)
{
	if(isPlayerMoving == true)
	{

		if(isPlayerDead == true)
		{
			target_.z += playerAxcell_.z / 1.5f;
			eye_.z += playerAxcell_.z / 1.5f;
		}
		else
		{
			//プレイヤーのポジションを代入(動いた分だけ進むように初期位置を引く)
			if(playerPos.y >= 7.0f)
			{
				target_.y += playerAxcell_.y;
				eye_.y += playerAxcell_.y;
			}
			
			target_.z += playerAxcell_.z;
			eye_.z += playerAxcell_.z;

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
	ImGui::SetWindowSize(ImVec2(300, 300));

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

	eye_.x = PlayEaseIn(sceneAnimeTimer_, 9.8f, 21.0f, kSceneAnimeTime_);
	eye_.y = PlayEaseIn(sceneAnimeTimer_, 4.1f, 3.4f, kSceneAnimeTime_);
	eye_.z = PlayEaseIn(sceneAnimeTimer_, 712.0f, -12.0f, kSceneAnimeTime_);

}
