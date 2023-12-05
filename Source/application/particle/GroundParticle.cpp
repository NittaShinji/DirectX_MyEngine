#include "GroundParticle.h"
#include "ImGuiManager.h"

std::unique_ptr<GroundParticle> GroundParticle::Create(std::string fileName)
{
	// 3Dオブジェクトのインスタンスを生成
	std::unique_ptr<GroundParticle> instance = nullptr;
	instance = std::make_unique<GroundParticle>();

	if(instance == nullptr)
	{
		return nullptr;
	}

	instance->particleFileName_ = fileName;

	return instance;
}

void GroundParticle::Preparation(Vector3 playerPos, Attribute playerColor)
{
	for(int32_t i = 0; i < 1; i++)
	{
		const float md_pos = 2.0f;
		setPos_.x = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f + playerPos.x + imGuiPos_[0];
		const float shiftY = -0.8f;
		setPos_.y = playerPos.y + shiftY + imGuiPos_[1];
		setPos_.z = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f + playerPos.z + imGuiPos_[2];

		setVel_.x = 0.0f + imGuiVel_[0];

		const float md_velY = 0.174f;
		setVel_.y = md_velY + imGuiVel_[1];

		const float md_velZ = -0.68f;
		setVel_.z = md_velZ + imGuiVel_[2];

		//重力に見立ててYのみ{-0.001f,0}でランダムに分布
		Vector3 acc{};
		const float md_acc = -0.017f;
		acc.x = imGuiAcc_[0];
		acc.y = md_acc + imGuiAcc_[1];
		//acc.z = md_acc + imGuiAcc_[2];

		//Vector4 endColor;
		Vector4 endColor = { 0.0f,0.0f,0.0f,1.0f };
		Vector4 color = { 0.0f,0.0f,0.0f,0.0f };

		//色設定
		if(playerColor == Attribute::pink)
		{
			//color = { kColorPinkR + imGuiColor_[0],kColorPinkG + imGuiColor_[1],kColorPinkB + imGuiColor_[2],0.0f + imGuiColor_[3] };
			color = { kColorPinkR + 0.500f,kColorPinkG + 0.300f,kColorPinkB + 0.500f,0.0f + imGuiColor_[3] };

		}
		else if(playerColor == Attribute::yellow)
		{
			//color = { kColorYellowR + imGuiColor_[0],kColorYellowG + imGuiColor_[1],kColorYellowB + imGuiColor_[2],0.0f + imGuiColor_[3] };
			color = { kColorYellowR + 0.500f,kColorYellowG  +0.300f,kColorYellowB  +0.500f,0.0f + imGuiColor_[3] };

		}
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
		float rotationSpeed = 0.01f;


		//色を変化させる
		Vector4 colorSpeed{ 0.02f,0.02f,0.02f,0.15f };

		//初期ライフ
		const float InitLife = 30.0f;

		//追加
		if(GetIsMaxParticle() == false)
		{
			Add(InitLife, setPos_, setVel_, acc, endColor, color, colorSpeed, startScale_, endScale_,rotation,rotationSpeed);
		}
	}

}

void GroundParticle::ImguiUpdate()
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
