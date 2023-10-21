#include "BlockParticle.h"

std::unique_ptr<BlockParticle> BlockParticle::Create()
{
	// 3Dオブジェクトのインスタンスを生成
	std::unique_ptr<BlockParticle> instance = nullptr;
	instance = std::make_unique<BlockParticle>();

	if(instance == nullptr)
	{
		return nullptr;
	}

	instance->Initialize();

	return instance;
}

void BlockParticle::Initialize()
{
	startScale_ = { 1.0f,1.0f,1.0f };
	endScale_ = { 0.0f,0.0f,0.0f };
}


void BlockParticle::Preparation(const Vector3& popPos, std::string fileName)
{

	const float md_pos = 2.0f;
	setPos_.x = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f + popPos.x + imGuiPos_[0];
	const float shiftY = -0.8f;
	setPos_.y = popPos.y + shiftY + imGuiPos_[1];
	setPos_.z = popPos.z + imGuiPos_[2];

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
	Vector4 colorSpeed{ 1.0f,-1.0f,-1.0f,1.0f };

	//追加
	if(GetIsMaxParticle() == false)
	{
		Add(fileName, 60, setPos_, setVel_, acc, colorSpeed, startScale_, endScale_);
	}

}
