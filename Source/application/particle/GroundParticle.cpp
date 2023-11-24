#include "GroundParticle.h"
#include "ImGuiManager.h"

std::unique_ptr<GroundParticle> GroundParticle::Create()
{
	// 3Dオブジェクトのインスタンスを生成
	std::unique_ptr<GroundParticle> instance = nullptr;
	instance = std::make_unique<GroundParticle>();

	if(instance == nullptr)
	{
		return nullptr;
	}

	instance->Initialize();

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

		//色を変化させる
		if(playerColor == Attribute::pink)
		{
			Vector4 colorSpeed{ 1.0f,1.0f,1.0f,1.0f };
		}
		else if(playerColor == Attribute::yellow)
		{
			Vector4 colorSpeed{ 1.0f,1.0f,1.0f,1.0f };
		}

		//色を変化させる
		Vector4 colorSpeed{ 1.0f,-1.0f,-1.0f,1.0f };

		//追加
		if(GetIsMaxParticle() == false)
		{
			Add(60, setPos_, setVel_, acc, colorSpeed, startScale_, endScale_);
		}
	}
}

//void GroundParticle::Update(Camera* camera, Attribute attribute)
//{
//	/*Preparation()*/
//	ParticleEmitter::Update(camera,attribute);
//}
