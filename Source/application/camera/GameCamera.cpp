#include "GameCamera.h"
#include "ImGuiManager.h"
#include "Easing.h"

using namespace MathUtillty;

void GameCamera::Initialize()
{
	const Vector3 initcameraEye = { 30,7.5,-20 };
	const Vector3 initcameraTarget = { 0,5,5 };
	const Vector3 initcameraUp = { 0,1,0 };

	eye_ = initcameraEye;
	target_ = initcameraTarget;
	up_ = initcameraUp;

	axcelRate_ = 0;
}

void GameCamera::Update(bool isPlayerMoving, bool isPlayerDead, bool isPlayerStoped, Vector3 playerAxcell_, Vector3 playerPos)
{
	if(isPlayerMoving == true )
	{
		if(isPlayerStoped == false)
		{
			if(isSlowDown_ == false)
			{
				totalAxcellSpeed_ = playerAxcell_;
			}
			
			if(isPlayerDead == true)
			{
				totalAxcellSpeed_.z += playerAxcell_.z / 1.5f;
				totalAxcellSpeed_.z += playerAxcell_.z / 1.5f;
			}
			else
			{
				//プレイヤーのポジションを代入(動いた分だけ進むように初期位置を引く)
				if(playerPos.y >= 6.0f)
				{
					//target_.y += playerAxcell_.y;
					//eye_.y += playerAxcell_.y;
				}

				totalAxcellSpeed_.y = playerAxcell_.y / 10.0f;

				/*target_.z += playerAxcell_.z;
				eye_.z += playerAxcell_.z;*/
			}

			if(moveSlowTimer_ <= 0)
			{
				isSlowDown_ = false;
				moveSlowTimer_ = kSlowDownTime_;
			}
			
		}
		else
		{
			//急に止まっているからおかしい
			//徐々に減速させる
			if(isPlayerStoped == true && isSlowDown_ == false)
			{
				isSlowDown_ = true;
			}
			
			//totalAxcellSpeed_ = Vector3Zero();

			//if(isSlowDown_ == true)
			//{
			//	float x = 0;
			//	x = moveSlowTimer_ / kSlowDownTime_;
			//	axcelRate_ = easeOutQuint(x);

			//	/*axcelRate_ = PlayEaseOutQuint(moveSlowTimer_, 0.5f, -0.5f, kSlowDownTime_);
			//	totalAxcellSpeed_.z *= axcelRate_;*/

			//	//totalAxcellSpeed_.z = PlayEaseOutQuint(moveSlowTimer_, playerAxcell_.z, -playerAxcell_.z, kSlowDownTime_);

			//	if(moveSlowTimer_ > 0)
			//	{
			//		moveSlowTimer_--;
			//	}

			//	if(totalAxcellSpeed_.z <= 0)
			//	{
			//		totalAxcellSpeed_.z = 0;
			//	}
			//}

			if(isSlowDown_ == true)
			{
				if(moveSlowTimer_ > 0)
				{
					moveSlowTimer_--;
				}

				float x = 0;
				x = moveSlowTimer_ / kSlowDownTime_;
				axcelRate_ = easeOutQuint(x);

				/*axcelRate_ = PlayEaseOutQuint(moveSlowTimer_, 0.5f, -0.5f, kSlowDownTime_);
				totalAxcellSpeed_.z *= axcelRate_;*/

				//totalAxcellSpeed_.z = PlayEaseOutQuint(moveSlowTimer_, playerAxcell_.z, -playerAxcell_.z, kSlowDownTime_);

				totalAxcellSpeed_.z *= axcelRate_;



				if(totalAxcellSpeed_.z <= 0)
				{
					totalAxcellSpeed_.z = 0;
				}
			}
		}

		

		target_ += totalAxcellSpeed_;
		eye_ += totalAxcellSpeed_;

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
