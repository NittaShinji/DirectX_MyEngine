#include "SecondJump2DParticle.h"
#include "ImGuiManager.h"

std::unique_ptr<SecondJump2DParticle> SecondJump2DParticle::Create(std::string fileName)
{
	// 3Dオブジェクトのインスタンスを生成
	std::unique_ptr<SecondJump2DParticle> instance = nullptr;
	instance = std::make_unique<SecondJump2DParticle>();

	if(instance == nullptr)
	{
		return nullptr;
	}

	instance->particleFileName_ = fileName;

	return instance;
}

void SecondJump2DParticle::Preparation(Vector3 playerPos, Attribute playerColor)
{

	//const float md_pos = 2.0f;
	setPos_.x = playerPos.x + imGuiPos_[0];
	const float shiftY = -0.3f;
	setPos_.y = playerPos.y + shiftY + imGuiPos_[1];
	const float shiftZ = 0.68f * 2.3f;
	setPos_.z = playerPos.z + shiftZ + imGuiPos_[2];

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
	acc.z = md_acc + imGuiAcc_[2];

	//色を変化させる
	Vector4 colorSpeed{ 0.0f,0.0f,0.0f,0.04f };

	Vector4 startColor = { 1.0f,1.0f,1.0f,1.0f };
	Vector4 endColor = { 1.0f,1.0f,1.0f,0.0f };
	endColor.w = 0.0f;

	if(playerColor == Attribute::pink) {}

	//初期ライフ
	const float InitLife = 25.0f;

	float rotation = 0.0f;
	float rotationSpeed = 0.0f;

	//追加
	if(GetIsMaxParticle() == false)
	{
		Add(InitLife, setPos_, setVel_, acc,startColor, endColor, colorSpeed, startScale_, endScale_, rotation,rotationSpeed);
	}

}

void SecondJump2DParticle::Update(Camera* camera)
{
	ParticleEmitter::Update(camera);

}

void SecondJump2DParticle::ImGuiUpdate()
{
	ImGui::Begin("SecondJumpParticle");
	ImGui::SetWindowPos(ImVec2(900, 0));
	ImGui::SetWindowSize(ImVec2(300, 100));

	ImGui::SliderFloat3("pos", imGuiPos_, -10.0f, 30.0f);
	ImGui::SliderFloat3("vel", imGuiVel_, -5.0f, 5.0f);
	ImGui::SliderFloat3("acc", imGuiAcc_, -0.05f, 0.05f);

	ImGui::End();
}
