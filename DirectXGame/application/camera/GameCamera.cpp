#include "GameCamera.h"
#include "ImGuiManager.h"


void GameCamera::Update(bool isPlayerMoving, Vector3 playerAxcell_, Vector3 pInitPos)
{
	if(isPlayerMoving == true)
	{
		//プレイヤーのポジションを代入(動いた分だけ進むように初期位置を引く)
		target_.z += playerAxcell_.z;
		eye_.z += playerAxcell_.z;
	}

	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void GameCamera::Reset()
{
	eye_ = { 30,7.5,-20 };
	target_ = { 0,5,5 };
}

void GameCamera::ImGuiUpdate()
{
	//スプライトの編集ウインドウの表示

	ImGui::Begin("Camera");
	ImGui::SetWindowPos(ImVec2(300, 0));
	ImGui::SetWindowSize(ImVec2(300, 100));

	ImGui::SliderFloat("eyeX", &eye_.x, -100.0f, 50.0f);
	ImGui::SliderFloat("eyeY", &eye_.y, -100.0f, 50.0f);
	ImGui::SliderFloat("eyeZ", &eye_.z, -100.0f, 1000.0f);

	ImGui::End();

}
