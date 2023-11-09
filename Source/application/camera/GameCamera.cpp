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
	goalEyePos_.x = { initcameraEye.x + initEyeDistance.x };
	goalEyePos_.y = { initcameraEye.y };
	goalEyePos_.z = { initcameraEye.y + initEyeDistance.z };

	goalEyeTarget_.x = { initcameraTarget.x + initTargetDistance.x };
	goalEyeTarget_.y = { initcameraTarget.y };
	goalEyeTarget_.z = { initcameraTarget.z + initTargetDistance.z };

	cameraSpeed_ = 0.0f;

	//moveSlowTimer_ = kSlowDownTime_;
	//isSlowDown_ = false;
	//axcelRate_ = 0;
	//slowMoveqQuantity_ = 0.0f;
	//stopTimer_ = 0;
	//backNormalTimer_ = 0.0f;
}

void GameCamera::Update(bool isPlayerMoving, bool isPlayerDead, bool isPlayerStoped, Vector3 playerAxcell_, Vector3 playerPos, Vector3 playerInitPos, float playerNormalAxcell)
{
	/*eyePosition_.x = { playerPos.x - playerInitPos.x + initcameraEye.x };
	eyePosition_.y = { playerPos.y - playerInitPos.y + initcameraEye.y };
	eyePosition_.z = { playerPos.z - playerInitPos.z + initcameraEye.z };

	targetPosition_.x = { playerPos.x - playerInitPos.x + initcameraTarget.x };
	targetPosition_.y = { playerPos.y - playerInitPos.y + initcameraTarget.y };
	targetPosition_.z = { playerPos.z - playerInitPos.z + initcameraTarget.z};*/

	/*goalEyePos_.x = { eyePosition_.x + initEyeDistance.x };
	goalEyePos_.y = { eyePosition_.x };
	goalEyePos_.z = { eyePosition_.x + initEyeDistance.z };

	goalEyeTarget_.x = { targetPosition_.x + initTargetDistance.x };
	goalEyeTarget_.y = { targetPosition_.y };
	goalEyeTarget_.z = { targetPosition_.z + initTargetDistance.z};*/

	if(isPlayerMoving == true)
	{
		Vector3 moveVec = goalEyePos_ - eye_;
		moveVec.Normalize();

		goalEyePos_.x = playerPos.x - playerInitPos.x + initcameraEye.x + initEyeDistance.x;
		goalEyePos_.y = initcameraEye.y;
		goalEyePos_.z = playerPos.z - playerInitPos.z + initcameraEye.z + initEyeDistance.z;

		goalEyeTarget_.x = playerPos.x - playerInitPos.x + initcameraTarget.x + initTargetDistance.x;
		goalEyeTarget_.y = initcameraTarget.y;
		goalEyeTarget_.z = playerPos.z - playerInitPos.z + initcameraTarget.z + initTargetDistance.z;

		//eye_.x += moveVec.x * playerAxcell_.x * cameraSpeed_;
		//eye_.y += moveVec.y * playerAxcell_.y * cameraSpeed_;
		//eye_.z += moveVec.z * playerAxcell_.z * cameraSpeed_;

		//target_.x += moveVec.x * playerAxcell_.x * cameraSpeed_;
		//target_.y += moveVec.y * playerAxcell_.y * cameraSpeed_;
		//target_.z += moveVec.z * playerAxcell_.z * cameraSpeed_;

		if(isPlayerStoped == true)
		{
			isSlowDown_ = true;
		}

		if(playerNormalAxcell >= 0.0f) {}

		/*cameraTargetPosition_ = { playerPos - playerInitPos  + initcameraTarget };*/
		if(isSlowDown_ == true)
		{
			moveSlowTimer_--;

			//通常加速度にカメラスピードをかける
			//float endDistance = goalEyePos_.z - eye_.z;
			//eye_.z = PlayEaseOutQuint(moveSlowTimer_, eye_.z, endDistance, kSlowDownTime_);
			//float endTargetDistance = goalEyeTarget_.z - target_.z;
			//target_.z = PlayEaseOutQuint(moveSlowTimer_, target_.z, endTargetDistance, kSlowDownTime_);

			//if(cameraSpeed_ > 0)
			//{
			//	//cameraSpeed_ = 1 - (PlayEaseOutQuint(moveSlowTimer_, 1.0f, -1.0f, kSlowDownTime_));
			//}
			//else
			//{
			//	cameraSpeed_ = 0;
			//}
			
			//cameraSpeed_ = 1.0f - 0.1f * moveSlowTimer_;

			cameraSpeed_ = 1.0f;

			if(eye_.z < goalEyePos_.z )
			{
				cameraSpeed_ = 1.25f;
				eye_.x += moveVec.x * playerNormalAxcell * cameraSpeed_;
				eye_.y += moveVec.y * playerNormalAxcell * cameraSpeed_;
				eye_.z += moveVec.z * playerNormalAxcell * cameraSpeed_;

				/*float endDistance = goalEyePos_.z - eye_.z;
				eye_.z = PlayEaseOutQuint(moveSlowTimer_, eye_.z, endDistance, kSlowDownTime_);*/
			}

			if(target_.z < goalEyeTarget_.z)
			{
				cameraSpeed_ = 1.25f;
				target_.x += moveVec.x * playerNormalAxcell * cameraSpeed_;
				target_.y += moveVec.y * playerNormalAxcell * cameraSpeed_;
				target_.z += moveVec.z * playerNormalAxcell * cameraSpeed_;

				/*float endTargetDistance = goalEyeTarget_.z - target_.z;
				target_.z = PlayEaseOutQuint(moveSlowTimer_, target_.z, endTargetDistance, kSlowDownTime_);*/
			}

			if(eye_.z >= goalEyePos_.z && target_.z >= goalEyeTarget_.z)
			{
				isSlowDown_ = false;
				moveSlowTimer_ = kSlowDownTime_;
				cameraSpeed_ = 1.0f;
			}
			
			if(moveSlowTimer_ <= 0)
			{
				//isSlowDown_ = false;
				//moveSlowTimer_ = kSlowDownTime_;
			}
		}
		else
		{
			float nowEyeDistance = goalEyePos_.z - eye_.z;
			float nowTargetDistance = goalEyeTarget_.z - target_.z;

			if(initEyeDistance.z >= nowEyeDistance && initTargetDistance.z >= nowTargetDistance)
			//if(eye_.z >= goalEyePos_.z && target_.z >= goalEyeTarget_.z)
			{
				cameraSpeed_ = 0.8f;
				eye_.x += moveVec.x * playerAxcell_.x * cameraSpeed_;
				eye_.y += moveVec.y * playerAxcell_.y * cameraSpeed_;
				eye_.z += moveVec.z * playerAxcell_.z * cameraSpeed_;

				target_.x += moveVec.x * playerAxcell_.x * cameraSpeed_;
				target_.y += moveVec.y * playerAxcell_.y * cameraSpeed_;
				target_.z += moveVec.z * playerAxcell_.z * cameraSpeed_;

			}
			else
			{
				cameraSpeed_ = { 1.0f };
				eye_.x += moveVec.x * playerAxcell_.x * cameraSpeed_;
				eye_.y += moveVec.y * playerAxcell_.y * cameraSpeed_;
				eye_.z += moveVec.z * playerAxcell_.z * cameraSpeed_;

				target_.x += moveVec.x * playerAxcell_.x * cameraSpeed_;
				target_.y += moveVec.y * playerAxcell_.y * cameraSpeed_;
				target_.z += moveVec.z * playerAxcell_.z * cameraSpeed_;

			}
		}
		//{
			//totalAxcellSpeed_ = playerAxcell_;

			/*if(isSlowDown_ == false)
			{
				totalAxcellSpeed_ = playerAxcell_;
			}*/

		if(isPlayerDead == true) {}
		//	{
		//		totalAxcellSpeed_.z += playerAxcell_.z / 1.5f;
		//		totalAxcellSpeed_.z += playerAxcell_.z / 1.5f;
		//	}
		//	else
		//	{
		//		//プレイヤーのポジションを代入(動いた分だけ進むように初期位置を引く)
		//		if(playerPos.y >= 6.0f)
		//		{
		//			//target_.y += playerAxcell_.y;
		//			//eye_.y += playerAxcell_.y;
		//		}

		//		totalAxcellSpeed_.y = playerAxcell_.y / 10.0f;

		//		/*target_.z += playerAxcell_.z;
		//		eye_.z += playerAxcell_.z;*/
		//	}

		//	if(stopTimer_ >= kStopTime_)
		//	{
		//		stopTimer_ = 0;
		//		slowMoveqQuantity_ = 0.0f;
		//		moveSlowTimer_ = kSlowDownTime_;
		//	}

		//	if(moveSlowTimer_ <= 0)
		//	{
		//		
		//	}

		//	//止まった状態から通常時のスピードに加速して戻る処理
		//	if(slowMoveqQuantity_ > 0.0f && stopTimer_ < kStopTime_ )
		//	{
		//		//totalAxcellSpeed_.z += PlayEaseOutQuint(returnTimer_, 0.0f, );
		//		//減速量分を複数回に分けて足していく
		//		if(backNormalTimer_ < kBackNormalTime_)
		//		{
		//			backQuantity_ = slowMoveqQuantity_ / kBackNormalTime_;
		//			backNormalTimer_++;
		//			totalAxcellSpeed_.z += backQuantity_;
		//		}
		//		else
		//		{
		//			slowMoveqQuantity_ = 0.0f;
		//			backNormalTimer_ = 0.0f;
		//		}
		//	}
		//}
		//else
		//{
		//	//急に止まっているからおかしい
		//	//徐々に減速させる
			if(isPlayerStoped == true && isSlowDown_ == false) {}
			if(playerAxcell_.z <= 0.0f) {}

		//	{
		//		isSlowDown_ = true;
		//		stopTimer_ = 0;
		//		slowMoveqQuantity_ = 0.0f;
		//		moveSlowTimer_ = kSlowDownTime_;
		//	}

		//	if(stopTimer_ >= kStopTime_)
		//	{
		//		stopTimer_ = 0;
		//		slowMoveqQuantity_ = 0.0f;
		//		moveSlowTimer_ = kSlowDownTime_;
		//	}

		//	//totalAxcellSpeed_ = Vector3Zero();

		//	//if(isSlowDown_ == true)
		//	//{
		//	//	float x = 0;
		//	//	x = moveSlowTimer_ / kSlowDownTime_;
		//	//	axcelRate_ = easeOutQuint(x);

		//	//	/*axcelRate_ = PlayEaseOutQuint(moveSlowTimer_, 0.5f, -0.5f, kSlowDownTime_);
		//	//	totalAxcellSpeed_.z *= axcelRate_;*/

		//	//	//totalAxcellSpeed_.z = PlayEaseOutQuint(moveSlowTimer_, playerAxcell_.z, -playerAxcell_.z, kSlowDownTime_);

		//	//	if(moveSlowTimer_ > 0)
		//	//	{
		//	//		moveSlowTimer_--;
		//	//	}

		//	//	if(totalAxcellSpeed_.z <= 0)
		//	//	{
		//	//		totalAxcellSpeed_.z = 0;
		//	//	}
		//	//}

		//	if(isSlowDown_ == true)
		//	{
		//		stopTimer_++;

		//		if(moveSlowTimer_ > 0)
		//		{
		//			moveSlowTimer_--;
		//		}

		//		/*float x = 0;
		//		x = moveSlowTimer_ / kSlowDownTime_;
		//		axcelRate_ = easeOutQuint(x);*/

		//		//axcelRate_ = 0.2 * moveSlowCount_;

		//		if(totalAxcellSpeed_.z >= 0.01f)
		//		{
		//			axcelRate_ = 0.9f;
		//		}
		//		else
		//		{
		//			axcelRate_ = 0.0f;
		//			isSlowDown_ = false;
		//		}
		//		
		//		/*axcelRate_ = PlayEaseOutQuint(moveSlowTimer_, 0.5f, -0.5f, kSlowDownTime_);
		//		totalAxcellSpeed_.z *= axcelRate_;*/

		//		//totalAxcellSpeed_.z = PlayEaseOutQuint(moveSlowTimer_, playerAxcell_.z, -playerAxcell_.z, kSlowDownTime_);

		//		totalAxcellSpeed_.z *= axcelRate_;
		//		
		//		//スロー時の移動量を保存
		//		slowMoveqQuantity_ += totalAxcellSpeed_.z;
		//		
		//		if(totalAxcellSpeed_.z <= 0)
		//		{
		//			totalAxcellSpeed_.z = 0;
		//		}
		//	}
		//}

		//target_ += totalAxcellSpeed_;
		//eye_ += totalAxcellSpeed_;

		//eye_ = goalEyePos_ - initEyeDistance;
		//target_ = goalEyeTarget_ - initTargetDistance;

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
