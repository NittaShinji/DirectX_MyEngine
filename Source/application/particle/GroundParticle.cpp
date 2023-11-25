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
	for(int i = 0; i < 3; i++)
	{
		const float md_pos = 2.0f;
		setPos_.x = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f + playerPos.x + imGuiPos_[0];
		const float shiftY = -0.8f;
		setPos_.y = playerPos.y + shiftY + imGuiPos_[1];
		setPos_.z = playerPos.z + imGuiPos_[2];

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
		acc.z = md_acc + imGuiAcc_[2];

		Vector4 color;

		//色設定
		if(playerColor == Attribute::pink)
		{	
			color = { kColorPinkR,kColorPinkG,kColorPinkB,kColorPinkAlpha };
		}
		else if(playerColor == Attribute::yellow)
		{
			color = { kColorYellowR,kColorYellowG,kColorYellowB,kColorYellowAlpha };
		}

		if(playerColor == Attribute::pink) {}

		//色を変化させる
		Vector4 colorSpeed{ 0.0,0.0f,0.0f,0.0f };

		//初期ライフ
		const int32_t InitLife = 60;

		//追加
		if(GetIsMaxParticle() == false)
		{
			Add(InitLife, setPos_, setVel_, acc, colorSpeed,color, startScale_, endScale_);
		}
	}
}
