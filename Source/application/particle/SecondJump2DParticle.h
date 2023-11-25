#pragma once
#include "ParticleManager.h"

class SecondJump2DParticle : public ParticleEmitter
{
public:

	static std::unique_ptr<SecondJump2DParticle> Create(std::string fileName);

	void Preparation(Vector3 playerPos, Attribute playerColor);

	void Update(Camera* camera) override;

	void ImGuiUpdate();

	void SetStartPop(bool isStartPop) { isStartPop_ = isStartPop; };

private:

	Vector3 setPos_;		//座標
	Vector3 setVel_;		//方向

	float startScale_ = 4.5f;	//初期スケール
	float endScale_ = 1.0f;	//終期スケール

	bool isStartPop_;

	float imGuiPos_[3]{ 0.0f,0.0f,0.0f };
	float imGuiVel_[3]{ 0.0f,0.0f,0.0f };
	float imGuiAcc_[3]{ 0.0f,0.0f,0.0f };
};