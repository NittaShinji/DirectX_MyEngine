#include "HitParticle2D.h"
#include "ImGuiManager.h"

std::unique_ptr<HitParticle2D> HitParticle2D::Create(std::string fileName)
{
	// 3Dオブジェクトのインスタンスを生成
	std::unique_ptr<HitParticle2D> instance = nullptr;
	instance = std::make_unique<HitParticle2D>();

	if(instance == nullptr)
	{
		return nullptr;
	}

	instance->particleFileName_ = fileName;

	return instance;
}

void HitParticle2D::Preparation(Vector3 playerPos)
{
	for(int32_t i = 0; i < 3; i++)
	{
		const float md_pos = 2.0f;
		setPos_.x = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f + playerPos.x + imGuiPos_[0] + i * 0.2f;
		const float shiftY = -0.8f;
		setPos_.y = playerPos.y + shiftY + imGuiPos_[1] - i * 0.2f;
		setPos_.z = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f + playerPos.z + imGuiPos_[2] - i * 0.2f;

		setVel_.x = 0.0f + imGuiVel_[0];

		const float md_velY = 0.0f;
		setVel_.y = md_velY + imGuiVel_[1];

		const float md_velZ = 0.0f;
		setVel_.z = md_velZ + imGuiVel_[2];

		//重力に見立ててYのみ{-0.001f,0}でランダムに分布
		Vector3 acc{};
		const float md_acc = 0.0f;
		acc.x = imGuiAcc_[0];
		acc.y = md_acc + imGuiAcc_[1];
		//acc.z = md_acc + imGuiAcc_[2];

		//Vector4 endColor;
		Vector4 endColor = { 1.0f,1.0f,1.0f,0.0f };
		const Vector4 startColor = { 1.0f,1.0f,1.0f,1.0f };

		//色設定
		//if(playerColor == Attribute::pink)
		//{
		//	/*color = { 0.0f,0.0f,0.0f,0.0f };
		//	endColor = { kColorPinkR,kColorPinkG,kColorPinkB,kColorPinkAlpha };*/
		//}
		//else if(playerColor == Attribute::yellow)
		//{
		//	/*color = { 0.0f,0.0f,0.0f,0.0f };
		//	endColor = { kColorYellowR,kColorYellowG,kColorYellowB,kColorYellowAlpha };*/
		//}

		//color = { 1.0f,1.0f,1.0f,1.0f };

		const float md_rotate = 1.0f;
		float rotation = (float)rand() / RAND_MAX * md_rotate - 0.0f;
		float rotationSpeed = 0.005f;

		//色を変化させる
		Vector4 colorSpeed{ 0.02f,0.02f,0.02f,0.05f };

		//初期ライフ
		const float InitLife = 60.0f;

		//追加
		if(GetIsMaxParticle() == false)
		{
			Add(InitLife, setPos_, setVel_, acc, startColor,endColor, colorSpeed, startScale_, endScale_, rotation, rotationSpeed);
		}
	}

}

void HitParticle2D::ImguiUpdate()
{
	ImGui::Begin("groundParticle");
	ImGui::SetWindowPos(ImVec2(900, 0));
	ImGui::SetWindowSize(ImVec2(300, 100));

	ImGui::SliderFloat3("pos", imGuiPos_, -10.0f, 30.0f);
	ImGui::SliderFloat3("vel", imGuiVel_, -5.0f, 5.0f);
	ImGui::SliderFloat3("acc", imGuiAcc_, -0.05f, 0.05f);
	ImGui::SliderFloat4("color", imGuiColor_, -0.5f, 0.5f);

	ImGui::End();
}

